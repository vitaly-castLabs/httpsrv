# httpsrv
A simple HTTP(S) server which sucks much less than `python3 -m http.server` (96k vs 3.5k requests per second with `wrk -t4 -c200 -d10s <addr>`). Another selling point is an unbearably awesome name.

Based on my fork of tiny but mighty [cpp-httplib](https://github.com/vitaly-castLabs/cpp-httplib).

## How to build
```bash
git clone https://github.com/vitaly-castLabs/httpsrv.git && \
cd httpsrv && \
cmake . && \
cmake --build . --config Release
```

## How to run
`cd` into the folder you want to serve and run `httpsrv [port] [cert] [priv-key]` (self-signed certificate and privare key are provided). Some examples:
```
httpsrv                        - serve http on port 8080
httpsrv 80                     - serve http on port 80 (might require sudo)
httpsrv 8443 cert.pem pkey.pem - serve https on port 8443
httpsrv 8000 --sab             - serve http on port 8000 with SharedArrayBuffer headers
```
