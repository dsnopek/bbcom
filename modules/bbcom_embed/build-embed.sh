#!/bin/bash

REQUIRE=/home/dsnopek/prj/requirejs-0.14.5
BUILDSH=$REQUIRE/build/build.sh
CLOSURE=$REQUIRE/build/lib/closure/compiler.jar
LINGWO_DICTIONARY=/home/dsnopek/prj/lingwo/lingwo_dictionary
STUBS=$LINGWO_DICTIONARY/js/require/require-stubs.js
LINGWO_KORPUS=$LINGWO_DICTIONARY/lingwo_korpus
LESSC=/home/dsnopek/prj/lingwo/drupal/sites/all/modules/less/lessphp/lessc.inc.php

for profile in *.build.js; do
	name=`echo $profile | sed -e s,\.build\.js$,,`

	# build our code
	$BUILDSH $profile

	# combine the stubs and the code to start us up
	echo "(function () {"      > $name.temp.js
	cat $STUBS                >> $name.temp.js
	cat $name.uncompressed.js >> $name.temp.js
	echo "require(['bibliobird/Embed'], function (bb) { bb.start(); });" \
	                          >> $name.temp.js
	echo "})();"              >> $name.temp.js
	mv $name.temp.js $name.uncompressed.js

	# run the closure compiler
	java -jar $CLOSURE --js $name.uncompressed.js --js_output_file $name.js
done

echo "Generating annotation-reader.css..."
php <<EOF
<?php
require_once('$LESSC');
\$less = new lessc();
file_put_contents('annotation-reader.css', \$less->parse(file_get_contents('$LINGWO_KORPUS/annotation-reader.less')));
EOF

