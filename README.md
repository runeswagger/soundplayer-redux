soundplayer-redux
=================

A rewrite of a media player I wrote mostly to learn various audio API. soundplayer-redux, or sp2 for short, aims to be more modular and extensible than the original.

Soundplayer 2 is beginning to mature with wav, mp3, spc, and aac codecs attaining a high level of functionality. Modules are still below standards in regards to error checking and handling. Nonfunctional modules are currently advancing in development and may be available soon.

This project has quite a few dependancies. If you want to compile with fewer modules; comment out the USE_ line in config.h, comment out the initializer line in sp.c (play[][]), and use # to comment out the lines in the Make file that reference the module. do not comment out any of the file extensions in sp.c as detecting filetypes uses the index in the array to set format.
