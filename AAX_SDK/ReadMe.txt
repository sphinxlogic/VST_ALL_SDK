=============================================

AAX SDK

Copyright 2014-2016 by Avid Technology, Inc.
All rights reserved.
	
CONFIDENTIAL: This document contains confidential information.  Do not 
read or examine this document unless you are an Avid Technology employee
or have signed a non-disclosure agreement with Avid Technology which protects
the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
OF Avid Technology, INC.

=============================================

The SDK's Documentation directory includes HTML documentation with a complete set
of code-generated Doxygen documentation.

In addition to full code documentation, the HTML documentation includes guides to
many other aspects of AAX plug-in development. The "Manual" tab in the
documentation will reveal many pages of resources including a basic introduction
to the AAX architecture.

To compile the code-generated documentation:
  1) Download and install the latest version of Doxygen from www.doxygen.org
  2) (Optional for dot) Download and install the latest Graphviz tools from
     www.graphviz.org
  3) (Optional for MSC) Download and install the latest version of the mscgen tool
     from http://www.mcternan.me.uk/mscgen/
  4) Run Doxygen from the AAX_SDK/Documentation/Doxygen directory using the
     included AAX_SDK_Doxyfile config file. On OS X, simply run the
     generate_aax_sdk_doxygen.sh script to re-compile and format the documentation.
     This script requires that the doxygen command-line tool is included in your
     PATH variable.
