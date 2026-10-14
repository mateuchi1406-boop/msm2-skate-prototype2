# MSM2-Scripting

This repo contains code for MSM2-Script-SDK and a template script that uses it.

The template is adapted as the first `SM2-Skateboard-Prototype` slice. It is a
single-player movement/trick input test and does not include game files or
assets from Skate 3.

The main purpose of the SDK script is to consolidate knowledge, allowing other scripts to depend on it. If such scripts don't hook any functions on their own and only rely on SDK, only the SDK will have to be updated when the game updates.

# Usage

Dependent scripts can use the .lib by including .h files from 'MSM2-Script-SDK/game/'.

## Prototype controls

- Xbox **View + Menu** toggles skate movement (`F7` also works).
- Xbox **left stick** moves (`WASD` also works).
- Xbox **A** performs an ollie (`Space` also works).
- Xbox **right trigger** increases speed (`Shift` also works).
- Xbox **right stick (R3)** recognizes ollie, nollie, kickflip, heelflip and
  frontside/backside shuvit flicks.

The current public SDK exposes the hero transform, but not a documented model
spawning, attachment or animation API. The trick input recognizer is therefore
implemented before the visible board and character animations.

# Building

To build the SDK .dll, you'd need to use vcpkg to download the dependencies. You may need to run `vcpkg integrate install` once to enable it.

Both SDK's and template script's projects have an extra property, `$GameDirectory`, which can be set via project properties in MSVC. As a post-build step, .dll is copied to 'scripts/' subfolder of that directory. You could also setup debug target to be the game executable with `-scripts -console` arguments for easier debugging.

# Credits and license

SDK code is heavily based on LDD565's [SM2ScriptTemplate](https://github.com/hbgda/SM2ScriptTemplate).

Following Overstrike, code here is under GPLv3.
