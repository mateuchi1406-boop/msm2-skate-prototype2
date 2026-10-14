# Skateboard prototype for Marvel's Spider-Man 2

This is a single-player prototype based on the public MSM2 Script SDK. It does
not include game files or assets from Skate 3.

## Current controls

- Xbox `View + Menu`: enable/disable skate movement (`F7` also works)
- Xbox left stick: move (`WASD` also works)
- Xbox right trigger: faster movement (`Shift` also works)
- Xbox `A`: ollie (`Space` also works)
- Xbox right stick (R3): directional trick input

Right-stick mappings: up = ollie, down = nollie, up-right = kickflip,
up-left = heelflip, right = frontside shuvit and left = backside shuvit.

## Current limitation

The public SDK exposes the hero transform, but not a documented model-spawn,
model-attachment or animation API. The movement and trick input layer is ready;
the visible board and character animations are the next milestone.
