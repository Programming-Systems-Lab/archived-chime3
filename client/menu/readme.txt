
 Read Me
 =======


 What is Crystal Space
 =====================

 Crystal Space is an open-source 3D engine. It can be used
 for a variety of 3D visualization tasks. Many people will
 probably be interested in using Crystal Space as the basis
 of a 3D game, for which it is very well suited.


 What you have downloaded
 ========================

 The Crystal Space 0.18001 beta release binaries package for Windows.

 A collection of the test applications and dynamic link
 library plugins, which from the code (which is freely
 downloadable) go to make up Crystal Space.

 For info on each of the test applications. See "app_manifest.txt"


 Tips for running the applications
 =================================

 Some Crystal Space applications e.g. "walktest" have there own
 configuration files, with which you can alter the way it looks
 and the renderer it uses i.e. Software (default), OpenGL or
 Direct3D. The "walktest" configuration file is called "cryst.cfg"
 and can be found in the "CS\data\config" directory.

 Example 1:

 If you want to run in fullscreen mode?

 Look in "cryst.cfg" for the line:

 Video.FullScreen = no

 and change the "no" to "yes".

 Example 2:

 If you wanted to use the OpenGL renderer?

 Look for the line in the following section:

 ; Video driver
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.software.offscreen
 System.PlugIns.VideoDriver = crystalspace.graphics3d.software
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.opengl
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.direct3d.dx61
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.glide.2
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.glide.3
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.line

 without ";" at the beginning and add one then remove the symbol
 the line indicating OpenGL like so:

 ; Video driver
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.software.offscreen
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.software
 System.PlugIns.VideoDriver = crystalspace.graphics3d.opengl
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.direct3d.dx61
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.glide.2
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.glide.3
 ;System.PlugIns.VideoDriver = crystalspace.graphics3d.line

 Always save your changes and re-run the "walktest" application. I will
 leave you to experiment as there are too many options to play with and
 we wouldn't want too spoil your fun.


 Getting more
 ============

 Go to:

 http://crystal.linuxgames.com

 There you will be able to obtain news, more information
 and links to the complete 0.18001 Crystal Space package -
 source code and more.



 Enjoy - The Crystal Space team.
 


