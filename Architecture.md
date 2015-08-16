this page is only a demo.
it seems better to use a google doc for those type of rather complex document

see : [introduction à l'architecture matérielle des ordinateurs](http://docs.google.com/Doc?id=dxhxfnh_21873337vmcz)


# Brève introduction à l'architecture des ordinateurs : #

  * historique
  * les composants / la technologie
  * la programmation
  * les systèmes d'exploitation


## Les premiers automates mécaniques ##

  * horloges à eau (clepsydres)

![http://soeurs-blanches.cef.fr/chapitre/clepsydre.jpg](http://soeurs-blanches.cef.fr/chapitre/clepsydre.jpg) Munichglock

  * machine à calculer de Pascal


![http://pages.globetrotter.net/pcbcr/images/machine.jpg](http://pages.globetrotter.net/pcbcr/images/machine.jpg)

  * automates musicien de Vaucanson

canardduck

  * la machine analytique de C. Baabage

![http://oz.irtc.org/ftp/pub/stills/1999-04-30/babbage.jpg](http://oz.irtc.org/ftp/pub/stills/1999-04-30/babbage.jpg)

L'électricité et le transistor
L'utilisation de l'électricité permet de réaliser des systèmes ne comportant pas ou peu de pièces mécaniques en mouvement.

  * premiers systèmes à électro-aimants & lampes

  * le transistortra

deux modes de fonctionnement (au moins...), deux types d'applications :
amplificateur de courant
commutateur commandable (mode utilisé en micro électronique)

une porte NON ET (NAND)

  * pourquoi électronique numérique plutot qu'analogique ?

On sait réaliser des calculateurs analogiques (à base d'AOP, cicuit RLC, ...) mais leur usage tend à se raréfier.

En effet, l'information est plus aisément stockable, transportable sans perturbation, mesurable, stable dans le temps et traitée quand elle est numérique, c'est à dire qu'on ne s'intéresse qu'à deux potentiels électriques (proche de gnd, soit 0, proche de Vcc, soi 1)

Il faut cependant convertir les données physiques (température, vitesse, pression...) en données numérique, et vice versa.

Chaîne d'acquisition...

  * autre utiliation des propiétés de l'électricité : les mémoires magnétiques


# L'architecture dite "Von Neumann" #
Le plan général d'un système informatisé est du à ce mathématicien d'origine hongroise réfugié au USA.
Il est considéré comme le père de l'ordinateur actuel, même s'il y a bien d'autres ingénieurs, chercheurs qui ont apporté leur contribution.

![http://www.cs.helsinki.fi/u/kerola/tkhist/k2003/alustukset/von_Neumann/Image6.gif](http://www.cs.helsinki.fi/u/kerola/tkhist/k2003/alustukset/von_Neumann/Image6.gif)


Alan Turing
s'est intéressé à a théorie de la peuve : qu'est ce qu'une machine peut faire ?

The image “http://irb.cs.tu-berlin.de/~zuse/Konrad_Zuse/cmp-neum.jpg” cannot be displayed, because it contains errors.

  * registres

  * bus de communication

  * unité arithmétique et logique (UAL)

logique combinatoire
demi additionneur
anticipateur de retenue

Le schéma de l'UAL 4 bits 74181.


  * la mémoire de travail


  * le séquenceur

micro commandes

registre d'instruction

compteur de programme


  * le cycle principal du séquenceur

  * fetch

  * exec

  * pc <- pc + 1




  * jeu d'instruction & microprogramme

Optimisations possibles pour µ-Processeur

Le nombre de transistor disponible par unité de surface permet certaines optimisations :

  * co-processeur mathématique

  * pipeline

  * mémoire cache

decodage d'adresse
plan mémoire



