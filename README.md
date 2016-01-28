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
  - 

remplacez compile par run pour lancer directement sur le périphérique.
  

Si vous voulez utiliser la machine Vagrant :
--------------------------------------------

Clonez le projet avec la commande 

`` 
$ git clone https://github.com/ludomuse/Ludomuse.git
``

Une fois le projet crée déplacer vous dans le répertoire du projet avec une invite de commande et taper la commande :

``
host$ vagrant up
``

Ceci va démarrer la machine virtuelle qui va télécharger toutes les dépendances requises pour la compilation pour android.
Une fois cette commande terminée (peut prendre un certain temps) entrez la commande :

``
host$ vagrant ssh
``

Pour vous logger sur la machine virtuelle.
La manière dont les projets cocos sont gérés génère beaucoup de fichier qui ne peuvent pas être versionnés sur github (trop larges). Ce qui oblige une manipulation étrange.

Entrez la commande 

``
vagrant$ cocos new LudoMuse -l cpp -p com.IHMTEK.LudoMuse -d ~/
``

ceci va créer un projet cocos dans le répertoire personnel (qui contient tous les fichiers trop larges pour git)
collez ensuite le contenu du projet récupéré depuis fit dedans pour fusionner les projets.

``
vagrant$ cp -r /vagrant/* ~/LudoMuse
``

/vagrant/ depuis la machine virtuelle est un répertoire partagé qui pointe sur le répertoire clôné depuis git sur votre machine hôte.
Ensuite recopiez le projet dans le sens inverse pour le remettre dans le répertoire partagé.

``
vagrant$ cp -r ~/LudoMuse/* /vagrant/
``

vous devriez voir les fichiers mis à jour depuis la machine hôte.
Engin tapez la commande :

``
vagrant$ cocos compile -p android
``

vous pouvez ensuite déployer sur le périphérique en lançant la commande 

``
host$ cd bin/debug/android
host$ adb install -r LudoMuse-debug.apk
``

vous pouvez ensuite lancer l'application depuis votre périphérique android
