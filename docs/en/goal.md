---
rank: 2
layout: default
title: Goal
permalink: /en/goal/
lang: en
lang-ref: goal
---

# What is our goal?

For this exercise, we will perform a very simple little game scene, in which it will be a question of moving a small ball on a regular tiling similar to an infinite chessboard. The ball's motion commands will be applied using the console's `PAD`. I let you discover in this short video the result that we will try to achieve roughly throughout this tutorial:

<iframe width="560" height="315" src="https://www.youtube.com/embed/OBBptplCc44" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope;" allowfullscreen></iframe>

The purpose of this tutorial will be to detail how to perform the *lighting* of the game scene. The paving will be plunged into darkness and partially illuminated where the ball is located (the center of the screen here). The light intensity is inversely proportional to the square of the distance as you move away from the ball.


# The graphic assets

We will only need two small graphic elements to render the game scene. I drew them with the online editor [Piskel](https://www.piskelapp.com/), which is a very well done and free tool:

![assets](../../assets/figures/goal/assets.png){: width="226" height="126" }

You can [download](../../assets/downloads/shading-effect-assets.zip) these graphic assets ... you won't have to draw them! <i class="far fa-smile-wink"></i>


# Preliminary reading

The display technique we will use throughout this tutorial is probably not the one you are used to applying... Indeed, to be able to handle the display in high resolution (160x128 pixels), we will unfortunately not be able to rely on the standard `gb.display` API which does not allow to handle a display in RGB565 (full 16-bit color) with only 32 KB of RAM. So we won't be able to benefit from the very useful functions you are used to using, such as `drawPixel()`, `drawCircle()`, `fillRect()` or `drawImage()`. We will have to completely recode the display of our graphic assets... bit by bit... to be able to directly tackle the DMA controller (*Direct Memory Access*) via the `gb.tft` API. It is the only solution to handle a high-resolution display in full 16-bit color (and with reasonable performance). Our highly valued [Andy](https://gamebuino.com/@aoneill) has published a [very interesting article](https://gamebuino.com/creations/high-resolution-without-gb-display) on the subject, in which he details how to implement this *low level* technique. We will have the opportunity to come back to this in the next chapter, but I strongly encourage you to take a closer look at this article before going any further! It will prevent you from deciphering the file [Display-ST7735.cpp](https://github.com/Gamebuino/Gamebuino-META/blob/master/src/utility/Display-ST7735/Display-ST7735.cpp) in the [Gamebuino-Meta](https://github.com/Gamebuino/Gamebuino-META) library, which implements the interface with the META Adafruit 1.8" TFT screen, and which will surely appear indigestible. But the reward is huge! This technique opens the doors to high-resolution in full RGB565. A big thank you Andy for giving us the keys of the DMA controller! <i class="far fa-smile-wink"></i>