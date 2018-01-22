mv bin/debug/linux/LudoMuse bin/debug/linux/LudoMuse.exe
chrpath -r '$ORIGIN:$ORIGIN/../' bin/debug/linux/LudoMuse.exe
mkdir ./bin/debug/linux/lib
cp prebuilt/libfmod.so.6 ./bin/debug/linux/lib
