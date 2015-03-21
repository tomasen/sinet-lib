sinet-lib C++ (.lib/.dll) approach to simplify internet download/upload

dependencies:

  * curl
  * openssl

design to be:

  * thread-safe
  * able to cancel safely during data transition
  * cross-platform
  * simplify linking and using
  * handle multiple requests in a graceful manner
  * support HTTPS
  * written in C++ but is compatible with C program linking
  * dll boundary safe
  * ...

manual:

  * http://code.google.com/p/sinet-lib/wiki/sinetlibmanpage