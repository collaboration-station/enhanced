# Collaboration Station: Enhanced
_Collaboration Station: Enhanced_ is a cooperative multiplayer mobile game set aboard the International Space Station.
See [collaborationstationgame.info](http://collaborationstationgame.info) for more information about the project,
including teacher resources and lesson plans.

The source code for this app is provided under the terms of the [GNU General Public License version 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html). 
The assets are provided under [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).

## Project History
This is the repository for _Collaboration Station: Enhanced_. The original _Collaboration Station_ game was
written in [PlayN](http://playn.io) and [released for Android on the Google Play 
Store](https://play.google.com/store/apps/details?id=edu.bsu.issgame.android&hl=en).
[This repository](https://github.com/collaboration-station/playn) contains the source code
for the original release; the source is licensed under [GPL3](https://www.gnu.org/licenses/gpl-3.0.en.html) 
and the assets are available under [CC BY-NC](https://creativecommons.org/licenses/by-nc/4.0/legalcode).

## Building

The game was written using [Unreal Engine 4.18.3](https://www.unrealengine.com).
To build this project, you will need three plug-ins from [GameDNA](https://gamednastudio.com/).
Make a `Plugins` directory in your project folder,
[download the gameDNAinstaller from GitHub](bhttps://github.com/gameDNAstudio/gameDNAinstaller/releases), and extract it into that folder. You will also need the commercial [RealtimeDatabase](https://gamednastudio.com/realtime-database/) 
and [Ultimate Mobile Kit](https://gamednastudio.com/ultimate-mobile-kit/)
plug-ins, each also extracted to that folder. 
Additional configuration information for these plug-ins can be found in their respective documentation.

## Code Conventions

For Blueprints, try to keep everything as clean as you can;
[Allar's UE4 Style Guide](https://github.com/Allar/ue4-style-guide) provides good guidance here.
Use well-named functions and macros to break down large
networks. Use comments to clarify intent, especially for events that must be part of the main graph.

For C++, we generally follow [the Unreal Engine conventions](https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/).

## Acknowledgements

The development of the original game was supported by an internal [immersive learning](http://bsu.edu/immersive)
grant at [Ball State University](http://bsu.edu). The revised version is supported by a grant from the [Indiana Space Grant Consortium](https://insgc.spacegrant.org/) with additional support from the [Computer Science Department at Ball State University](http://www.cs.bsu.edu).
