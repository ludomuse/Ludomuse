export BULLET=cocos2d/external/bullet/prebuilt/linux/64-bit
cp prebuilt/libbullet.a $BULLET
rm $BULLET/libBulletCollision.a $BULLET/libBulletDynamics.a
ln -s libbullet.a $BULLET/libBulletCollision.a
ln -s libbullet.a $BULLET/libBulletDynamics.a
