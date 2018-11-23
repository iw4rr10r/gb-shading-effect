---
rank: 3
layout: default
title: Méthode d'affichage
permalink: /fr/methode-d-affichage/
lang: fr
lang-ref: display
---

# La méthode d'Andy

La technique décrite par Andy dans son article [High Resolution without gb.display](https://gamebuino.com/creations/high-resolution-without-gb-display) est relativement simple mais terriblement astucieuse. L'idée générale consiste à effectuer le rendu graphique dans une zone tampon en mémoire, avant de la déverser vers le bus d'interfaçage de l'écran TFT. Néanmmoins, il part du constat que la Gamebuino META ne dispose pas de suffisamment de mémoire (sa RAM étant limitée à 32 Ko) pour pouvoir contenir un tampon de la taille de l'écran en haute résolution avec une profondeur de couleurs de 16 bits (la mémoire nécessaire s'élève à 40 Ko). C'est la raison pour laquelle on ne peut pas utiliser `gb.display`.

Il propose donc de découper la surface de rendu à calculer en tranches suffisamment petites, de sorte qu'elles puissent être contenues en RAM à tour de rôle, sous la forme d'un petit tampon. Le calcul du rendu est ainsi effectué sur ce tampon, et une fois que le calcul est terminé, le tampon est alors recopié vers l'écran pour être affiché. Puis on passe à la tranche suivante, et ainsi de suite... Si le cycle de rendu partiel (calcul + recopie vers l'écran) de chaque tranche est suffisamment rapide, la scène complète du jeu peut alors être affichée sans que l'on puisse distinguer chacune des étapes successives.

L'idée géniale d'Andy consiste à utiliser le contrôleur DMA pour effectuer les transferts entre la mémoire et le périphérique d'affichage. L'acronyme `DMA` signifie *Direct Memory Access*, ce qui veut dire un accès direct à la mémoire. Il est en effet possible d'opérer un transfert **direct** de données via un contrôleur spécialisé entre la mémoire principale de la console et le périphérique d'affichage. Ce procédé ne fait intervenir le processeur que pour initier et conclure le transfert à travers des interruptions. Par conséquent le transfert DMA offre un moyen plus rapide pour l'échange de blocs de données entre la mémoire et l'écran. Cerise sur le gâteau : pendant que le transfert s'effectue, le processeur est au repos. On peut donc le solliciter pour effectuer d'autres tâches en parralèle ! Et c'est là que toute la magie de cette technique opère...

Il s'agit en effet de gérer **2** tampons en parallèle : pendant que le processeur s'active à calculer le rendu sur le premier tampon, le contrôleur DMA se charge de transférer le second vers l'écran <i class="far fa-smile-wink"></i>. Et c'est précisément en utilisant `gb.tft` que nous allons pouvoir réaliser ces opérations dans notre code C++.


# Mise en oeuvre

Vous avez toute la liberté d'organiser le découpage de la surface complète de rendu, en délimitant les tranches comme bon vous semble. Il vous suffit de déterminer l'agencement le plus approprié au rendu que vous devez réaliser. Les méthodes les plus simples (mais pas nécessairement les plus performantes selon le cas) consistent à découper la surface de rendu en tranches régulières horizontales ou verticales.

Tout au long de ce tutoriel, nous nous appuierons sur un découpage en tranches horizontales. Chaque tranche couvrira la largeur totale de l'écran, sur une hauteur de 8 pixels. Nous pouvons donc calculer la taille nécessaire à allouer en mémoire pour accueillir un tampon correspondant à chacune de ces tranches :

> Taille d'un tampon : (160 x 8 pixels) x (16 bits) = 20 480 bits = 2 560 octets = 2,5 Ko

Comme nous devons gérer deux tampons en parallèle, il nous suffira donc de réserver 2 x 2,5 = 5 Ko de RAM pour pouvoir réaliser le rendu complet de notre scène de jeu avec 65 536 couleurs... C'est génial, nan ?

La mise en oeuvre est donc assez simple : il suffira de parcourir l'ensemble des tranches une à une, et pour chaque nouvelle tranche, on alternera l'écriture des bits sur le tampon 1 ou le tampon 2.

Que devrons-nous écrire sur le tampon ? Et bien, pour chaque point `(x,y)` de la tranche en cours, nous devrons déterminer la couleur du pixel qui sera affiché à cette position. Nous reporterons donc sur le tampon la valeur de l'entier, codé sur 16 bits, correspondant à cette couleur.

Pour déterminer la couleur du pixel à afficher, il suffira de parcourir l'ensemble des éléments graphiques de la scène de jeu et de déterminer quels sont ceux qui ont une intersection non vide avec la tranche courante. Il ne restera plus qu'à projeter les coordonnées `(x,y)` dans le référentiel de chaque élément graphique pour y piocher le code de la couleur à recopier dans le tampon. Les éléments graphiques seront parcourus dans l'ordre de la pile d'affichage (du plus éloigné au plus proche). On pourrait même essayer d'optimiser les choses car un pixel de profondeur `n` masque nécessairement tous les pixels dont la profondeur est supérieure à `n`. Il est donc inutile d'explorer tous les pixels des éléments graphiques ayant une profondeur supérieure à `n`.

Voici une petite animation illustrant le processus de rendu pour une scène de jeu très simple où seule la bille est présente :

![slice buffering](../../assets/figures/display/slice-buffering.gif){: width="320" height="256" class="shadow" }

> **Remarque importante**
>
> L'ordre dans lequel les octets sont organisés en mémoire diffère selon qu'ils sont destinés au CPU ou à l'écran TFT ! Cet ordre est désigné par le terme **endianness**.
>
> Quand le CPU de la META enregistre un entier sur 16 bits en mémoire, par exemple `0xA3B7` en notation hexadécimale, il l'enregistre dans des octets dans l'ordre inversé `B7 A3` pour une structure de mémoire fondée sur une unité atomique de 1 octet et un incrément d'adresse de 1 octet. Ainsi l'octet de poids le plus faible (ici `B7`) est enregistré à l'adresse mémoire la plus petite (le poids le plus faible en premier). L'octet de poids supérieur (ici `A3`) est enregistré à l'adresse mémoire suivante, et ainsi de suite. Les architectures qui respectent cette règle sont dites **little-endian**.
>
> Dans le cas de l'écran TFT, c'est exactement l'inverse : l'octet de poids le plus fort (`A3`) est enregistré à l'adresse mémoire la plus petite. L'octet de poids inférieur (`B7`) est enregistré à l'adresse mémoire suivante, et ainsi de suite. Les architectures qui respectent cette règle sont dites **big-endian**.
>
> Il faudra donc penser à inverser les octets deux à deux lorsque l'on écrira dans le tampon qui est destiné à être envoyé à l'écran (par le biais du contrôleur DMA), sans quoi les couleurs affichées risquent de ne pas correspondre à ce que vous attendiez !