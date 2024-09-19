# httpsrv
A simple HTTP(S) server which sucks much less than `python3 -m http.server` (96k vs 3.5k requests per second with `wrk -t4 -c200 -d10s <addr>`). Another selling point is an unbearably awesome name.

Based on my fork of tiny but mighty [cpp-httplib](https://github.com/vitaly-castLabs/cpp-httplib).
