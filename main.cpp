#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <memory>

#include "httplib.h"

static inline bool fileExists(const std::string& fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void printUsage(const char* argv0) {
    std::cerr << "\nUsage: " << argv0 << " [port] [cert] [key]\n" <<
        "       " << argv0 << "                        - serve http on port 8080\n" <<
        "       " << argv0 << " 80                     - serve http on port 80 (might require sudo)\n" <<
        "       " << argv0 << " 8443 cert.pem pkey.pem - serve https on port 8443\n" <<
        "       " << argv0 << " 8000 --sab             - serve http on port 8000 with SharedArrayBuffer headers (--sab is only supported as last arg for now)\n";
}

std::uint16_t parsePort(const char* arg) {
    auto port = std::atoi(arg);
    if (port <= 0 || port > 65535 || std::to_string(port) != arg) {
        std::cerr << "Invalid port (" << arg << ")\n";
        port = 0;
    }
    return static_cast<std::uint16_t>(port);
}

int main(int argc, char* argv[]) {
    std::uint16_t port;
    std::string cert;
    std::string key;
    bool enableSharedArrayBuffer = false;

    if (argc > 1 && !strcmp(argv[argc - 1], "--sab")) {
        enableSharedArrayBuffer = true;
        --argc;
    }

    if (argc == 1) {
        port = 8080;
    }
    else if (argc == 2) {
        port = parsePort(argv[1]);
        if (!port) {
            printUsage(argv[0]);
            return -1;
        }
    }
    else if (argc == 4) {
        port = parsePort(argv[1]);
        if (!port) {
            printUsage(argv[0]);
            return -1;
        }

        cert = argv[2];
        key = argv[3];
    }
    else {
        std::cerr << "Invalid number of arguments\n";
        printUsage(argv[0]);
        return -1;
    }

    if (!cert.empty() && !fileExists(cert)) {
        std::cerr << "Can't open the certificate file (" << cert << ")\n";
        return -1;
    }

    if (!key.empty() && !fileExists(key)) {
        std::cerr << "Can't open the private key file (" << key << ")\n";
        return -1;
    }

    std::unique_ptr<httplib::Server> svr(cert.empty() ? new httplib::Server() : new httplib::SSLServer(cert.c_str(), key.c_str()));

    // https://github.com/TechEmpower/FrameworkBenchmarks/wiki/Project-Information-Framework-Tests-Overview#plaintext
    svr->Get("/plaintext", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello, World!\r\n", "text/plain; charset=UTF-8");
    });

    if (!svr->set_mount_point("/", ".")) {
        std::cerr << "Failed to set the mount point\n";
        return -1;
    }

    svr->set_post_routing_handler([&enableSharedArrayBuffer](const auto& req, auto& res) {
        if (req.path != "/plaintext") {
            res.set_header("Access-Control-Allow-Origin", "*");
            // add some CORS headers in order to make SharedArrayBuffers work
            if (enableSharedArrayBuffer) {
                res.set_header("Cross-Origin-Opener-Policy", "same-origin");
                res.set_header("Cross-Origin-Embedder-Policy", "require-corp");
            }
        }
    });

    std::cout << "Listening on port " << port << " (http" << (cert.empty() ? ")" : "s)") << std::endl;

    if (!svr->listen("0.0.0.0", port)) {
        std::cerr << "Port is already in use, terminating...\n";
        return -1;
    }

    return 0;
}
