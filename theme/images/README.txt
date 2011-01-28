
Also, they should be converted to indexed to make them as small as possible on the wire.

All PNGs need to be processed with 'pngcrush' to get them displaying correctly in IE!

  pngcrush -rem cHRM -rem gAMA -rem iCCP -rem sRGB in.png out.png

