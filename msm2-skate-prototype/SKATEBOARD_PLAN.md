# Skateboard prototype for Marvel's Spider-Man 2

## Scope

This project starts with a safe single-player prototype:

1. Toggle skate mode.
2. Attach a board model under the active hero.
3. Move, brake and turn while grounded.
4. Add a small jump/ollie and trick state later.

The project is based on the public MSM2 Script SDK/template and is intended to
be packaged as an Overstrike `.script` mod. It does not include game files or
assets from Skate 3.

## Current status

- Public `MSM2-Scripting` SDK/template downloaded.
- No files in the Steam game directory have been changed.
- No existing user mods are enabled or modified.
- The template currently demonstrates access to the hero transform; it does
  not yet spawn a board or implement skating.

## Next technical milestone

Build the template against the current SDK, verify that the script loads in a
separate Overstrike profile, then add the board asset and grounded movement.
