---
rank: 8
layout: default
title: Dynamique du Mouvement
permalink: /fr/dynamique-du-mouvement/
lang: fr
lang-ref: motion
---

# Gestion des événements utilisateur

Nous devons intercepter les événements lorsque l'utilisateur appuiera sur les boutons de la console de manière à impulser un mouvement à la bille. Et dans la mesure où la bille reste au centre de l'écran comme si elle était suivie par une caméra, l'illusion du mouvement sera donnée par le déplacement relatif de son environnement, c'est-à-dire le dallage. Rappelez-vous que nous avons déjà prévu dans le calcul de rendu du dallage un vecteur de déplacement `(offsetX, offsetY)`. Et bien, c'est justement ce vecteur que nous allons nous appliquer à calculer maintenant pour gérer l'impression de mouvement.

Commençons par mettre en place les éléments nécessaires à l'interception des événements utilisateurs. Pour simplifier les choses, nous allons le faire directement dans le moteur du jeu `GameEngine` :

<div class="filename">GameEngine.cpp</div>
```c++
void GameEngine::tick() {

    // interception des événements utilisateur

    if (gb.buttons.repeat(BUTTON_LEFT, 1)) {
        tiling->left();
    }

    if (gb.buttons.repeat(BUTTON_RIGHT, 1)) {
        tiling->right();
    }

    if (gb.buttons.repeat(BUTTON_UP, 1)) {
        tiling->up();
    }

    if (gb.buttons.repeat(BUTTON_DOWN, 1)) {
        tiling->down();
    }

    // on délègue ensuite le calcul du mouvement au dallage
    // nous allons donc lui ajouter une boucle de contrôle
    // pour réaliser ces calculs
    tiling->tick();

    // et on effectue ensuite le rendu de la scène de jeu
    Renderer::draw();
}
```


# Cinématique du point matériel

La gestion du mouvement est relativement simple. Nous allons nous appuyer sur la *cinématique du point matériel*. Je vous renvoie à vos cours de physique pour les détails théoriques... mais, en substance, nous retiendrons simplement ici les principes suivants :

> - Si l'on considère un référentiel spatial ayant pour origine le point $$O$$ de coordonnées $$(0,0)$$, et par-rapport auquel on étudie le mouvement d'un point matériel `M`. La position du point $$M$$ à l'instant $$t$$ est donnée par le **vecteur position** $$\overrightarrow{OM}$$ (ou $$\vec{r}$$) de coordonnées $$(x,y)$$.
> ![assets](../../assets/figures/motion/position.png){: width="200" height="160" }
> - La vitesse moyenne entre deux positions successives $$M$$ et $$M'$$ du point matériel se définit comme le rapport entre la distance $$MM'$$ parcourue et la durée $$\Delta{t} = t'-t$$ entre ces deux instants. Il s'agit d'une grandeur scalaire. En considérant des instants de plus en plus rapprochés, et donc en passant à la limite $$\Delta{t} \to 0$$, on peut définir une grandeur vectorielle instantanée appelée **vecteur vitesse** et définie par la dérivée temporelle du vecteur position :
>
> $$\vec{v} = \lim\limits_{\Delta{t} \to 0} \frac{\overrightarrow{MM'}}{\Delta{t}} = \frac{d\vec{r}}{dt}$$
>
> - De la même manière on peut définir le **vecteur accélération** défini par la dérivée temporelle du vecteur vitesse :
>
> $$\vec{a} = \frac{d\vec{v}}{dt} = \frac{d^2\vec{r}}{dt^2}$$

Ces équations sont très utiles... mais pour qu'il y ait mouvement, il faut qu'on introduise également la notion de dynamique qui s'appuie sur les *forces* mécaniques...


# Principe fondamental de la dynamique

Ce principe désigne une loi physique mettant en relation la masse d'un objet et l'accélération qu'il reçoit si des forces lui sont appliquées. On l'appelle aussi la *deuxième loi de Newton* ou *relation fondamentale de la dynamique*, et elle s'énonce ainsi :

> Soit un corps de masse $$m$$ constante non nulle, l'accélération subie par ce corps dans un référentiel galiléen est proportionnelle à la résultante des forces qu'il subit, et inversement proportionnelle à sa masse :
>
> $$\vec{a} = \frac{1}{m} \sum_{i} \vec{F_i}$$

Pas trop mal à la tête ?... <i class="far fa-smile"></i> ... mais nan ... c'est pas si méchant, allez. Vous allez voir que tout ça va se traduire de manière très simple dans notre code.


# Gestion du mouvement

Nous avons désormais toutes les clefs mathématiques et physiques pour mettre en oeuvre le mouvement de notre bille, transposé sur le déplacement du dallage pour simuler le suivi d'une caméra.

Pour ne pas faire intervenir de calculs trigonométriques, qui seraient plus coûteux pour le processeur, qui est déjà bien occupé avec le calcul du rendu de la scène, nous allons décider de représenter nos vecteurs position, vitesse et accélération par leurs coordonnées respectives (c'est-à-dire en les projetant sur les axes X et Y), et nous allons simplifier (drastiquement) la relation fondamentale de la dynamique. Pour cela, on va agglomérer la masse de la bille ainsi que la somme des forces qui lui sont appliquées sous la forme d'une simple impulsion motrice constante `PULSE=1` que l'on affectera telle quelle aux coordonnées `ax` et `ay` du vecteur accélération. Vous voyez qu'on ne va pas faire dans la dentelle... mais ça simplifiera énormément les calculs et soulagera le processeur.

Injectons maintenant toutes ces idées dans le code de la classe `Tiling` :

<div class="filename">Tiling.h</div>
```c++
#ifndef SHADING_EFFECT_TILING
#define SHADING_EFFECT_TILING

#include "Renderable.h"

// l'impulsion motrice constante
#define PULSE 1

class Tiling : public Renderable
{
    private:

        static const uint8_t TILE_WIDTH;
        static const uint8_t TILE_HEIGHT;
        static const uint16_t TRANSPARENT_COLOR;
        static const uint8_t BITMAP[];

        // les coordonnées du vecteur accélération
        float ax,ay;

        // les coordonnées du vecteur vitess
        float vx,vy;

        // les coordonnées du vecteur position ne sont autres
        // que celles de notre vecteur de déplacement
        int8_t offsetX,offsetY;

    public:

        // le constructeur devra désormais initialiser tous les vecteurs
        Tiling();
        ~Tiling();

        // les commandes de déplacement invoquées par `GameEngine`
        void left();
        void right();
        void up();
        void down();

        // le point de raccordement de la boucle de contrôle
        void tick();

        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) override;
};

#endif
```

Voyons maintenant comment définir les nouveaux éléments que nous avons introduits :

<div class="filename">Tiling.cpp</div>
```c++
Tiling::Tiling() {
    this->ax = 0;
    this->ay = 0;
    this->vx = 0;
    this->vy = 0;
    this->offsetX = 0;
    this->offsetY = 0;
}

// la relation fondamentale de la dynamique s'applique
// ici très simplement :-)

void Tiling::left() {
    this->ax = -PULSE;
}

void Tiling::right() {
    this->ax = PULSE;
}

void Tiling::up() {
    this->ay = -PULSE;
}

void Tiling::down() {
    this->ay = PULSE;
}

// le raccordement à la boucle de contrôle
// est l'occasion d'appliquer nos équations
// de la dynamique du mouvement

void Tiling::tick() {
    // le vecteur vitesse est directement dérivé
    // du vecteur accélération
    this->vx += this->ax;
    this->vy += this->ay;

    // le vecteur de déplacement est directement dérivé
    // du vecteur vitesse
    this->offsetX += this->vx;
    this->offsetY += this->vy;

    // ! attention ici !
    // n'oubliez pas que l'accélération est une grandeur INSTANTANÉE
    // donc une fois qu'elle a été appliquée au vecteur vitesse,
    // vous devez impérativement la remettre à zéro !
    this->ax = 0;
    this->ay = 0;
}
```

Ok ! Allez-y ! Compilez... déversez le code machine sur la console pour l'exécuter... et testez !... Que remarquez vous ? Le mouvement de la bille ne vous paraît pas bizarre ? Testez bien... par exemple donnez de petites impulsions toujours dans la même direction... vous voyez que la vitesse de la bille augmente et ne fait qu'augmenter, sauf si vous lui appliquez une force contraire en lui donnant une impulsion dans le sens opposé, ce qui aura pour effet de la freiner... jusqu'à inverser son mouvement. Et ensuite il ne cessera d'augmenter à nouveau dans cette direction.

Que se passe-t-il ? Et bien tout se déroule normalement ! Si, si <i class="far fa-smile"></i> !... Mais je ne vous ai pas encore parlé d'un autre principe fondamental de la physique Newtonienne : l'**inertie**.


# Principe d'inertie

> En physique, l'inertie d'un corps, dans un référentiel galiléen (dit inertiel), est sa tendance à conserver sa vitesse : en l'absence d'influence extérieure, tout corps ponctuel perdure dans un mouvement rectiligne uniforme. L'inertie est aussi appelée *principe d'inertie*, ou *loi d'inertie*, et, depuis Newton, *première loi de Newton*.
>
> La loi d'inertie exprime le fait que si la vitesse $$\vec{v}$$ du corps ponctuel par rapport au repère galiléen est constante, « la somme des forces $$\vec{F_i}$$ s'exerçant sur le corps est nulle » :
>
> $$\vec{v} = cte \iff \sum_{i} \vec{F_i} = \vec{0}$$

Vous saisissez maintenant ? Voilà la raison pour laquelle la vitesse de la bille ne ralentit jamais (sauf si on lui applique une force opposée à la direction de son mouvement). C'est tout simplement le principe d'inertie qui est à l'oeuvre dans notre modélisation. Aussi, pour ajouter plus de réalisme à notre scène de jeu, nous pourrions considérer que le dallage sur lequel se déplace la bille induit des frictions (des forces de frottement) sur la bille. En physique, le frottement (ou friction) est une interaction qui s'oppose au mouvement relatif entre deux systèmes en contact. Autrement dit, le dallage (qui est fixe) interagit avec la bille de manière à s'opposer à son mouvement. Ce qui veut tout simplement dire que cette interaction peut être grossièrement modélisée par un affaiblissement du vecteur vitesse de la bille. Si on considère maintenant que la bille est fixe (puisqu'elle est suivie par la caméra), c'est donc le dallage qui se déplace par-rapport à elle. Par conséquent, les frictions entraînent un affaiblissement de la vitesse du dallage par-rapport à la bille.

Comment pouvons nous affaiblir simplement cette vitesse ? Et bien il suffit de la multiplier (à chaque cyle de la boucle de contrôle) par un facteur plus petit que `1`. Et oui... tout simplement <i class="far fa-smile"></i>. Et plus ce facteur sera proche de zéro, plus le ralentissement sera brutal. Nous déciderons (arbitrairement) ici d'affecter la valeur `0.9` à notre facteur de friction.

Intégrons maintenant ce nouveau phénomène dans nos équations :

<div class="filename">Tiling.cpp</div>
```c++
void Tiling::tick() {
    this->vx += this->ax;
    this->vy += this->ay;
    this->offsetX += this->vx;
    this->offsetY += this->vy;
    this->ax = 0;
    this->ay = 0;

    // application du phénomène de friction :
    this->vx *= .9;
    this->vy *= .9;

    // et nous pouvons également décider que lorsque les composantes
    // de la vitesse deviennent inférieures à 0.5 (en valeurs absolues),
    // alors on peut considérer qu'elles s'annullent tout simplement
    if (this->vx > -.5 && this->vx < .5) { this->vx = 0; }
    if (this->vy > -.5 && this->vy < .5) { this->vy = 0; }
}
```

Et voilà... vous pouvez maintenant compiler et tester l'application... vous voyez que c'est quand même vachement mieux ! On obtient désormais un mouvement beaucoup plus réaliste, n'est-ce pas ?

Même si nous avons abordé quelques principes théoriques physiques et mathématiques qui vous permettent maintenant de comprendre POURQUOI et COMMENT mettre en oeuvre un mouvement réaliste dans vos applications, vous voyez que l'application de ces principes dans le code n'est pas si compliquée.

Allez, on va ENFIN pouvoir passer à la technique tant attendue qui consiste à appliquer des effets de *shading* sur vos scènes de jeu. Direction le prochain chapitre...