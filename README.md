soundplayer-redux
=================

A rewrite of a media player I wrote mostly to learn various audio API. soundplayer-redux, or sp2 for short, aims to be more modular and extensible than the original. The chainable branch implements a linked list structure for transforming data in stages at each module. This new structure eliminates the API differences between a codec module and an output module. It also allows each module to function without any special knowledge of the other modules. It also features a rudimentary message passing system for modules.

This project has quite a few dependancies. If you want to compile with fewer modules; comment out the USE_ line in config.h, comment out the initializer line in sp.c (handlers[]), and use # to comment out the lines in the Make file that reference the module. do not comment out any of the file extensions in sp.c as detecting filetypes uses the index in the array to set format.
