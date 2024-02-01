# SK Macro

Welcome to the SK Macro project. This is the first version of my macro pad and while it's far from perfect, it's a start.

Watch the build video ↓

[<img src="/img/Thumbnail%20-%202.jpg">](https://www.youtube.com/watch?v=0lB-7FOjO98)

Building your own is possible, though I wouldn't wholeheartedly recommend it unless you're are willing to make some sacrifices.

If you want to take a look at the Fusion files you can find them on [Printables](https://www.printables.com/model/741922). Due to bandwith limitations I can't upload them here :(

# Hardware

<a href="https://www.pcbway.com/project/shareproject/DIY_Macropad_ab1137f4.html"><img src="/img/banner.jpg" alt="PCB from PCBWay" /></a>

My version is compatible with both Choc and MX switches using hot-swap sockets. As long as you solder the correct socket, you can easily swap your switches.

If you decide to build your own version make sure to solder the IO expander in the correct oriantation. In general it would be best to have the Fusion project open as a reference

I originally planned to make this Macropad wireless, but that ultimately didn't work out. I tried reflashing an HC-05 with firmware that should make it HID compatible, but I couldn't get it to work. For this reason, most of the components are not necessary.

# Cost
But how much would something like this even cost

For me, the price ended up around 32€ (see Parts (toBuy).xlsx). 

However, this does not include shipping, which might differ greatly depending on where you live. I spent around 25€ on shipping, with LCSC making up a large portion of that (I can't recall exactly, but around 15€), so you might be able to save a bit by purchasing somewhere else.

# Firmware

In terms of firmware, I was forced to create my own. It's currently missing a lot of features, but it's a work in progress. If you want to compile it ypu will need to install a few things apart from VS Code and the PlatformIO addin. Instructions for this can be found [here](https://arduino-pico.readthedocs.io/en/latest/platformio.html)

I wasn't able to get KMK to work since I used an IO expander without a keyboard matrix. As I have never used KMK before, I wasn't able to configure it. However, if you're able to do it, I would love to hear your input. It should be possible.

Thank you for your interest in the SK Macro project. Any feedback or contributions are greatly appreciated.
