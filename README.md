![Linux CI](https://github.com/flightaware/mdsplib/workflows/Linux%20CI/badge.svg)
![Windows CI](https://github.com/flightaware/mdsplib/workflows/Windows%20CI/badge.svg)

METAR Decoder Software Package Library
===

METAR is a format for reporting weather information that is commonly used in aviation worldwide.

mdsplib parses metars into a data structure that breaks out different weather conditions such as winds, clouds, visibility, temperature, and dew point.  Many weather conditions are included such as rain, snow, lightning, smoke, haze, fog, hail and thunderstorms.

### ABOUT THE MDSP LIBRARY

The MDSP Library provides a programmer with two functions, the major one being decode_metar, which decodes a METAR into C structures provided by the library. Also print_decoded_metar, which prints out a decoded METAR structure.

The MDSP was written by Carl McCalla and released to the public by the US National Weather Service. The MDSP Library is the original MDSP modified to compile and run on *NIX systems.

Please read the file README.MDSP included in this package. It's the README that comes with the original MDSP from the NWS. You must read the LICENSE before using this program (it's the LGPL).

### COMPILING

Switch into the mdsplib directory and run "make". If you also want the test program to be produced, run "make metar_test" instead. Both commands will produce "metar.a". This is the library. The header file for this library is "metar.h". The test application will be named "dmetar", it takes no parameters.

"make install" will install the include file to /usr/local/include and the library to /usr/local/lib by default.  There is no modern autoconf or cmake-style build architecture so you are limited to editing the source by hand to get it to build on odd operating systems and/or architectures.  This isn't super likely or won't take much, though, as it is pretty much sstraight C and doesn't delve into the operating system very much.

### USING

After installing the header file "metar.h" and the library "metar.a", include and use the functions and structures as defined in that file.

### FLIGHTAWARE

FlightAware has made a number of fairly important bug fixes to mdsplib over the years.  Also we've smoothed things out, expanded function names past fifteen characters and you know, stuff like that to improve readability.

As the sourceforge project that's reported to be mdsplib's home seems pretty inactive and isn't configured to receive bug reports or patches, and as we're using git internally, it was ever so easy to put this repo on github.

As of September 2014 mdsplib can ingest more than 350,000,000 METARs issued between 2008 and 2014 without crashing.  The same can not be said for unpatched mdsplib.

### SECURITY

mdsplib was written in the early 90s and is not up to date with current security practices for C coding.  Specifically it generally assumes that target strings are large enough to contain whatever is being assembled into them.

### PARSING FAILURES

mdsplib has other problems like because of how it parses and how it is structured in many cases if there is more than one report of something in the remarks, all the remarks pertaining to that thing will be thrown away.  That's not good and it's not going to be easy to change it.  We're thinking about what to do about it.

### ORIGINAL CONTACT

If you find any bugs/errors or otherwise have questions about the MDSP Library let me know. I'd also welcome any changes and additions to the code.

Eric McCarthy <eric@limulus.net>


