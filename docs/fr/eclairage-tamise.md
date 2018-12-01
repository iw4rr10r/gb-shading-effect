---
rank: 9
layout: default
title: Éclairage Tamisé
permalink: /fr/eclairage-tamise/
lang: fr
lang-ref: shading
---

# Et la lumière fut

Nous voilà enfin dans le vif du sujet de ce tutoriel. Notre objectif, au travers de ce chapitre sera de plonger la scène de jeu dans la pénombre et d'éclairer la bille par un halo de lumière pour ne distinguer que son voisinage proche. Cet effet d'éclairage peut donner une ambiance très sympa à vos jeux. Ce qui est remarquable ici, c'est que nous disposons de toute la palette de couleurs offerte par le référentiel `RGB565`. Il est clair que si nous ne disposions que des 16 couleurs proposées par le mode d'affichage `DISPLAY_MODE_INDEX` prévu par défaut lorsque l'on veut développer une application en haute résolution avec la META, nous serions très frustrés, car un tel effet n'aurait certainement pas le même rendu. Il pourrait même être impossible à mettre en oeuvre si vos sprites mobilisent déjà ne serait-ce que quelques couleurs...

La problématique qui s'impose à nous ici est de trouver le moyen de créer de l'ombre en partant d'une scène pleinement éclairée comme celle que nous avons développée jusqu'ici. La question est donc « comment *faire* de l'ombre avec de simples pixels » ? Et bien il suffit de les « assombrir » <i class="far fa-smile"></i> ! C'est-à-dire de jouer sur le niveau de luminosité de la couleur du pixel.

Le modèle colorimétrique que nous avons à notre disposition est le modèle `RGB565`. Autrement dit, les couleurs sont fabriquées à partir de trois couleurs primaires : le rouge (sur 5 bits), le vert (sur 6 bits) et le bleu (sur 5 bits), dont on peut fixer les niveaux d'intensité indépendamment les uns des autres. Si je vous donne par exemple la couleur `ORANGE` définie dans la [palette officielle](https://gamebuino.com/creations/color-palettes) de la bibliothèque et que je vous demande de l'assombrir en diminuant sa luminosité de `50%`... Comment faire ? Regardons d'un peu plus près le codage de cette couleur dans les deux référentiels `RGB888` et `RGB565` :

| Couleur        |  R8 |  G8 | B8  | RGB888    | RGB565   | R5 | G6 | B5 |
|:---------------|----:|----:|----:|:---------:|:--------:|---:|---:|---:|
| ORANGE         | 255 | 168 |  17 | `#FFA811` | `0xFD42` | 31 | 42 |  2 |

Diminuer la luminosité de cette couleur de `50%` revient simplement à diviser par `2` chacun des niveaux d'intensité de ses 3 couleurs primaires (on arrondira à l'entier le plus proche lorsque la division n'est pas entière) :

| Couleur        |  R8 |  G8 | B8  | RGB888    | RGB565   | R5 | G6 | B5 |
|:---------------|----:|----:|----:|:---------:|:--------:|---:|---:|---:|
| ORANGE         | 255 | 168 |  17 | `#FFA811` | `0xFD42` | 31 | 42 |  2 |
| ORANGE sombre  | 128 |  84 |   9 | `#805409` | `0x82A1` | 16 | 21 |  1 |

On obtient ainsi 2 niveaux de luminosité régulièrement répartis entre la teinte de référence et la couleur noire (où la luminosité est nulle).

<div style="margin:0 auto;">
    <table>
        <tbody>
            <tr style="box-shadow:0 2px 4px #aaa;">
                <td style="background-color:#ffa811;width:32px;height:32px;"></td>
                <td style="background-color:#805409;width:32px;height:32px;"></td>
                <td style="background-color:#000000;width:32px;height:32px;"></td>
            </tr>
        </tbody>
    </table>
</div>

En effet, si l'on soustrait à la teinte intermédiaire l'équivalent de 50% de la luminosité de la teinte de référence, on retombe bien sur le noir.

De manière générale, pour obtenir $$n$$ niveaux de luminosité $$\{R_i,G_i,B_i\}$$ pour $$i \in [1,n]$$ d'une teinte de référence $$\{R,G,B\}$$, on peut définir les relations suivantes :

$$
\forall i \in [1,n]
\begin{cases}
    R_i = \frac{n-i+1}{n} R \\
    G_i = \frac{n-i+1}{n} G \\
    B_i = \frac{n-i+1}{n} B
\end{cases}
$$

Vous avez compris le principe ? Ok, donc si l'on souhaite maintenant générer `8` niveaux de luminosité, il suffit d'appliquer ces jolies petites formules pour obtenir rapidement la palette suivante (en incluant le noir comme 9<sup>ème</sup> couleur) :

<div style="margin:0 auto;">
    <table>
        <tbody>
            <tr style="box-shadow:0 2px 4px #aaa;">
                <td style="background-color:#ffa811;width:32px;height:32px;"></td>
                <td style="background-color:#DF930F;width:32px;height:32px;"></td>
                <td style="background-color:#BF7E0D;width:32px;height:32px;"></td>
                <td style="background-color:#9F690B;width:32px;height:32px;"></td>
                <td style="background-color:#805409;width:32px;height:32px;"></td>
                <td style="background-color:#603F06;width:32px;height:32px;"></td>
                <td style="background-color:#402A04;width:32px;height:32px;"></td>
                <td style="background-color:#201502;width:32px;height:32px;"></td>
                <td style="background-color:#000000;width:32px;height:32px;"></td>
            </tr>
            <tr>
                <td colspan="9" style="text-align:center;border:none;">
                    Je vous laisse faire le calcul <i class="far fa-smile"></i>
                </td>
            </tr>
        </tbody>
    </table>
</div>

Vous voyez que c'est relativement simple, n'est-ce pas ? Bien... Comment allons-nous appliquer ce principe à notre scène de jeu, de manière à créer ce halo de lumière autour de notre petite bille ?

Et bien, il suffira simplement d'aller piocher le code couleur du pixel dans la spritesheet du dallage, exactement comme nous l'avons fait jusqu'ici, et en fonction de la distance qui sépare ce pixel du centre de la bille, c'est-à-dire du centre de l'écran, nous lui affecterons un niveau de luminosité. C'est aussi simple que ça.


# Préparatifs pour la mesure de performances

Mais auparavant, nous allons ajouter quelques lignes dans le croquis de notre application de manière à mesurer la charge du CPU et l'espace mémoire disponible, pour observer l'impact du calcul de luminosité sur les performances à l'exécution.

<div class="filename">ShadingEffect.ino</div>
```c++
#include <Gamebuino-Meta.h>
#include "GameEngine.h"

void setup() {
    gb.begin();

    // initialisation du port série
    SerialUSB.begin(9600);

    gb.display.init(0, 0, ColorMode::rgb565);
    GameEngine::init();
}

void loop() {
    while(!gb.update());

    // mesure de la charge du CPU à chaque seconde (on est à 25 fps par défaut)
    // et envoi des données vers le port série
    if (gb.frameCount % 25 == 0) {
        SerialUSB.printf("CPU: %i, RAM: %i\n", gb.getCpuLoad(), gb.getFreeRam());
    }

    GameEngine::tick();
}
```

Connectez votre META sur un port USB de votre ordinateur, puis ouvrez le port série dans **Arduino IDE** (notez que l'identifiant du port sera probablement différent chez vous) :

![serial port](../../assets/figures/shading/serial-port.jpg){: class="shadow" }

Puis lancez l'exécution pour observer comment se déroulent les choses tant que nous n'avons rien fait concernant le calcul de luminosité... Vous devriez obtenir quelque-chose dans ce goût là :

```
[Starting] Opening the serial port - /dev/cu.usbmodem141401
[Info] Opened the serial port - /dev/cu.usbmodem141401
CPU: 82, RAM: 9755
CPU: 82, RAM: 9755
CPU: 82, RAM: 9755
```

Le CPU est donc déjà à `82%` de charge ! Forcément... on dessine énormément de pixels... et pour chaque pixel, on effectue pas mal de calculs finalement. Pour mémoire, voici le code de notre méthode `draw()` débarassé de tous ses commentaires :

<div class="filename">Tiling.cpp</div>
```c++
void Tiling::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    bool txodd,tyodd,jump;
    uint8_t sy,x,y, xo,yo;
    uint16_t syw,index,value,index_y;
    uint16_t nfo = TILE_WIDTH * TILE_HEIGHT;
    for (sy = 0; sy < sliceHeight; sy++) {
        syw = sy * SCREEN_WIDTH;
        y = sliceY + sy;
        yo = y + this->offsetY;
        tyodd = (yo / TILE_HEIGHT) % 2;
        index_y = (yo % TILE_HEIGHT) * TILE_WIDTH;
        for (x = 0; x < SCREEN_WIDTH; x++) {
            xo = x + this->offsetX;
            txodd = (xo / TILE_WIDTH) % 2;
            jump = txodd ^ tyodd;
            index = index_y + (xo % TILE_WIDTH) + (jump * nfo);
            value = BITMAP[index];
            buffer[x + syw] = value != TRANSPARENT_COLOR ? value : 0;
        }
    }
}
```


# Première mise en oeuvre

Commençons par déclarer deux constantes dans `Tiling.h` :

<div class="filename">Tiling.h</div>
```c++
class Tiling
{
    private:

        // le carré du rayon du halo
        static const uint16_t HALO_RADIUS2;

        // le nombre de niveaux de luminosité
        static const uint8_t BRIGHTNESS_LEVELS;
    
    // le reste des déclarations reste inchangé
};
```

Vous voyez que nous allons caractériser le halo de lumière par **le carré** de son rayon. En effet, dans la mesure où nous allons devoir calculer des distances, pour éviter de surcharger inutilement le CPU nous travaillerons avec le carré de ces distances, de manière à éviter d'avoir à utiliser la fonction `sqrt()` qui permet de calculer la racine carrée d'un nombre, mais qui ferait s'effondrer les performances. Nous préférerons travailler directement sur les carrés.

Définissons maintenant ces deux constantes :

<div class="filename">Tiling.cpp</div>
```c++
const uint16_t Tiling::HALO_RADIUS2 = 0b1<<11;
const uint8_t Tiling::BRIGHTNESS_LEVELS = 4;
```

Vous remarquerez, que pour chacune d'entre elles, nous avons choisi une valeur qui est une puissance de 2 : `0b1<<11` = $$2^{11}$$ = `2048`, autrement dit, le rayon du halo s'étendra sur $$\sqrt{2048}$$ = environ `45` pixels, et nous souhaitons afficher ici `4` = $$2^2$$ niveaux de luminosité.

Voyons maintenant comment modifier notre méthode `draw()` pour qu'elle prenne en compte ces niveaux de luminosité dans son calcul de rendu :

<div class="filename">Tiling.cpp</div>
```c++
void Tiling::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    bool txodd,tyodd,jump;
    uint16_t nfo = TILE_WIDTH * TILE_HEIGHT;
    uint8_t sy,x,y;
    uint8_t xo,yo;
    uint16_t syw;
    uint16_t index, value;
    uint16_t index_y;

    // nous aurons besoin de mesurer le carré de la distance `r2`
    // entre le pixel du dallage et le centre de l'écran...
    // un calcul intermédiaire permettra de déterminer
    // la composante Y de `r2` que nous nommerons `ry2`
    uint16_t r2,ry2;
    uint8_t hsw = SCREEN_WIDTH / 2;
    uint8_t hsh = SCREEN_HEIGHT / 2;

    // pour effectuer les calculs de luminosité, nous aurons
    // besoin d'isoler les composantes rouge, verte et bleue
    // du code couleur RGB565 du pixel à traiter
    uint16_t red, green, blue;

    // nous calculerons alors le niveau de luminosité du pixel
    // en fonction du rapport entre le carré de la distance `r2`
    // qui le sépare du centre de l'écran et le carré du rayon
    // du halo de lumière `HALO_RADIUS2`
    uint16_t lux;

    // cette variable nous permettra d'effectuer les calculs
    // intermédiaires sur la couleur à transformer pour qu'elle
    // intègre les données de luminosité
    uint16_t color;

    for (sy = 0; sy < sliceHeight; sy++) {
        syw = sy * SCREEN_WIDTH;
        y = sliceY + sy;
        yo = y + this->offsetY;
        tyodd = (yo / TILE_HEIGHT) % 2;
        index_y = (yo % TILE_HEIGHT) * TILE_WIDTH;

        // on calcule la composante Y du carré de la distance
        // qui sépare le pixel du centre de l'écran
        ry2 = (y - hsh) * (y - hsh);

        for (x = 0; x < SCREEN_WIDTH; x++) {
            xo = x + this->offsetX;
            txodd = (xo / TILE_WIDTH) % 2;
            jump = txodd ^ tyodd;
            index = index_y + (xo % TILE_WIDTH) + (jump * nfo);

            // on calcule le carré de la distance qui sépare
            // le pixel du centre de l'écran
            r2 = (x - hsw) * (x - hsw) + ry2;

            // on plonge la dalle dans le noir par défaut
            value = 0;

            // et lorsque l'on est à l'intérieur du halo
            if (r2 <= HALO_RADIUS2) {
                // on récupère le code couleur du sprite de la dalle
                color = BITMAP[index];

                // s'il ne s'agit pas de la couleur de transparence
                if (color != TRANSPARENT_COLOR) {
                    // on calcule le niveau de luminosité à cette distance
                    lux = BRIGHTNESS_LEVELS - BRIGHTNESS_LEVELS * r2 / HALO_RADIUS2;

                    // on réécrit le code couleur en big-endian
                    color = ((color & 0xff) << 8) | (color >> 8);

                    // puis on récupère les niveaux d'intensité
                    // des 3 couleurs primaires
                    red   = color >> 11;
                    green = (color >> 5) & 0b111111;
                    blue  = color & 0b11111;

                    // on applique le niveau de luminosité
                    // à chacune des couleurs primaires
                    red   = red   * lux / BRIGHTNESS_LEVELS;
                    green = green * lux / BRIGHTNESS_LEVELS;
                    blue  = blue  * lux / BRIGHTNESS_LEVELS;

                    // puis on recompose le code couleur RGB565
                    value = (red << 11) | (green << 5) | blue;

                    // et on le réécrit en little-endian
                    value = ((value & 0xff) << 8) | (value >> 8);
                }
            }

            // il ne reste plus qu'à recopier
            // le code couleur dans le tampon
            buffer[x + syw] = value;
        }
    }
}
```

Allez-y, vous pouvez compiler et tester ! Vous devriez obtenir un truc dans ce goût là :

![shading with 4 levels](../../assets/figures/shading/shading-4-levels.png){: width="320" height="256" class="shadow" }

C'est sûr qu'avec seulement 4 niveaux de luminosité, c'est assez moche... mais ça permet de bien visualiser le découpage quadratique des couronnes d'ombre : la luminosité y est inversement proportionnelle aux carrés de leurs rayons. Voyons un peu ce que donnent les mesures de performances :

```
[Starting] Opening the serial port - /dev/cu.usbmodem141401
[Info] Opened the serial port - /dev/cu.usbmodem141401
CPU: 98, RAM: 9755
CPU: 98, RAM: 9755
CPU: 98, RAM: 9755
```

Ouch ! Vous voyez que ça a un coût ! Le processeur est déjà presque à donf !... Allez, même pas peur... passons à 32 niveaux de luminosité pour plus de finesse dans le rendu :

<div class="filename">Tiling.cpp</div>
```c++
const uint8_t Tiling::BRIGHTNESS_LEVELS = 0b1<<5; // 2^5 = 32
```

<div class="centered">Recompilez et testez !</div>

![shading with 32 levels](../../assets/figures/shading/shading-32-levels.jpg){: width="320" height="256" class="shadow" }

<div class="centered"><i class="far fa-2x fa-surprise"></i><br /><em>Woaaaaahhhh...</em></div>

Vous remarquerez que les performances restent les mêmes. Merci les puissances de `2` ! Tiens d'ailleurs, essayez de fixer une valeur qui n'est pas une puissance de `2` à la constante `BRIGHTNESS_LEVELS`, par exemple la valeur `25`... vous allez voir l'impact sur les performances ! Ne vous fiez pas trop à ce qui est envoyé au port série... cet affichage n'a lieu qu'en dehors de la méthode `draw()`... donc si la charge du CPU est trop importante, les données seront envoyées dès qu'il sortira de la méthode `draw()` et qu'il pourra souffler un peu. Du coup les valeurs affichées ne rendent pas vraiment compte de la charge réelle... Par contre, essayez de déplacer votre bille... vous vous rendrez très vite compte du réel impact sur les performances. Vous voyez donc que travailler avec des puissances de `2` peut largement optimiser un calcul. Et c'est tout à fait logique, puisque dans ce cas, les opérations arithmétiques peuvent souvent être simplifiées par de simples décalages de bits. Mettez-ça dans une de vos cases mémoires <i class="far fa-smile-wink"></i>.

On pourrait encore optimiser les choses. Par exemple, dans la mesure où le carré du rayon du halo de lumière est une puissance de `2` :

<div class="filename">Tiling.cpp</div>
```c++
// on pourrait remplacer le test suivant
if (r2 <= HALO_RADIUS2) ...

// par celui-ci
if (!(r2 >> 11)) ...
```

On pourrait également réécrire les codes couleurs en **big-endian** dans la constante `BITMAP` : ça nous éviterait 1 renversement (mais pas l'autre !)... par contre, on ne gagnerait presque rien sur cette opération (qui est très rapide). Et c'est de toutes façons inutile à ce stade, puisque je vais maintenant vous proposer une autre manière d'optimiser largement les choses...


# Une nouvelle idée ?

Ce qui plombe les performances ici est bien entendu le calcul sur la luminosité, qui intervient sur chaque pixel. Comment pourrions-nous alléger la charge du processeur ? Et bien tout simplement en ayant effectué les calculs au préalable, à sa place. Mais comment ? Et bien c'est tout simple : il suffit de lui fournir une spritesheet qui intègre tous les codes couleurs précalculés avec tous les niveaux de luminosité que l'on souhaite afficher :

![precalculated spritesheet](../../assets/figures/shading/precalculated-spritesheet.png){: width="256" height="64" class="shadow" }

Il suffira alors d'aller piocher le pixel dans la dalle approprié. Cette solution n'est pas mal du tout... mais elle a trois inconvénients majeurs ! S'il vous prenait l'envie de modifier le nombre de niveaux de luminosité, il vous faudrait alors entièrement redessiner la spritesheet, ce qui peut s'avérer laborieux à la longue... De même que si vous souhaitez afficher de nombreux niveaux de luminosité, la conception de la spritesheet peut là aussi s'avérer laborieuse... Et de surcroît, plus vous aurez de dalles à y intégrer, plus elles occuperont de l'espace en mémoire ! Vous voyez que ça n'est pas la meilleure solution tout compte fait.


# La bonne solution

Mais il existe une autre solution <i class="far fa-smile"></i>. En effet, il suffit d'indexer les couleurs utilisées sur la spritesheet `tile.png` telle que je vous l'ai donnée (il y en 4). Puis de reconstruire la constante `BITMAP` en utilisant ces index plutôt que les codes couleurs `RGB565`.

![sprite colors](../../assets/figures/shading/sprite-colors.png){: width="229" height="128" }

Il ne restera plus ensuite qu'à construire une palette de couleurs `COLORMAP` présentant toutes les déclinaisons des couleurs de référence que nous avons indexées, en fonction des niveaux de luminosité souhaités... et à écrire les codes couleurs en **little-endian**. Bon... ça fait quand même un peu de boulot... Mais j'ai pensé à vous : j'ai publié, en même temps que ce tuto, un petit outil qui va faire tous les calculs pour vous et vous fournir *in fine* les constantes `BITMAP` et `COLORMAP` correspondantes.

![img4shading](../../assets/figures/shading/img4shading.gif){: width="160" height="128" class="shadow" }

<span style="display:block;text-align:center;" markdown="1">[Image Transcoder for Shading in High Res](https://gamebuino.com/creations/image-transcoder-for-shading-in-high-res)</span>

Sur la page de l'outil, saisissez le paramétrage suivant, puis faîtes glisser l'image `tiles.png` sur l'encart **Drag and drop your image here** :

![img4shading-settings](../../assets/figures/shading/img4shading-settings.png){: width="280" height="576" class="shadow" }

<div class="centered">Vous devriez obtenir le code suivant :</div>

```c++
// reminder for decoding:
//
// number of frames: 2
// frame width: 16
// frame height: 16
// transparent color: 0xffff
// brightness levels: 32

const uint8_t BITMAP[] = {
    // frame #1
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 
    // frame #2
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0
};

const uint16_t COLORMAP[] = {
    // color #0 (transparent color)
    0xffff, 
    // color #1
    0x79ce, 0x38c6, 0xf7bd, 0xd7bd, 0x96b5, 0x75ad, 0x34a5, 0xf39c, 0xd39c, 0x9294, 0x718c, 0x3084, 0x1084, 0xcf7b, 0x8e73, 0x6d6b, 0x2c63, 0x0c63, 0xcb5a, 0x8a52, 0x694a, 0x2842, 0x0842, 0xc739, 0x8631, 0x6529, 0x2421, 0x0421, 0xc318, 0x8210, 0x6108, 0x2000, 
    // color #2
    0x55ad, 0x34a5, 0xf39c, 0xd39c, 0xb294, 0x718c, 0x518c, 0x3084, 0x1084, 0xcf7b, 0xae73, 0x8e73, 0x4d6b, 0x2c63, 0x0c63, 0xcb5a, 0xaa52, 0x8a52, 0x494a, 0x2842, 0x0842, 0xc739, 0xa631, 0x8631, 0x4529, 0x2421, 0x0421, 0xc318, 0xa210, 0x8210, 0x4108, 0x2000, 
    // color #3
    0x2842, 0x0842, 0x0842, 0xe739, 0xe739, 0xc739, 0xa631, 0xa631, 0x8631, 0x8631, 0x6529, 0x6529, 0x4529, 0x4529, 0x2421, 0x2421, 0x0421, 0x0421, 0xe318, 0xe318, 0xc318, 0xa210, 0xa210, 0x8210, 0x8210, 0x6108, 0x6108, 0x4108, 0x4108, 0x2000, 0x2000, 0x0000, 
    // color #4
    0x518c, 0x3084, 0x1084, 0xcf7b, 0xae73, 0x8e73, 0x6d6b, 0x4d6b, 0x2c63, 0x0c63, 0xeb5a, 0xcb5a, 0xaa52, 0x8a52, 0x694a, 0x494a, 0x2842, 0x0842, 0xe739, 0xa631, 0x8631, 0x6529, 0x4529, 0x2421, 0x0421, 0xe318, 0xc318, 0xa210, 0x8210, 0x6108, 0x4108, 0x2000
};
```

Les données générées permettent d'économiser beaucoup d'espace mémoire par-rapport à la solution précédente (qui consistait à redessiner tous les sprites avec différents niveaux de luminosité). Mais voyons tout de suite de quelle manière...
- mettre en oeuvre ce nouveau codage de la spritesheet,
- et optimiser encore davantage notre méthode de calcul de rendu.

Reprenons la déclaration intégrale de notre classe `Tiling` pour mieux voir les changements que nous allons y apporter :

<div class="filename">Tiling.h</div>
```c++
#ifndef SHADING_EFFECT_TILING
#define SHADING_EFFECT_TILING

#include "Renderable.h"

// l'accélération transférée à chaque impulsion motrice
#define PULSE 1

// nous avons vu que les calculs de distance
// pouvaient être effectués rapidement en utilisant
// le carré du rayon du halo de lumière simplement
// exprimé par une puissance de 2
#define HALO_RADIUS2_POWER_OF_TWO 11

// et nous pouvons faire de même avec
// les niveaux de luminosité
#define BRIGHTNESS_LEVELS_POWER_OF_TWO 5

class Tiling : public Renderable
{
    private:

        static const uint8_t TILE_WIDTH;
        static const uint8_t TILE_HEIGHT;

        // notez le changement de type des valeurs
        // désormais stockées dans la spritesheet
        static const uint8_t BITMAP[];

        // la palette de couleurs associée à la spritesheet
        // incluant toutes les déclinaisons lumineuses des
        // couleurs de référence de nos sprites
        static const uint16_t COLORMAP[];

        // les propriétés cinématiques
        float ax,ay;
        float vx,vy;
        int8_t offsetX,offsetY;

    public:

        Tiling();
        ~Tiling();

        // les commandes de mouvement
        void left();
        void right();
        void up();
        void down();

        // le point de raccordement de la boucle de contrôle
        void tick();

        // la méthode de calcul du rendu
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) override;
};

#endif
```

Voyons maintenant les définitions des variables statiques de notre classe `Tiling` ainsi que la nouvelle définition de la méthode `draw()` :

<div class="filename">Tiling.cpp</div>
```c++
// les paramètres descriptifs de nos sprites (les dalles)
const uint8_t Tiling::TILE_WIDTH = 16;
const uint8_t Tiling::TILE_HEIGHT = 16;

// la nouvelle spritesheet obtenue avec l'outil de transcodage
const uint8_t Tiling::BITMAP[] = {
    // la dalle claire
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 
    // la dalle sombre
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0
};

// puis la palette de couleurs que nous a fournie l'outil de transcodage
// avec leurs 32 niveaux de luminosité
const uint16_t Tiling::COLORMAP[] = {
    // couleur #0 (couleur de transparence)
    0xffff, 
    // couleur #1
    0x79ce, 0x38c6, 0xf7bd, 0xd7bd, 0x96b5, 0x75ad, 0x34a5, 0xf39c, 0xd39c, 0x9294, 0x718c, 0x3084, 0x1084, 0xcf7b, 0x8e73, 0x6d6b, 0x2c63, 0x0c63, 0xcb5a, 0x8a52, 0x694a, 0x2842, 0x0842, 0xc739, 0x8631, 0x6529, 0x2421, 0x0421, 0xc318, 0x8210, 0x6108, 0x2000, 
    // couleur #2
    0x55ad, 0x34a5, 0xf39c, 0xd39c, 0xb294, 0x718c, 0x518c, 0x3084, 0x1084, 0xcf7b, 0xae73, 0x8e73, 0x4d6b, 0x2c63, 0x0c63, 0xcb5a, 0xaa52, 0x8a52, 0x494a, 0x2842, 0x0842, 0xc739, 0xa631, 0x8631, 0x4529, 0x2421, 0x0421, 0xc318, 0xa210, 0x8210, 0x4108, 0x2000, 
    // couleur #3
    0x2842, 0x0842, 0x0842, 0xe739, 0xe739, 0xc739, 0xa631, 0xa631, 0x8631, 0x8631, 0x6529, 0x6529, 0x4529, 0x4529, 0x2421, 0x2421, 0x0421, 0x0421, 0xe318, 0xe318, 0xc318, 0xa210, 0xa210, 0x8210, 0x8210, 0x6108, 0x6108, 0x4108, 0x4108, 0x2000, 0x2000, 0x0000, 
    // couleur #4
    0x518c, 0x3084, 0x1084, 0xcf7b, 0xae73, 0x8e73, 0x6d6b, 0x4d6b, 0x2c63, 0x0c63, 0xeb5a, 0xcb5a, 0xaa52, 0x8a52, 0x694a, 0x494a, 0x2842, 0x0842, 0xe739, 0xa631, 0x8631, 0x6529, 0x4529, 0x2421, 0x0421, 0xe318, 0xc318, 0xa210, 0x8210, 0x6108, 0x4108, 0x2000
};

// vous allez voir que nous allons optimiser certaines choses et que,
// finalement, si vous virez tous les commentaires, cette méthode est
// somme toute assez concise :-)
void Tiling::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    bool txodd,tyodd;
    bool jump;
    uint16_t nfo = TILE_WIDTH * TILE_HEIGHT;
    uint8_t sy,x,y;
    uint8_t xo,yo;
    uint16_t syw;
    uint16_t index, index_y, value;

    // nous aurons besoin de mesurer le carré de la distance `r2`
    // entre le pixel du dallage et le centre de l'écran...
    // un calcul intermédiaire permettra de déterminer
    // la composante Y de `r2` que nous nommerons `ry2`
    uint16_t r2,ry2;
    uint8_t hsw = SCREEN_WIDTH / 2;  // le centre
    uint8_t hsh = SCREEN_HEIGHT / 2; // de l'écran

    // nous calculerons alors le niveau de luminosité du pixel
    // en fonction du rapport entre le carré de la distance `r2`
    // qui le sépare du centre de l'écran et le carré du rayon
    // du halo de lumière, défini sous la forme d'une
    // puissance de 2 : `HALO_RADIUS2_POWER_OF_TWO`
    uint8_t lux;

    // cette variable nous permettra de stocker l'index
    // de la couleur récupérée dans la spritesheet et
    // correspondant à l'index de la couleur de référence
    // dans la palette `COLORMAP`
    uint8_t colorIndex;

    // on plonge toute la scène dans le noir en remplissant la tranche de zéros
    // > nous utilisons des pointeurs ici pour accélérer le traitement...
    // on pourrait également déplacer cette procédure
    // dans la méthode `draw()` du `Renderer`
    uint32_t* tmp = (uint32_t*)buffer;
    uint16_t bs = (sliceHeight * SCREEN_WIDTH) >> 1;
    while (bs--) *tmp++ = 0;

    // allez... c'est parti !
    for (sy = 0; sy < sliceHeight; sy++) {
        syw = sy * SCREEN_WIDTH;
        y = sliceY + sy;
        yo = y + this->offsetY;
        tyodd = (yo / TILE_HEIGHT) % 2;
        index_y = (yo % TILE_HEIGHT) * TILE_WIDTH;

        // on calcule la composante Y du carré de la distance
        // qui sépare le pixel du centre de l'écran
        ry2 = (y - hsh) * (y - hsh);

        for (x = 0; x < SCREEN_WIDTH; x++) {
            xo = x + this->offsetX;
            txodd = (xo / TILE_WIDTH) % 2;
            jump = txodd ^ tyodd;
            index = index_y + (xo % TILE_WIDTH) + (jump * nfo);

            // on calcule le carré de la distance qui sépare
            // le pixel du centre de l'écran
            r2 = (x - hsw) * (x - hsw) + ry2;

            // si le pixel est à l'intérieur du halo de lumière
            if (!(r2 >> HALO_RADIUS2_POWER_OF_TWO)) {
                // on récupère le code couleur du pixel de la dalle
                // dans notre spritesheet
                colorIndex = BITMAP[index];

                // et s'il ne s'agit pas de la couleur de transparence
                if (colorIndex--) {
                    // remarque sur le `--`
                    // si colorIndex n'est pas l'index la couleur de transparence
                    // alors colorIndex > 0 ... donc on va décrémenter sa valeur
                    // immédiatement après l'avoir lue pour simplifier le calcul
                    // qui suit, qui consistera à piocher le code couleur dans
                    // la palette des couleurs `COLORMAP`
                    //
                    // on calcule le niveau de luminosité à cette distance
                    lux = (r2 << BRIGHTNESS_LEVELS_POWER_OF_TWO) >> HALO_RADIUS2_POWER_OF_TWO;

                    // on va ensuite piocher le code couleur correspondant
                    // dans la palette de couleurs...
                    // > et on n'oublie pas d'appliquer le décalage correspondant
                    // > à la couleur de transparence, qui est le premier élément
                    // > de la palette
                    // et on termine en écrivant cette valeur dans le buffer
                    buffer[x + syw] = COLORMAP[1 + (colorIndex << BRIGHTNESS_LEVELS_POWER_OF_TWO) + lux];
                }
            }
        }
    }
}

// et voilà le travail ;-)
```

Vous voyez ici que, dans la mesure où nous avons initialisé notre buffer avec des `0`,
nous pouvons nous contenter de n'y écrire que lorsque nous sommes à l'intérieur du halo de lumière, et que la couleur à reporter n'est pas la couleur de transparence du sprite. Ceci nous fait gagner énormément de temps en diminuant les opérations de calcul et d'écriture dans le tampon !

Bien... nous pouvons maintenant compiler et lancer l'exécution :

![shading with 32 levels](../../assets/figures/shading/shading-32-levels.jpg){: width="320" height="256" class="shadow" }

<div style="margin:1em 0;text-align:center;">Le rendu est parfait <i class="far fa-smile"></i></div>

Jetons maintenant un oeil aux performances :

```
[Starting] Opening the serial port - /dev/cu.usbmodem141401
[Info] Opened the serial port - /dev/cu.usbmodem141401
CPU: 73, RAM: 9739
CPU: 73, RAM: 9739
CPU: 73, RAM: 9739
```

<div style="margin:1.5rem 0;font-size:3em;text-align:center;">🍺</div>

On a bien bossé : on a fait chuter la charge du CPU de `98%` à `73%`... Bravo !

Nous devrions pouvoir augmenter le framerate à `32` pour être à `100%` au niveau de la charge du CPU. Ce qui augmentera l'impression de fluidité au rendu.

<div class="filename">ShadingEffect.ino --> setup()</div>
```c++
gb.setFrameRate(32);
```

Et voilà ! Ce tutoriel est terminé. On a fait du bon boulot. Le rendu finalisé a quand même de la gueule, vous en conviendrez <i class="far fa-smile-wink"></i>. Il me reste à ajouter le mot de la fin... donc direction le prochain et dernier chapitre...
