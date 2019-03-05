This version of OpenSCAD is built on top of the official one.

Read the official OpenSCAD README to prepare for a build et cetera and come back to this project and try their instructions on this project. You also have to prepare the Pi to use swap memory (2 GB swap). There are good instructions on the official homepage for the Pi on how to prepare it for building such a big project as OpenSCAD, and which packages to install before proceeding to build.

This experimental version of OpenSCAD is aimed for the Raspberry Pi 3 with OpenGLES2 as renderer. When the program has been compiled (use branch opengles2) on Raspberry Pi 3, you have to install it somewhere, e.g. /usr/local/bin. You also have to copy the two shader files in folder src/shaders to the same folder as the program file - otherwise the program will silently fail to render any object, but will start. Use the scroll wheel to zoom in/out, and use the left mouse button and pan the mouse at the same time to rotate the object (which will just rotate in one direction). These controls are the only ones that can be used so far and they are not in production state yet. Many UI elements and the menu is somewhat confined regarding what can be used/selected, because all functions that perform something with the view (e.g OpenGL commands under the hood) are not implemented at the moment.

Good luck with testing this version!

//Best Regards Henrik