
 Read Me
 =======


 What is Hime Client
 =====================



 What you have downloaded
 ========================

 Tips for running the applications
 =================================

 If you want to run in fullscreen mode?

 Look in "chime.cfg" for the line:

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



 Enjoy - The Chime World.
 


