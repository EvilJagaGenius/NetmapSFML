# NetmapSFML
C++/SFML port of the existing Netmap 1.0 code

This is meant to be used with the CodeBlocks IDE.  To set it up, you'll need to supply your own SFML libraries.  I didn't include them as they're ~28 megs and I didn't want to make this any bigger than it needed to be.

Create a folder called SFML_Libraries in the same directory as NetmapSFML.cbp.  Put the bin, include, and lib folders from your own pack of SFML files in there.  Finally, put the required DLLs in the bin folders.  For bin/Debug, put in sfml-graphics-d-2.dll, sfml-system-d-2.dll, and sfml-window-d-2.dll in the folder.  For bin/release, use sfml-graphics-2.dll, sfml-graphics-2.dll, and sfml-graphics-2.dll.

As for downloads, I used the "Code::Blocks - 32-bit" download from https://www.sfml-dev.org/download/sfml/2.5.1/

From there you _should_ be good to go, just open NetmapSFML.cbp and get to work.  Poke me if things blow up.