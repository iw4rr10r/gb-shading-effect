---
rank: 5
layout: default
title: Patron Observateur
permalink: /fr/patron-observateur/
lang: fr
lang-ref: observer
---

# Le patron de conception Observateur

Le *Design Pattern* **Observateur** (ou ***Observer***) est utilisé pour envoyer un signal à des objets qui jouent le rôle d'*observateurs*. En cas de notification, les observateurs effectuent alors l'action adéquate en fonction des informations qui parviennent depuis les objets qu'ils observent (les *observables*). Les notions d'observateur et d'observable permettent de limiter le couplage entre les objets aux seuls phénomènes à observer. Elles permettent également une gestion simplifiée d'observateurs multiples sur un même objet observable.

Dans ce modèle, le sujet observable se voit attribuer une collection d'observateurs qu'il notifie lors de changements d'états. Chaque observateur est chargé de faire les mises à jour adéquates en fonction des changements notifiés. Ainsi, l'observé n'est pas responsable des changements qu'il impacte sur les observateurs.

Nous allons appliquer ce patron dans le cadre de notre application. L'observable sera bien évidemment le moteur de rendu `Renderer` et les observateurs seront tout simplement les objets affichables sur la scène de jeu : la bille et le dallage sur lequel se déplace la bille.

Comme nous l'avons mentionné plus haut, l'observable se voit attribuer une collection d'observateurs. Nous aurons donc besoin d'une structure de données pour gérer cette collection. Et pour garder une certaine généricité, c'est-à-dire conserver une relative indépendance des types de données des observateurs, nous allons définir une **interface** pour qualifier nos observateurs de manière générique. Une interface est une sorte de contrat auquel doivent se plier tous les objets qui y souscrivent.

Nous pourrions ainsi définir une interface `Renderable` qui garantisse que tout objet ayant souscrit à ce contrat est capable de se dessiner (et donc d'effectuer un rendu de son apparence graphique). De cette façon, le moteur de rendu aurait juste à gérer une collection d'objets `Renderable`, sans avoir véritablement besoin de connaître la nature exacte des observateurs. En effet, il a juste besoin de s'assurer qu'ils sont capables de se dessiner.


# Définition de l'interface Renderable

Cette interface peut se définir très simplement :

<div class="filename">Renderable.h</div>
```c++
#ifndef SHADING_EFFECT_RENDERABLE
#define SHADING_EFFECT_RENDERABLE

#include <Gamebuino-Meta.h>

class Renderable
{
    public:

        // méthode virtuelle pure
        virtual void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) = 0;
};

#endif
```

La notion d'interface n'existe pas au sens strict du terme en C++, comme on peut la trouver en Java ou en Ada par exemple. Mais il existe une possibilité pour l'émuler en s'appuyant sur la notion de classe abstraite et de méthode virtuelle *pure*. Une méthode virtuelle pure est une méthode qui est déclarée mais non définie dans une classe. Elle est définie dans une des classes dérivées. Pour déclarer une méthode virtuelle pure dans une classe, il suffit de faire suivre sa déclaration de « `= 0` ». La méthode doit également être déclarée avec le mot-clef `virtual`.

Donc, chaque objet répondant à l'*interface* `Renderable` devra implémenter la méthode `draw()` pour se dessiner lorsqu'il recevra une notification du moteur de rendu `Renderer` pour le prévenir que c'est à son tour de se dessiner. Et souvenez-vous de ce que nous avions mentionné dans la définition de la classe `Renderer` :

<div class="filename">Renderer.cpp</div>
```c++
// exécution du rendu de la scène de jeu
void Renderer::draw() {
    // ...
    // ---------------------------------------------------------------------
    // ici nous effectuerons les calculs de rendu de nos éléments graphiques
    // à partir des 3 paramètres suivants :
    // - sliceY       : ordonnée de la tranche courante (frange supérieure)
    // - SLICE_HEIGHT : hauteur des tranches
    // - buffer       : pointeur vers le tampon courant
    // ---------------------------------------------------------------------
    // ...
}
```

Nous retrouvons donc ces 3 paramètres comme arguments de la fonction `draw()`.

Bien, maintenant que le *contrat* est établi, il faut ajouter à notre moteur de rendu la capacité de gérer une collection d'observateurs `Renderable`. Nous pourrions gérer cela avec un simple tableau... l'ennui, c'est qu'en C++ un tableau doit nécessairement être dimensionné de manière à ce que l'espace mémoire nécessaire à le contenir soit réservé. Or, dans un jeu, le nombre d'objets présents sur la scène de jeu et devant être affichés peut être très variable. Il est donc dangereux ou limitatif de définir la taille d'un tel tableau *a priori*. On peut même avoir tendance à vouloir allouer plus d'espace que nécessaire pour éviter d'atteindre les limites du nombre d'objets pouvant être référencés dans ce tableau. C'est donc définitivement une très mauvaise idée !

On préfèrera dans ce cas gérer une structure de données dynamique, dont la taille peut varier selon les besoins au cours de l'exécution du programme. L'une des structures de données dynamiques bien connues parmi les plus simple en algorithmique est la *liste chaînée*.


# Collection dynamique d'observateurs

Nous allons donc attribuer une liste d'observateurs à notre moteur de rendu. Cette liste sera désignée par la variable statique `listeners` (les observeurs sont également appelés des *écouteurs*). Alors, faudra-t-il déclarer cette variable de cette façon ?

```c++
static Renderable* listeners;
```

**NON !** C'est un peu plus compliqué que ça <i class="far fa-smile-wink"></i>.

Les observateurs devront être rangés dans des boîtes que nous allons enchaîner les unes aux autres avec des pointeurs. Chacune de ces boîtes sera considée comme un chaînon de la liste. Ces chaînons constitueront les *noeuds* (`Node`) de la liste. Et voici comment va se présenter la structure de cette liste chaînée :

![linked-list](../../assets/figures/observer/linked-list.png){: width="504" height="186" }

Chaque `Node` est constitué de 2 pointeurs :

1. `renderable` qui est un pointeur vers un objet de type `Renderable`
2. `next` qui est un pointeur vers un objet de type `Node`

Le pointeur `renderable` permet donc d'accéder à l'objet de type `Renderable` qui est encapsulé dans le noeud, tandis que le pointeur `next` permet d'accéder au noeud suivant de la liste.

La déclaration correcte de notre variable statique `listeners` est donc :

```c++
static Node* listeners;
```

Et le dernier noeud de la liste aura son pointeur `next` qui pointe sur NULL (rien).

Voyons maintenant comment implémenter cette liste chaînée. Pour cela nous allons avoir besoin de déclarer la classe `Node` :

<div class="filename">Node.h</div>
```c++
#ifndef SHADING_EFFECT_NODE
#define SHADING_EFFECT_NODE

#include "Renderable.h"

class Node
{
    private :

        // pointeur vers l'objet de type `Renderable`
        Renderable* renderable;
        // pointeur vers le noeud suivant de la liste
        Node* next;

    public:

        // constructeur du noeud
        Node(Renderable* renderable);
        // destructeur du noeud
        ~Node();

        // méthode d'accès à l'objet `Renderable` encapsulé
        Renderable* getRenderable();
        // méthode d'accès au noeud suivant de la liste
        Node* getNext();

        // méthode de recherche d'un objet `Renderable` en particulier
        Node* search(Renderable* renderable);
        // méthode qui permet d'ajouter un nouveau noeud à la liste
        void add(Renderable* renderable);
        // méthode qui permet de supprimer un noeud de la liste
        void del(Renderable* renderable);
};

#endif
```

Dont voici la définition :

<div class="filename">Node.cpp</div>
```c++
#include "Node.h"

// le constructeur initialise les attributs du noeud
Node::Node(Renderable* renderable) : renderable(renderable), next(NULL) {};

// le destructeur "oublie" les objets qu'il référençait
Node::~Node() {
    this->renderable = NULL;
    this->next = NULL;
}

// permet d'accéder à l'objet `Renderable` encapsulé
Renderable* Node::getRenderable() {
    return this->renderable;
}

// permet d'accéder au noeud suivant de la liste
Node* Node::getNext() {
    return this->next;
}

// recherche un objet `Renderable` en particulier
// et retourne un pointeur sur le noeud détenteur de cet objet
Node* Node::search(Renderable* renderable) {
    if (this->renderable == renderable) {
        return this;
    }

    // si on arrive en fin de liste, c'est que
    // l'objet `Renderable` recherché n'a pas été trouvé
    if (this->next == NULL) {
        return NULL;
    }

    // si le noeud courant n'en est pas le détenteur,
    // alors on effectue la recherche dans le noeud suivant,
    // et ainsi de suite...
    return this->next->search(renderable);
}

// ajoute un noeud en FIN de liste
void Node::add(Renderable* renderable) {
    if (this->next == NULL) {
        this->next = new Node(renderable);
    } else {
        this->next->add(renderable);
    }
}

// supprime le noeud détenteur de l'objet `Renderable`
// que l'on cherche à indentifier...
void Node::del(Renderable* renderable) {
    if (this->next != NULL) {
        if (this->next->getRenderable() == renderable) {
            Node* toDelete = this->next;
            this->next = toDelete->getNext();
            delete toDelete;
        } else {
            this->next->del(renderable);
        }
    }
}
```

Vous voyez que dans le cas de la suppression d'un noeud, on part du principe ici que le noeud courant n'encapsule pas l'objet `Renderable` que l'on recherche. En effet, la recherche s'effectue à partir du noeud suivant. C'est normal, ne vous inquiétez pas, la suppression du noeud courant ne peut pas se faire ici. En effet, si l'on supprimait le noeud courant... alors le noeud précédent perdrait sa référence vers son suivant (qui est justement le noeud courant)... et perdrait du même coup toute la liste ! On ne peut donc pas traiter ce cas ici. Nous le traiterons dans la classe `Renderer` qui détient la référence à la tête de la liste.


# Enregistrement des observateurs

Rappelez-vous qu'en définitive, la gestion de cette liste chaînée doit nous permettre d'assurer la gestion des observateurs du `Renderer`. Et donc, le fait d'ajouter ou de supprimer un observateur dans cette liste se traduit, du point de vue du `Renderer`, comme le fait de permettre à un objet de type `Renderable` de pouvoir s'abonner ou se désabonner aux notifications du `Renderer`. Voilà donc comment compléter la déclaration de la classe `Renderer` :

<div class="filename">Renderer.h</div>
```c++
#ifndef SHADING_EFFECT_RENDERER
#define SHADING_EFFECT_RENDERER

#include <Gamebuino-Meta.h>
#include "Node.h"
#include "Renderable.h"
#include "constants.h"

#define SLICE_HEIGHT 8

class Renderer
{
    private:
    
        // pointeur vers la liste d'observateurs
        static Node* listeners;

    public:

        // abonnement d'un observateur
        static void subscribe(Renderable* renderable);
        // désabonnement d'un observateur
        static void unsubscribe(Renderable* renderable);
        // permet de savoir si un objet de type `Renderable` est déjà abonné
        static bool hasSubscribed(Renderable* renderable);
    
    // toutes les autres déclaration restent inchangées
};

#endif
```

Voilà comment définir ces nouvelles méthodes :

<div class="filename">Renderer.cpp</div>
```c++
// par défaut, la liste est vide
Node* Renderer::listeners = NULL;

// abonnement d'un observateur
void Renderer::subscribe(Renderable* renderable) {
  // si la liste est vide, on l'initialise avec le nouvel observateur ;-)
  if (listeners == NULL) {
    listeners = new Node(renderable);
  } else {
    // sinon on délègue l'ajout au noeud suivant
    // puisque l'ajout doit se faire en FIN de liste
    listeners->add(renderable);
  }
}

// désabonnement d'un observateur
void Renderer::unsubscribe(Renderable* renderable) {
  // si la liste est vide, il n'y a rien à faire !
  // par contre, si elle ne l'est pas...
  if (listeners != NULL) {
    // si le noeud de tête de la liste est bien celui
    // qui encapsule l'observateur que l'on cherche à identifier
    if (listeners->getRenderable() == renderable) {
      // on récupère la référence vers le noeud suivant
      // qui va devenir la nouvelle tête de liste
      Node* next = listeners->getNext();
      // et on peut donc définitivement supprimer le premier noeud
      delete listeners;
      // on raccroche le nouveau noeud de tête de liste
      listeners = next;
    } else {
      // dans le cas contraire, on délègue la suppression
      // au noeud suivant dans la liste...
      listeners->del(renderable);
    }
  }
}

// pour savoir si un objet de type `Renderable` est déjà abonné
bool Renderer::hasSubscribed(Renderable* renderable) {
  // il suffit d'identifer le noeud qui en est le détenteur
  return listeners->search(renderable) != NULL;
}
```

Vous voyez que notre problème de suppression de noeud est désormais résolu <i class="far fa-smile-wink"></i>.


# Envoi des notifications

Il nous reste une dernière chose à faire ici ! Mettre en place le mécanisme de notification à l'ensemble des observateurs. Et oui, maintenant nous pouvons enfin demander à tous nos observateurs de se dessiner chacun leur tour dans l'ordre de la pile d'affichage, c'est-à-dire précisément dans l'ordre où ils sont rangés dans la liste d'abonnement. Le noeud de tête se dessine, puis le noeud suivant, et ainsi de suite, jusqu'à la fin de la liste :

<div class="filename">Renderer.cpp</div>
```c++
void Renderer::draw() {
  if (listeners != NULL) {
    uint8_t slices = SCREEN_HEIGHT / SLICE_HEIGHT;
    for (uint8_t sliceIndex = 0; sliceIndex < slices; sliceIndex++) {
      uint16_t* buffer = sliceIndex % 2 == 0 ? buffer1 : buffer2;
      uint8_t sliceY = sliceIndex * SLICE_HEIGHT;
    
      // il suffit d'envoyer le signal au noeud de tête
      // qui le relaiera à son tour au noeud suivant,
      // et ainsi de suite, jusqu'à la fin de la liste
      listeners->draw(sliceY, SLICE_HEIGHT, buffer);

      if (sliceIndex != 0) waitForPreviousDraw();
      customDrawBuffer(0, sliceY, buffer, SCREEN_WIDTH, SLICE_HEIGHT);
    }
    waitForPreviousDraw();
  }
}
```

Pour mettre en place ce mécanisme, il nous reste donc à ajouter dans la classe `Node` la prise en compte du signal de notification pour le relayer à l'objet de type `Renderable` qu'il encapsule. Donc, dans la classe `Node` nous allons déclarer une méthode `draw()` qui va se charger de transmettre cette notification :

<div class="filename">Node.h</div>
```c++
class Node
{
    public:

        // relaie la notification provenant du `Renderer`
        // qu'il est temps de se dessiner
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer);
};
```

<div class="filename">Node.cpp</div>
```c++
void Node::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    // l'observateur se dessine
    this->renderable->draw(sliceY, sliceHeight, buffer);
    // et on passe le relais au noeud suivant s'il existe
    if (this->next != NULL) {
        this->next->draw(sliceY, sliceHeight, buffer);
    }
}
```

Et la boucle est bouclée <i class="far fa-smile-wink"></i>.

Vous voyez que notre interface `Renderable` est bien utile ici. On s'appuie sur le seul fait que le contrat décrit par cette interface nous garantit que l'objet est capable de se dessiner. Le moteur de rendu n'a ici aucune connaissance de la nature exacte de ses observateurs. Et pourtant tout fonctionne à merveille.

Bien, il est temps maintenant de nous intéresser aux objets que nous allons dessiner sur la scène de jeu : la bille et le dallage sur lequel elle se déplace. Nous commencerons par la bille, dans le prochain chapitre...