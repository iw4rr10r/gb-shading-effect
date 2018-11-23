---
rank: 6
layout: default
title: La Bille
permalink: /fr/la-bille/
lang: fr
lang-ref: ball
---

# Ajout de la bille sur la scène

Maintenant que la mécanique est bien huilée, nous pouvons enfin ajouter des objets sur notre scène de jeu et admirer tout le travail de préparation que nous venons d'accomplir !

Commençons donc par notre petite bille. Rappelez-vous que le jeu est gouverné par un contrôleur principal : `GameEngine`. C'est donc lui qui va se charger de placer la bille sur la scène de jeu :

<div class="filename">GameEngine.h</div>
```c++
#ifndef SHADING_EFFECT_GAME_ENGINE
#define SHADING_EFFECT_GAME_ENGINE

// nous allons définir la classe `Ball` juste après...
#include "Ball.h"

class GameEngine
{
    private:

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
Ball* GameEngine::ball = NULL;

void GameEngine::init() {
    // instantiation de la bille
    ball = new Ball();

    // enregistrement de la bille comme observateur
    // au sein du moteur de rendu
    Renderer::subscribe(ball);
}

void GameEngine::tick() {
    // effectue le rendu de la scène
    Renderer::draw();
}
```

Vous voyez que c'est tout simple ! Y'a rien de plus à faire en-dehors de la définition de la bille elle-même...


# Préparation des éléments graphiques

Si vous ne l'avez pas déjà fait, [téléchargez](../../assets/downloads/shading-effect-assets.zip) l'archive ZIP contenant les éléments graphiques du tutoriel et jetez un oeil sur le fichier `ball.png` :

![assets](../../assets/figures/goal/assets.png){: width="226" height="126" }

Voilà venu le moment d'intégrer cette image à notre code C++. Comment allons-nous procéder ? Rappelez-vous que nous devrons écrire les codes couleurs de tous les pixels qui composent cette bille dans le tampon `buffer` du moteur de rendu. Attention, le référentiel colorimétrique du fichier `ball.png` est le `RGB888`... or il faudra convertir ces couleurs en `RGB565`. Et de surcroît, rappelez-vous également que nous devrons faire en sorte que ces codes couleurs soient écrits selon l'ordre **big-endian** avant d'être envoyés au contrôleur DMA. Ouhlala... ça va pas être simple tout ça ! Et ben si <i class="far fa-smile-wink"></i> ! J'ai publié un petit outil, il y a 2 semaines, qui permet justement de faire tout ça pour nous :

![img2tft](../../assets/figures/ball/img2tft.gif){: width="160" height="128" class="shadow" }

<span style="display:block;text-align:center;" markdown="1">[Image Transcoder for HD & gb.tft](https://gamebuino.com/creations/image-transcoder-for-hd-gb-tft)</span>

Il vous suffit de paramétrer l'outil comme suit, et de glisser-déposer le fichier `ball.png` dans l'encart **Drag and drop your image here** (sur la page de l'outil bien sûr... pas ici !) :

![img2tft settings](../../assets/figures/ball/img2tft-settings.png){: width="200" height="505" class="shadow" }

Vous obtiendrez alors le code suivant :

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
//     1,
//     (uint16_t*) &spritedata,
//     0xffff
// };

const uint16_t spritedata[] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x5cfe, 0x0ef8, 0x0780, 0xffff, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0780, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0780, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0450, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0xffff, 0x0780, 0x0780, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0780, 0x0450, 0x0450, 0x0000, 0x0000, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0x0450, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0x0450, 0x0450, 0x0000, 0x0000, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0450, 0x0450, 0x0450, 0x0450, 0x0450, 0x0450, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
};
```

La partie qui nous intéresse ici est la définition de la constante `spritedata`, que nous allons reporter dans notre classe `Ball`. J'ai choisi une couleur de transparence arbitraire qui ne fait pas partie des couleurs que j'ai utilisées pour dessiner la bille. J'ai choisi le blanc (`#ffff`), mais vous pouvez tout à fait choisir celle que vous voulez... en dehors de celles utilisées pour dessiner la bille !

Arrêtons-nous juste quelques instants sur ces codes couleurs. Prenons par exemple, le rose *flashy* de notre bille. Son code couleur est `#FF0071` en RGB888 et `#F80E` en RGB565. Vous pouvez remarquer que ce code couleur est remplacé par l'entier `0x0ef8` dans la définition de `spritedata`... ce qui signifie qu'il a été ré-écrit dans l'ordre **little-endian**...

> <div style="text-align:center;"><i class="far fa-flushed fa-2x"></i></div>
>
> Heeeeuuuuuu...???...???... oui mais tu nous as dit qu'on devait l'écrire en **big-endian** parce-que l'écran TFT il bouffe du **big-endian** !!!???...

Ha ha ha ... moi aussi ça m'a déconcerté au départ !... Mais [Soru](http://gamebuino.com/@sorunome) m'a donné l'explication : dans la mesure où le processeur mange du **little-endian**, le compilateur C++ effectue justement la conversion des constantes que vous définissez dans votre code (qui sont écrites en big-endian qui est l'ordre naturel dans lequel nous écrivons les choses) pour les transformer en little-endian... donc si vous définissez une constante avec une écriture little-endian... et bien elle sera renversée et donc convertie en **big-endian**. Et c'est exactement ce qu'on veut puisque c'est ça que le processeur va balancer au contrôleur DMA. Voilà pourquoi les codes couleurs obtenus sont bien écrits en little-endian. Fallait y penser hein !

Allez, on peut continuer...


# Définition de la bille

Commençons par déclarer la class `Ball` :

<div class="filename">Ball.h</div>
```c++
#ifndef SHADING_EFFECT_BALL
#define SHADING_EFFECT_BALL

#include "Renderable.h"

// voilà comment déclarer le fait que la classe `Ball` remplit le contrat
// défini par l'interface `Renderable` (qui est réalité une classe)
class Ball : public Renderable
{
    private:

        // les paramètres descriptifs du sprite
        static const uint8_t FRAME_WIDTH;
        static const uint8_t FRAME_HEIGHT;
        static const uint16_t TRANSPARENT_COLOR;
        // la carte des pixels que l'on a obtenue avec l'outil de transcodage
        static const uint16_t BITMAP[];
        
        // les coordonnées de la bille, qui sont constantes
        // puisque la bille est fixée au centre de l'écran
        static const uint8_t X_POS;
        static const uint8_t Y_POS;

    public:

        // la fameuse méthode qui permet de remplir le contrat `Renderable`
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) override;
};

#endif
```

La déclaration qui consiste à dire que la classe `Ball` remplit le contrat défini par l'interface `Renderable` se fait par le biais de l'héritage en C++. Rappelez-vous que la notion d'interface au sens strict du terme n'existe pas ici. Et c'est en utilisant l'héritage multiple du C++ que vous pouvez faire en sorte que vos classes implémentent plusieurs interfaces, en dérivant de plusieurs classes qui définissent différents contrats.

Bien, passons maintenant à la définition de notre classe `Ball` :

<div class="filename">Ball.cpp</div>
```c++
#include <Gamebuino-Meta.h>
#include "Ball.h"
#include "constants.h"

// on reporte les paramètres descriptifs de notre sprite
const uint8_t Ball::FRAME_WIDTH = 16;
const uint8_t Ball::FRAME_HEIGHT = 16;
const uint16_t Ball::TRANSPARENT_COLOR = 0xffff;
// on positionne la bille au centre de l'écran...
// notez que ces coordonnées correspondent au coin
// en haut à gauche de notre sprite
const uint8_t Ball::X_POS = (SCREEN_WIDTH - FRAME_WIDTH) / 2;
const uint8_t Ball::Y_POS = (SCREEN_HEIGHT - FRAME_HEIGHT) / 2;

// on reporte ici la valeur de la variable `spritedata`
// que nous a fourni l'outil de transcodage
const uint16_t Ball::BITMAP[] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x5cfe, 0x0ef8, 0x0780, 0xffff, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x5cfe, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x5cfe, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0780, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0x0780, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0450, 0x0450, 0x0000, 0xffff,
    0xffff, 0xffff, 0xffff, 0x0780, 0x0780, 0x0ef8, 0x0ef8, 0x0ef8, 0x0ef8, 0x0780, 0x0780, 0x0450, 0x0450, 0x0000, 0x0000, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0x0450, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0x0450, 0x0450, 0x0000, 0x0000, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0450, 0x0450, 0x0450, 0x0450, 0x0450, 0x0450, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
};

// et on définit la méthode de calcul du rendu de la bille
void Ball::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    // on détermine les bornes du sprite selon l'axe Y qui sont
    // situées à l'intérieur de la tranche courante
    int8_t startIndex = Y_POS <= sliceY ? 0 : Y_POS - sliceY;
    int8_t endIndex = Y_POS + FRAME_HEIGHT >= sliceY + sliceHeight ? sliceHeight - 1 : Y_POS + FRAME_HEIGHT - sliceY - 1;
    // on prépare une variable qui recevra tour à tour les codes couleurs
    // de chacun des pixels qui composent le sprite (dans la tranche courante)
    uint16_t value;
    // coordonnées du pixel traité
    uint8_t x,y;
    // et on balaie le sprite entre les bornes Y définies plus haut
    for (y = startIndex; y <= endIndex; y++) {
        // ainsi qu'entre les bornes selon l'axe X
        for (x = X_POS; x < X_POS + FRAME_WIDTH; x++) {
            // on va piocher la couleur du pixel correspondant
            value = BITMAP[(sliceY - Y_POS + y) * FRAME_WIDTH - X_POS + x];
            // et s'il ne s'agit pas de la couleur de transparence
            if (value != TRANSPARENT_COLOR) {
                // on la recopie dans le tampon
                buffer[x + y * SCREEN_WIDTH] = value;
            }
        }
    }
}
```


# Premier rendu de la scène

Et ben c'est nickel ! Y a plus qu'à compiler et déverser tout ça sur la console pour enfin voir quelque-chose apparaître à l'écran...

![repeated ball](../../assets/figures/ball/ball-rendering.png){: width="320" height="256" class="shadow" }

<div class="centered" markdown="1">
> <i class="far fa-flushed fa-2x"></i>
>
> C'est quoi ce bordel ???!!!...
</div>

Ha ha ha... c'est à peu près la tronche que j'ai tirée à la première exécution...

Mais l'explication est simple... une idée ?

En fait, regardez bien le code de la méthode de rendu `draw()` :
- de la bille
- et du moteur de rendu

À quel endroit voyez-vous que l'on écrit quelque-chose dans le buffer ? Nous le faisons exclusivement dans la méthode de rendu de la bille... et uniquement lorsqu'on est en présence de pixels dont la couleur n'est pas la couleur de transparence...

Bah oui ! Vous ne voyez toujours pas ? Regardons d'un peu plus près ce qui se passe réellement...

La surface de rendu est découpée en `128 / 8 = 16` tranches :

| tranche |   buffer  |            écriture           |    état résultant du buffer   |
|:-------:|:---------:|:-----------------------------:|:-----------------------------:|
| n°1     | `buffer1` |        on n'écrit rien        |         que des zéros         |
| n°2     | `buffer2` |        on n'écrit rien        |         que des zéros         |
| n°3     | `buffer1` |        on n'écrit rien        |         que des zéros         |
| n°4     | `buffer2` |        on n'écrit rien        |         que des zéros         |
| n°5     | `buffer1` |        on n'écrit rien        |         que des zéros         |
| n°6     | `buffer2` |        on n'écrit rien        |         que des zéros         |
| n°7     | `buffer1` |        on n'écrit rien        |         que des zéros         |
| n°8     | `buffer2` | moitié supérieure de la bille | moitié supérieure de la bille |
| n°9     | `buffer1` | moitié inférieure de la bille | moitié inférieure de la bille |
| n°10    | `buffer2` |        on n'écrit rien        | moitié supérieure de la bille |
| n°11    | `buffer1` |        on n'écrit rien        | moitié inférieure de la bille |
| n°12    | `buffer2` |        on n'écrit rien        | moitié supérieure de la bille |
| n°13    | `buffer1` |        on n'écrit rien        | moitié inférieure de la bille |
| n°14    | `buffer2` |        on n'écrit rien        | moitié supérieure de la bille |
| n°15    | `buffer1` |        on n'écrit rien        | moitié inférieure de la bille |
| n°16    | `buffer2` |        on n'écrit rien        | moitié supérieure de la bille |

<div class="centered"><strong>Capito?</strong></div>

Bah voilà ce qui se passe... et au deuxième passage, le buffer n'est plus vide, donc les moitiés de bille sont dessinées sur toutes les tranches... alternativement... indéfiniment... Et quand on repasse sur les tranches où la bille doit effectivement être dessinée, ben on réécrit exactement la même chose dans les buffers... Donc rien ne change à l'affichage...

Le premier réflexe est de se dire : « *oh ben y a qu'à écrire des zéros au lieu de rien du tout* » ... moé ... si la bille était seule sur la scène de jeu, c'est effectivement ce que l'on pourrait faire... mais le hic, c'est qu'elle n'est pas toute seule ! Et en faisant cela, vous effaceriez tous les pixels des objets qui auraient été dessinés auparavant.

Par contre, la chance qu'on a dans cette histoire, c'est qu'il nous reste à effectuer le rendu du dallage... qui, lui, recouvre TOUTE la surface de l'écran... donc le problème sera réglé. En effet, le dallage se situe à une profondeur supérieure sur la pile d'affichage, il sera donc dessiné avant la bille... Par conséquent, il recouvrira toute la surface noire, et les tranches seront toujours remplies avec ses pixels... que l'on dessine la bille ou pas d'ailleurs.

Allez, rendez-vous au prochain chapitre pour attaquer le rendu du dallage !