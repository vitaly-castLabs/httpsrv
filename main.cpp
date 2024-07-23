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
        "       " << argv0 << " 8443 cert.pem pkey.pem - serve https on port 8443\n";
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
    if (!svr->set_mount_point("/", ".")) {
        std::cerr << "Failed to set the mount point\n";
        return -1;
    }

    std::cout << "Listening on port " << port << " (http" << (cert.empty() ? ")" : "s)") << std::endl;

    return (svr->listen("0.0.0.0", port) ? 0 : -1);
}
