---
rank: 2
layout: default
title: Objectif
permalink: /fr/objectif/
lang: fr
lang-ref: goal
---

# Quel est notre objectif ?

Pour cet exercice, nous allons réaliser une petite scène de jeu très simple, dans laquelle il s'agira de déplacer une petite bille sur un dallage régulier semblable à un échiquer *infini*. Les commandes de mouvement de notre bille seront appliquées à l'aide du `PAD` de la console. Je vous laisse découvrir dans cette petite vidéo le résultat que nous chercherons *grosso modo* à atteindre tout au long de ce tutoriel :

<iframe width="560" height="315" src="https://www.youtube.com/embed/OBBptplCc44" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope;" allowfullscreen></iframe>

L'objet de ce tutoriel consistera à détailler comment réaliser l'*éclairage* de la scène de jeu. Le dallage sera plongé dans le noir et partiellement éclairé à l'endroit où se trouve la bille (le centre de l'écran ici). L'intensité lumineuse étant inversement proportionnelle au carré de la distance quand on s'éloigne de la bille.


# Les éléments graphiques

Nous n'aurons besoin que de deux petits éléments graphiques pour réaliser le rendu de la scène de jeu. Je les ai dessinés avec l'éditeur en ligne [Piskel](https://www.piskelapp.com/), qui est un outil très bien fait et gratuit :

![assets](../../assets/figures/goal/assets.png){: width="226" height="126" }

Vous pouvez [télécharger](../../assets/downloads/shading-effect-assets.zip) ces éléments graphiques... vous n'aurez pas à les dessiner ! <i class="far fa-smile-wink"></i>


# Lecture préliminaire

La technique d'affichage que nous utiliserons tout au long de ce tutoriel n'est probablement pas celle que vous avez l'habitude d'appliquer... En effet, pour pouvoir gérer l'affichage en haute résolution (160x128 pixels), nous ne pourrons malheureusement pas nous appuyer sur l'API `gb.display` standard qui ne permet pas de gérer un affichage en RGB565 (codage sur 16 bits) avec seulement 32 Ko de RAM. Nous ne pourrons donc pas bénéficier des fonctions très utiles que vous avez l'habitude d'utiliser, comme `drawPixel()`, `drawCircle()`, `fillRect()` ou encore `drawImage()`. Nous devrons recoder entièrement l'affichage de nos éléments graphiques... bits à bits... pour pouvoir attaquer directement le contrôleur DMA (*Direct Memory Access*) en passant par l'API `gb.tft`. C'est la seule solution pour gérer un affichage en haute résolution en 65 536 couleurs (et avec des performances raisonnables). Notre très estimé [Andy](https://gamebuino.com/@aoneill) nous a livré un [article très intéressant](https://gamebuino.com/creations/high-resolution-without-gb-display) sur le sujet, dans lequel il détaille comment mettre en oeuvre cette technique de *bas niveau*. Nous aurons l'occasion d'y revenir dans le prochain chapitre, mais je vous encourage vivement à passer cet article à la loupe avant d'aller plus loin ! Il vous évitera d'aller déchiffrer le fichier [Display-ST7735.cpp](https://github.com/Gamebuino/Gamebuino-META/blob/master/src/utility/Display-ST7735/Display-ST7735.cpp) dans la bibliothèque [Gamebuino-Meta](https://github.com/Gamebuino/Gamebuino-META), qui met en oeuvre l'interface avec l'écran TFT Adafruit 1,8" de la META, et qui vous apparaîtra sûrement indigeste *a priori*. Mais la récompense est de taille ! Cette technique vous ouvre les portes de la haute résolution en full RGB565. Un grand merci Andy pour nous avoir donné les clefs du contrôleur DMA ! <i class="far fa-smile-wink"></i>