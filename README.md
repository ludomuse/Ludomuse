# Ludomuse [![Build Status](https://travis-ci.org/ludomuse/Ludomuse.svg?branch=master)](https://travis-ci.org/ludomuse/Ludomuse)
Jeu coopératif et éducatif pour les musées. 


## Téléchargement
Vous pouvez télécharger la dernière version stable de LudoMuse en cliquant sur le lien ci-dessous : 
## [Télécharger LudoMuse version stable](https://github.com/ludomuse/LudoMuse/releases/latest)

Les version de développement (nightlies) sont disponibles ici :

[Parcourir les versions de développement](https://ihmtek-services.com/files/LudoMuse/releases/nightlies/LudoMuse)


## Pré-requis

- Il vous faudra soit une installation de cocos2d-x ainsi que de toute ses dépendances dispoible ici : http://www.cocos2d-x.org/
Et suivez le guide d'installation correspondant à votre plateforme : http://www.cocos2d-x.org/docs/static-pages/installation.html
- Vous pouvez sinon utiliser la machine virtuelle préconfigurée pour la compilation et le déploiement sur Android. Pour cela vous devrez installer Virtualbox (https://www.virtualbox.org/wiki/Downloads) pour votre plateforme ainsi que Vagrant (https://www.vagrantup.com/downloads.html). Il vous faudra également une installation des outils android (en particulier adb) pour installer les apk générés par la machine virtuelle sur votre périphérique android.


## Installation


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
  

## Déploiement Editeur

étape supplémentaire sur linux après avoir compilé avec cocos :
``
$ bash changeExeRPATH.sh
``
Ce script va modifier l'endroit ou l'executable va chercher ses dépendances. Icii dans le sous dossier lib.


Lors du déploiement de LudoMuse dans l'éditeur (simulateur) copier l'exécutable situé dans le sous dossier bin/debug/platform/ dans le dossier de l'exécutable LudoMuseEditor




## Problèmes connus

Pour les vieilles versions d'android, pensez bien à préciser la version de l'API à utiliser ou LudoMuse risque de craser avec pour seul message d'erreur l'impossiblité de résoudre certains symboles.

Example pour l'API 22 utilisée pour le développement de LudoMuse:
``
$ cocos compile -p android --ap android-22
``
