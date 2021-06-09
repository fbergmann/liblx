A note about these test files.
-----------------------------

These files were originally in libSBML, and had been autogenerated.
They used to have a warning like this:

```
# ====== WARNING ===== WARNING ===== WARNING ===== WARNING ===== WARNING ======
#
# DO NOT EDIT THIS FILE.
#
# This file was generated automatically by converting the file located at
# src/xml/test/TestXMLNode.c
# using the conversion program dev/utilities/translateTests/translateTests.pl.
# Any changes made here will be lost the next time the file is regenerated.
```

The `translateTests.pl` script is in the libSBML repository, but has not been included
in libLX. This is because, at the time of writing, the script has not been run for 11
years, and it's not even certain it would now do what is required anyway.

So these files are being edited directly.

The files which these would have been generated from would have been the libSBML
equivalent of C and C++ files. For example, the file

`liblx/src/bindings/python/test/xml/TestXMLAttributes.py`

would have been generated from

`liblx/src/liblx/xml/test/TestXMLAttributes.cpp`

and similarly for the C file (`TestXMLAttributesC.c`) generating the file `TestXMLAttributesC.py`. 


Matthew Gillman, UCL, 8th June 2021