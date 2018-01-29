export CONFIG=debug
if [[ $1 == release ]]; then CONFIG=release; fi
mv bin/$CONFIG/linux/LudoMuse bin/$CONFIG/linux/LudoMuse.exe
chrpath -r '$ORIGIN:$ORIGIN/../' bin/$CONFIG/linux/LudoMuse.exe
mkdir ./bin/$CONFIG/linux/lib
cp prebuilt/libfmod.so.6 ./bin/$CONFIG/linux/lib
