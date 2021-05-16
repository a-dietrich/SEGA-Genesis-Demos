GDK=/Users/andreas/Development/Genesis/Tools/SGDK
export GDK

make -f $GDK/makefile.gen
$GDK/..//Wine/usr/bin/wine $GDK/../GensKMod/gens.exe $PWD/out/rom.bin
