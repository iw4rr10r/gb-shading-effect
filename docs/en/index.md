---
rank: 1
layout: default
title: Overview
lang: en
lang-ref: index
---

# Overview of the tutorial

When Andy ([aoneill](https://gamebuino.com/@aoneill)) published his article about [High Resolution without gb.display](https://gamebuino.com/creations/high-resolution-without-gb-display), I vaguely looked at it, and I thought to myself, "Wow.... It looks hairy... and what's the story with the DMA? What's the DMA? And in addition, it's written in the official doc that `gb.tft` is part of the **danger zone**... okay, well, we'll see about that later eh!". I started from scratch in C++ just 2 months ago... it scared me...

And then, after finishing my first tutorial on the [Game of Life](https://m1cr0lab-gamebuino.github.io/gb-game-of-life/), I was finally beginning to understand something about that *barbarbaric* language, and I said to myself, "Come on! Next step: understanding Andy's thing... it still looks pretty good. Well, and this guy seems to be doing crazy stuff..."

What a great idea I had that day.... I had the impression I was discovering the Holy Grail! So, believe me, if you haven't read it yet.... Go for it!

So I immediately wanted to write a little project to see if I understood his technique... and it gave what I will develop for you in this tutorial. But don't stop at its simplistic title. I sincerely believe that it develops many useful tips for video game programming. And it explains in detail Andy's magical technique. So I think it could really be life-saving for all of you, if you didn't understand Andy's method correctly.

One last thing, please excuse my vague English... my natural language is French, but I also wanted to offer this tutorial in English to the whole community. I did what I could <i class="far fa-frown"></i>.


# What are the prerequisites?

|                   | Beginner |            Intermediary            |           Advanced           | Expert |
|------------------:|:--------:|:----------------------------------:|:----------------------------:|:------:|
|**Required level** |          | <i class="fas fa-check faded"></i> | <i class="fas fa-check"></i> |        |

To be able to follow this tutorial in the best conditions, you must:

- have successfully completed all the workshops of the [Academy](https://gamebuino.com/academy),
- master the general principles of the C++ language,
- master the notion of pointers,
- practice Object Oriented Programming,
- have read [Andy](https://gamebuino.com/@aoneill)'s article [High Resolution without gb.display](https://gamebuino.com/creations/high-resolution-without-gb-display).

Owning a Gamebuino META is essential here to be able to test your code under good conditions. Unfortunately, [Andy's online emulator]((http://games.aoneill.com/meta-emulator/)) does not work properly in high-resolution.


# What you will learn here

... quite a few things actually!

- you will better understand Andy's technique;
- then I will propose a way to develop a rendering engine that implements this technique dedicated to high resolution;
- you will learn what the **Observer** Design Pattern is for;
- as well as how to apply it in a rendering engine;
- then you will learn to program a linked list, which is part of the dynamic collections very useful in algorithmics;
- we will then move on to a more theoretical chapter, where I will present the mathematical notions of the kinematics of material point, as well as the fundamental relationship of the dynamics from Newtonian mechanics, to finally approach the principle of inertia in physics... all this to give you the keys to the **motion** programming in your games;
- and once all this is in place, we will finally tackle the **shading** technique: a method to play with light and create a subdued atmosphere in your game scenes.

That's it, basically... it's a lot of things. I hope you will enjoy discovering all these notions as much as I had the opportunity to write them myself <i class="far fa-smile"></i>.