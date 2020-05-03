---
rank: 1
layout: default
title: Introduction
lang: fr
lang-ref: index
---

# Présentation du tuto

Lorsque Andy ([aoneill](https://gamebuino.com/@aoneill)) a publié sont article [High Resolution without gb.display](https://gamebuino.com/creations/high-resolution-without-gb-display), j'y ai vaguement jeté un oeil, et je me suis dit « Ouhla... ça à l'air coton ce truc... et pi c'est quoi cette histoire de DMA ? Et en plus dans la doc officielle y'a écrit que `gb.tft` fait partie de la **danger zone**... ok, bon ben on verra ça plus tard hein ! ». Moi qui suis parti de zéro en C++ y a tout juste 2 mois, ça m'a fait peur...

Et puis, après avoir fini mon premier tuto sur le [Jeu de la Vie](https://m1cr0lab-gamebuino.github.io/gb-game-of-life/), j'commençais enfin à y comprendre quelque-chose à ce langage de *barbare*, et je me suis dit « Allez ! Prochaine étape : comprendre le truc d'Andy... ça a quand même l'air vachement bien. Et pi c'mec là il a l'air de faire des trucs de ouf... »

Quelle bonne idée j'ai eu ce jour là... J'ai eu l'impression de découvrir le Saint Graal ! Alors, croyez-moi, si vous ne l'avez pas encore lu... foncez !

J'ai donc eu tout de suite envie d'écrire un petit projet pour voir si j'avais bien compris sa technique... et ça a donné ce que je vais vous développer dans ce tuto. Mais ne vous arrêtez pas à son titre réducteur. Je crois sincèrement qu'il développe de nombreux trucs utiles pour la programmation de jeux vidéos. Et il explique en détail la technique magique d'Andy. Donc, je pense qu'il pourrait vraiment être salvateur pour toutes les âmes qui s'y sont égarées.


# Quels sont les prérequis ?

|                  | Débutant |            Intermédiaire           |             Avancé           | Expert |
|-----------------:|:--------:|:----------------------------------:|:----------------------------:|:------:|
|**Niveau requis** |          | <i class="fas fa-check faded"></i> | <i class="fas fa-check"></i> |        |

Pour pouvoir suivre ce tutoriel dans les meilleures conditions, vous devez :

- avoir suivi avec succès tous les ateliers de l'[Académie](https://gamebuino.com/academy),
- maîtriser les principes généraux du langage C++,
- maîtriser la notion de pointeurs,
- pratiquer la Programmation Orientée Objet,
- avoir lu attentivement l'article [High Resolution without gb.display](https://gamebuino.com/creations/high-resolution-without-gb-display) d'[Andy](https://gamebuino.com/@aoneill).

Posséder une Gamebuino META est ici indispensable pour pouvoir tester votre code dans de bonnes conditions. Malheureusement, l'[émulateur en ligne d'Andy](http://games.aoneill.com/meta-emulator/) ne fonctionne pas correctement en haute résolution.


# Ce que vous allez apprendre ici

... pas mal de choses en fait !

- vous allez déjà mieux comprendre la technique proposée par Andy ;
- puis je vous proposerai une manière de développer un moteur de rendu qui met en oeuvre cette technique dédiée à la haute résolution ;
- vous allez apprendre à quoi sert le Design Pattern **Observateur** ;
- ainsi que la manière de l'appliquer dans le cadre d'un moteur de rendu ;
- puis vous apprendrez à programmer une liste chaînée, qui fait partie des collections dynamiques très utiles en algorithmique ;
- nous passerons ensuite à un chapitre un peu plus théorique, ou je présenterai les notions mathématiques de la cinématique du point matériel, ainsi que la relation fondamentale de la dynamique que l'on retrouve en mécanique newtonienne, pour enfin aborder le principe d'inertie en physique... tout cela pour vous donner les clefs de la programmation du **mouvement** dans vos jeux ;
- et une fois que tout cela sera en place, nous nous attaquerons enfin à la technique de **shading** : une méthode pour jouer avec la lumière et créer une ambiance tamisée dans vos scènes de jeu.

Voilà, en gros... ça fait pas mal de choses. J'espère que vous allez prendre autant de plaisir à découvrir toutes ces notions, que j'en ai eu moi-même a les rédiger <i class="far fa-smile"></i>.
