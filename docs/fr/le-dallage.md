---
rank: 7
layout: default
title: Le Dallage
permalink: /fr/le-dallage/
lang: fr
lang-ref: tiling
---

# Ajout du dallage sur la scène

Comme dans le cas de la bille, c'est au niveau du contrôleur principal `GameEngine` que l'on va introduire le dallage sur la scène de jeu :

<div class="filename">GameEngine.h</div>
```c++
#ifndef SHADING_EFFECT_GAME_ENGINE
#define SHADING_EFFECT_GAME_ENGINE

// nous allons définir la classe `Tiling` juste après...
#include "Tiling.h"
#include "Ball.h"

class GameEngine
{
    private:

        // un pointeur vers l'instance du dallage
        static Tiling* tiling;
        // un pointeur vers l'instance de la bille
        static Ball* ball;

    public:

        static void init();
        static void tick();
};

#endif
```

<div class="filename">GameEngine.cpp</div>
```c++
#include "GameEngine.h"
#include "Renderer.h"

// toujours initialiser un pointeur à NULL
Tiling* GameEngine::tiling = NULL;
Ball* GameEngine::ball = NULL;

void GameEngine::init() {
    // instantiation du dallage
    tiling = new Tiling();
    // instantiation de la bille
    ball = new Ball();

    // enregistrement des observateurs
    // au sein du moteur de rendu
    Renderer::subscribe(tiling);
    Renderer::subscribe(ball);
}

void GameEngine::tick() {
    Renderer::draw();
}
```

Vous noterez ici que l'enregistrement des observateurs auprès du moteur de rendu se fait selon l'ordre décroissant de la profondeur de l'élément sur la pile d'affichage. Ceci de manière à ce que le dallage soit dessiné en premier (le plus profond dans la pile) et la bille ensuite (rappelez-vous que l'ajout d'un élément dans la liste chaînée des observateurs se fait en queue de liste).


# Préparation des éléments graphiques

Vous trouverez le fichier `tiles.png` dans la petite archive ZIP que vous avez téléchargée tout à l'heure. Vous voyez que cette fois, il s'agit d'une petite *spritesheet* comportant deux sprites : une dalle claire et une dalle sombre.

![assets](../../assets/figures/goal/assets.png){: width="226" height="126" }

Voilà comment paramétrer l'[outil de transcodage](https://gamebuino.com/creations/image-transcoder-for-hd-gb-tft) :

![img2tft settings](../../assets/figures/tiling/img2tft-settings.png){: width="200" height="505" class="shadow" }

Faîtes glisser le fichier `tiles.png` sur l'encart de dépôt, et vous devriez obtenir ceci :

```c++
// you can declare your sprites like this:

struct Sprite {
    int x, y;
    uint8_t width, height;
    uint8_t frames;
    uint16_t* data;
    uint16_t transparent;
};

// and after spritedata has been set
// you can initialize your sprite like that:
//
// Sprite mySprite = {
//     0, // choose an initial x value
//     0, // choose an initial y value
//     16,
//     16,
//     2,
//     (uint16_t*) &spritedata,
//     0xffff
// };

const uint16_t spritedata[] = {
    0xffff, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0xffff,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0xffff, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0xffff,
    0xffff, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0xffff,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0xffff, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0xffff
};
```


# Définition du dallage

Commençons pas déclarer la classe `Tiling` :

<div class="filename">Tiling.h</div>
```c++
#ifndef SHADING_EFFECT_TILING
#define SHADING_EFFECT_TILING

#include "Renderable.h"

// la classe Tiling remplit le contrat défini par l'interface `Renderable`
class Tiling : public Renderable
{
    private:

        // les paramètres descriptifs du sprite
        static const uint8_t TILE_WIDTH;
        static const uint8_t TILE_HEIGHT;
        static const uint16_t TRANSPARENT_COLOR;
        // la carte des pixels que l'on a obtenue avec l'outil de transcodage
        static const uint16_t BITMAP[];
        
        // nous allons déplacer le dallage pour donner une impression de mouvement
        // à la bille, donc on définit les coordonnées du vecteur de déplacement
        // pour le prendre tout de suite en compte dans le calcul du rendu
        int8_t offsetX;
        int8_t offsetY;

    public:

        // on déclare un constructeur dans lequel
        // nous initialiserons le vecteur de déplacement
        Tiling();

        // un destructeur doit nécessairement être déclaré ici pour
        // éviter de potentielles fuites de mémoire
        ~Tiling();

        // la méthode imposée par le contrat `Renderable`
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) override;
};

#endif
```

Passons maintenant à la définition de notre classe `Tiling`. Je vais détailler et commenter chacune des étapes de calcul du rendu directement dans le code :

<div class="filename">Tiling.cpp</div>
```c++
#include <Gamebuino-Meta.h>
#include "Tiling.h"
#include "constants.h"

// on reporte les paramètres descriptifs de notre sprite
const uint8_t Tiling::TILE_WIDTH = 16;
const uint8_t Tiling::TILE_HEIGHT = 16;
const uint16_t Tiling::TRANSPARENT_COLOR = 0xffff;

// on reporte ici la valeur de la variable `spritedata`
// que nous a fourni l'outil de transcodage
const uint16_t Tiling::BITMAP[] = {
    // 1ère dalle
    0xffff, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0xffff,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0x79ce, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x55ad, 0x2842,
    0xffff, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0xffff,
    // 2ème dalle
    0xffff, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0x79ce, 0xffff,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0x79ce, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x518c, 0x2842,
    0xffff, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0x2842, 0xffff
};

// vient ensuite le constructeur, qui initialise le vecteur de déplacement
Tiling::Tiling() : offsetX(0), offsetY(0) {}

// le destructeur doit être défini pour éviter
// les potentielles fuites de mémoire
Tiling::~Tiling() {
    // il ne fait rien de particulier ici,
    // mais c'est important d'y penser !
}

// et on définit la méthode de calcul du rendu du dallage
void Tiling::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    // nous allons précalculer certains paramètres
    // pour optimiser le temps de traitement...

    // on va chercher à numéroter les dalles selon les deux axes X et Y
    // pour leur associer des indices qui pourraient être nommés `tx` et `ty`
    // et on va alors calculer un témoin de parité sur ces deux indices :
    //   txodd = true lorsque `tx` est impair et false sinon
    //   tyodd = true lorsque `ty` est impair et false sinon
    bool txodd,tyodd;

    // il suffira alors de tester conjointement la parité de `tx` et `ty`
    // pour savoir si on doit afficher une dalle claire ou une dalle sombre
    // autrement dit, si l'on doit effectuer un "saut" dans la spritesheet
    // pour atteindre la dalle sombre... c'est le rôle du témoin `jump`
    bool jump;

    // et voici le décalage à appliquer dans la spritesheet
    // pour accéder aux couleurs de la dalle sombre
    uint16_t nfo = TILE_WIDTH * TILE_HEIGHT;

    // tous les pixels (sx,sy) de la tranche courante vont être examinés...
    // il faudra alors passer du système de coordonnées locales à la tranche
    // (sx,sy) au système de coordonnées globales de l'écran (x,y)...
    // et sx est en fait équivalent à x puisque la tranche couvre la largeur
    // totale de l'écran :
    //   x = sx
    //   y = sy + sliceY
    uint8_t sy,x,y;

    // n'oublions pas que nous devons tenir compte du vecteur de déplacement
    // (offsetX, offsetY) qui sera appliqué au dallage pour donner une impression
    // de mouvement à la bille...
    // il nous faudra donc transposer notre système de coordonnées globales (x,y) :
    //   xo = x + offsetX
    //   yo = y + offsetY
    uint8_t xo,yo;

    // lorsque nous devrons écrire dans le tampon, nous aurons affaire à
    // un tableau unidimensionnel... il nous faudra donc projeter les
    // coordonnées globales (x,y) sur l'indice correspondant dans le tampon :
    //   buffer_index = x + (sy * SCREEN_WIDTH)
    //                       -------syw-------
    uint16_t syw;

    // lorsque nous devrons aller piocher la couleur `value` dans la spritesheet
    // qu'il faudra ensuite recopier dans le tampon, nous devrons calculer à
    // quel indice `index` aller chercher cette couleur dans le tableau BITMAP
    uint16_t index, value;

    // le calcul de cet indice peut se décomposer en deux parties :
    //   index = index_x + index_y, où :
    //   index_x = (xo % TILE_WIDTH) + (jump * nfo)
    //   index_y = (yo % TILE_HEIGHT) * TILE_WIDTH
    uint16_t index_y;

    // balayage de chaque pixel de la tranche (ici la composante Y)
    for (sy = 0; sy < sliceHeight; sy++) {

        // passage du système bidimensionnel de la tranche
        // au système unidimensionnel du tampon
        syw = sy * SCREEN_WIDTH;

        // passage du système de coordonnées locales à la tranche
        // au système de coordonnées globales de l'écran
        y = sliceY + sy;

        // on applique la composante Y du vecteur de déplacement
        yo = y + this->offsetY;

        // on calcule le témoin de parité de la dalle selon l'axe Y
        tyodd = (yo / TILE_HEIGHT) % 2;

        // puis la composante Y de l'indice de lecture dans la spritesheet
        index_y = (yo % TILE_HEIGHT) * TILE_WIDTH;

        // balayage de chaque pixel de la tranche (ici la composante X)
        for (x = 0; x < SCREEN_WIDTH; x++) {

            // on applique la composante X du vecteur de déplacement
            xo = x + this->offsetX;

            // on calcule le témoin de parité de la dalle selon l'axe X
            txodd = (xo / TILE_WIDTH) % 2;

            // puis on détermine si on doit piocher le code couleur
            // dans une dalle claire ou une dalle sombre
            jump = txodd ^ tyodd;

            // l'indice de lecture dans la spritesheet peut maintenant
            // être entièrement déterminé
            index = index_y + (xo % TILE_WIDTH) + (jump * nfo);

            // il ne reste plus qu'à piocher le code couleur du pixel
            value = BITMAP[index];

            // et à le recopier dans le tampon s'il ne s'agit pas d'un
            // pixel transparent, sinon on fixe la couleur noire
            buffer[x + syw] = value != TRANSPARENT_COLOR ? value : 0;
        }
    }
}
```

Voilà, j'espère avoir été suffisamment clair sur le détail de toutes les étapes de calculs nécessaires à effectuer le rendu du dallage... C'était pas de la tarte !


# Nouveau rendu de la scène

Y a plus qu'à admirer le résultat... on compile... on déverse le code machine sur la META... et voilà ce que ça donne désormais :

![repeated ball](../../assets/figures/tiling/tiling-rendering.png){: width="320" height="256" class="shadow" }

Bingo ! On a enfin une scène de jeu qui ressemble à quelque-chose. Autant vous avouer tout de suite qu'on a fait le plus dur !... Si, si, je vous l'assure !... le reste c'est de la crème à côté <i class="far fa-smile-wink"></i> ! Et pourtant on n'a pas encore attaqué véritablement le sujet ciblé de ce tutoriel, à savoir le *shading*... Pourtant tous ces éléments étaient nécessaires. Il fallait mettre tout ça en place pour que les choses soient plus simples à digérer ensuite. Vous allez voir que ça n'est pas compliqué finalement...

Mais gardons encore un peu de suspense avant de tout dévoiler ! Nous allons d'abord ajouter un peu d'interactivité et de mouvement dans tout ça. Allez ! Direction le prochain chapitre...