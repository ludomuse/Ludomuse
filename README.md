# Ludomuse
Jeu coopératif et éducatif pour les musées.

Pré-requis
==========
- Il vous faudra soit une installation de cocos2d-x ainsi que de toute ses dépendances dispoible ici : http://www.cocos2d-x.org/
Et suivez le guide d'installation correspondant à votre plateforme : http://www.cocos2d-x.org/docs/static-pages/installation.html
- Vous pouvez sinon utiliser la machine virtuelle préconfigurée pour la compilation et le déploiement sur Android. Pour cela vous devrez installer Virtualbox (https://www.virtualbox.org/wiki/Downloads) pour votre plateforme ainsi que Vagrant (https://www.vagrantup.com/downloads.html). Il vous faudra également une installation des outils android (en particulier adb) pour installer les apk générés par la machine virtuelle sur votre périphérique android.


Installation
============


Si vous avez configuré cocos2d-x :
----------------------------------

créez un nouveau projet cocos :
déplacer vous à l'endroit ou vous voulez créer le projet et tapez

``
$  cocos new LudoMuse -l cpp -p com.IHMTEK.LudoMuse
``

créer un réperoire tmp `` $ mkdir tmp `` et déplacez vous dedans `` $ cd tmp ``
clônez le dépôt git avec la commande 

``
$ git clone https://github.com/ludomuse/Ludomuse.git
``

copiez tout le contenu du dépôt dans le projet que vous avez créé :

``
$ cp -r Ludomuse/* Ludomuse/.* ../LudoMuse/
``

vous pouvez supprimer le répertoire tmp `` $ cd ../ && rm -r tmp ``

Il ne reste plus qu'à compiler pour la plateforme de votre choix `` $ cd LudoMuse `` puis :

``
$ cocos compile -p <platform>
``

et en remplaçant <platform> par une des plateformes suivantes :
  - win32
  - android
  - linux

remplacez compile par run pour lancer directement sur le périphérique.
  

