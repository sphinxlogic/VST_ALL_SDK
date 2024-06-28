#!/bin/bash
#
#
# Copyright 2014 by Avid Technology, Inc.
#
#


#
# Initialize the shell
#

# Change to the shell's directory
# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)"
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd && echo x)"
#DIR="${DIR%x}"
SUBDIR="/."
cd "$DIR$SUBDIR"


#
# Run Doxygen
#

# Remove any previous Doxygen output
if [ -d "output" ]; then
	rm -r output
fi

# Run Doxygen
/Applications/Doxygen.app/Contents/Resources/doxygen AAX_SDK_Doxyfile >> doxygen_log.txt


#
# Fix up the output
#

# Doxygen does not copy over images that are included using the <IMG> tag, so we must manually copy the contents of the /img directory to output
if [ -d "output/html" ]; then
	if [ -d "img" ]; then
		cp -r img/ output/html/
	fi
fi

# Put dot images into 'centered' class
perl -e 's#\<img(.*?)src\=\"dot_#\<img class="centered"\1src\=\"dot_#g;' -pi $(find ${DIR}/output/html/*.html -type f)
