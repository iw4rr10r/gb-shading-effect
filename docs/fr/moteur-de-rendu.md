---
rank: 4
layout: default
title: Moteur de Rendu
permalink: /fr/moteur-de-rendu/
lang: fr
lang-ref: renderer
---

# Organisation de l'architecture du jeu

Après tout ce blabla, nous pouvons enfin passer à l'implémentation <i class="far fa-smile-wink"></i>. Nous allons commencer par mettre en place le moteur de rendu de notre application. Mais juste auparavant, organisons un peu les choses.

Définissons les constantes globales qui nous seront utiles par la suite :

<div class="filename">constants.h</div>
```c++
#ifndef SHADING_EFFECT_CONSTANTS
#define SHADING_EFFECT_CONSTANTS

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

#endif
```

Puis définissons le *croquis* de notre application `ShadingEffect.ino` :

<div class="filename">ShadingEffect.ino</div>
```c++
#include <Gamebuino-Meta.h>
#include "GameEngine.h"

void setup() {
    gb.begin();

    // nous n'utiliserons pas le tampon graphique standard
    // défini par `gb.display`, donc initialisons-le avec
    // une taille nulle pour qu'il n'occupe pas inutilement
    // de la place en mémoire :
    gb.display.init(0, 0, ColorMode::rgb565);

    // initialisation du contrôleur principal
    GameEngine::init();
}

void loop() {
    while(!gb.update());

    // exécution de la boucle de contrôle principale
    GameEngine::tick();
}
```

Vous pouvez noter que nous allons confier le contrôle global de l'application au composant `GameEngine` :

<div class="filename">GameEngine.h</div>
```c++
#ifndef SHADING_EFFECT_GAME_ENGINE
#define SHADING_EFFECT_GAME_ENGINE

class GameEngine
{
    public:

        static void init();
        static void tick();
};

#endif
```

Que nous allons définir de la façon suivante à ce stade :

<div class="filename">GameEngine.cpp</div>
```c++
#include "GameEngine.h"

// le moteur de rendu
#include "Renderer.h"

void GameEngine::init() {
    // rien de spécial à faire pour le moment
}

void GameEngine::tick() {
    // exécution du rendu de la scène de jeu
    Renderer::draw();
}
```

Nous voyons ici que la boucle principale est simplement chargée d'exécuter le rendu de la scène de jeu, qui est confié au moteur de rendu `Renderer` :


# Définition du moteur de rendu

Nous allons ici appliquer strictement la méthode d'Andy décrite au chapitre précédent pour définir notre moteur de rendu :

<div class="filename">Renderer.h</div>
```c++
#ifndef SHADING_EFFECT_RENDERER
#define SHADING_EFFECT_RENDERER

#include <Gamebuino-Meta.h>
#include "constants.h"

// définition de la hauteur des tranches
#define SLICE_HEIGHT 8

class Renderer
{
    private:
    
        // déclaration des deux tampons mémoires dans lesquels
        // seront réalisés les calculs de rendu
        static uint16_t buffer1[SCREEN_WIDTH * SLICE_HEIGHT];
        static uint16_t buffer2[SCREEN_WIDTH * SLICE_HEIGHT];
        // témoin qui nous indiquera si un transfert de mémoire
        // vers le contôleur DMA est en cours ou pas
        static bool drawPending;

        // méthode chargée d'initier le transfert mémoire vers le contrôleur DMA
        static void customDrawBuffer(int16_t x, int16_t y, uint16_t* buffer, uint16_t w, uint16_t h);
        // méthode chargée d'attendre que le transfert soit terminé
        // et de clôturer la transaction avec le contrôleur DMA
        static void waitForPreviousDraw();
    
    public:

        // exécution du rendu de la scène de jeu
        static void draw();
};

#endif
```

Et voici la définition des méthodes magiques :

<div class="filename">Renderer.cpp</div>
```c++
#include "Renderer.h"

// définition des deux tampons mémoire
uint16_t Renderer::buffer1[SCREEN_WIDTH * SLICE_HEIGHT];
uint16_t Renderer::buffer2[SCREEN_WIDTH * SLICE_HEIGHT];

// pour le moment, aucun transfert de mémoire n'est en cours
bool Renderer::drawPending = false;

// la routine magique relative au contrôleur DMA...
// jetez un coup d'oeil dans la bibliothèque officielle
// si cela attise votre curiosité :
// Gamebuino-META/src/utility/Display-ST7735/Display-ST7735.cpp
namespace Gamebuino_Meta {
  #define DMA_DESC_COUNT (3)
  extern volatile uint32_t dma_desc_free_count;

  static inline void wait_for_transfers_done(void) {
    while (dma_desc_free_count < DMA_DESC_COUNT);
  }

  static SPISettings tftSPISettings = SPISettings(24000000, MSBFIRST, SPI_MODE0);
};

// méthode chargée d'initier le transfert mémoire vers le contrôleur DMA...
// ces opérations sont également définies dans :
// Gamebuino-META/src/utility/Display-ST7735/Display-ST7735.cpp
void Renderer::customDrawBuffer(int16_t x, int16_t y, uint16_t* buffer, uint16_t w, uint16_t h) {
  drawPending = true;
  gb.tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
  SPI.beginTransaction(Gamebuino_Meta::tftSPISettings);
  gb.tft.dataMode();
  gb.tft.sendBuffer(buffer, w*h);
}

// méthode chargée d'attendre que le transfert soit terminé
// et de clôturer la transaction avec le contrôleur DMA...
// idem, tout est dans :
// Gamebuino-META/src/utility/Display-ST7735/Display-ST7735.cpp
void Renderer::waitForPreviousDraw() {
  if (drawPending) {
    // la petite routine magique définie plus haut
    Gamebuino_Meta::wait_for_transfers_done();
    gb.tft.idleMode();
    SPI.endTransaction();
    drawPending = false;
  }
}

// exécution du rendu de la scène de jeu
void Renderer::draw() {
  // on calcule le nombre de tranches horizontales à découper
  uint8_t slices = SCREEN_HEIGHT / SLICE_HEIGHT;
  // puis on parcourt chaque tranche une à une
  for (uint8_t sliceIndex = 0; sliceIndex < slices; sliceIndex++) {

    // on définit un pointeur qui alternera entre les deux tampons mémoire
    uint16_t* buffer = sliceIndex % 2 == 0 ? buffer1 : buffer2;
    // on calcule l'ordonnée de la première frange horizontale de la tranche
    uint8_t sliceY = sliceIndex * SLICE_HEIGHT;

    // ---------------------------------------------------------------------
    // ici nous effectuerons les calculs de rendu de nos éléments graphiques
    // à partir des 3 paramètres suivants :
    // - sliceY       : ordonnée de la tranche courante (frange supérieure)
    // - SLICE_HEIGHT : hauteur des tranches
    // - buffer       : pointeur vers le tampon courant
    // ---------------------------------------------------------------------
  
    // puis on s'assure que l'envoi du tampon précédent
    // vers le contrôleur DMA a bien eu lieu
    if (sliceIndex != 0) waitForPreviousDraw();
    // après quoi on peut alors envoyer le tampon courant
    customDrawBuffer(0, sliceY, buffer, SCREEN_WIDTH, SLICE_HEIGHT);
  }

  // toujours attendre que le transfert DMA soit bien terminé
  // pour la dernière tranche avant de sortir de la méthode !
  waitForPreviousDraw();
}
```

Il nous reste maintenant à définir précisément le calcul du rendu de chaque élément graphique présent sur la scène de jeu. Nous ne les avons pas encore définis, mais nous allons le faire bientôt. Toutefois, auparavant, pour introduire un peu de généricité au niveau de notre moteur de rendu, nous allons définir un mécanisme bien pratique : le *patron* **Observateur**. La suite dans le prochaine chapitre...