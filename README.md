# Jungle-Doom

**Jungle-Doom** aims to revolutionize Doom modding by integrating Python scripting capabilities into an accurate reproduction of the original DOS version of Doom and other games based on the Doom engine. Designed for modern computers, Jungle-Doom empowers modders with the flexibility and simplicity of Python, eliminating the complexities of traditional scripting languages like ZSCRIPT and DECORATE.

Originally, Jungle-Doom was a fork of Chocolate Doom. The project now includes ports of Heretic, Hexen, and Strife, enhanced with robust Python scripting support.

## Jungle-Doom’s Aims

- **100% Free and Open Source:** Jungle-Doom remains committed to being free and open source software under the GNU GPL.
- **Cross-Platform Portability:** Ensuring compatibility across a wide range of operating systems, including Windows, macOS, and Linux.
- **Accurate Emulation:** Faithfully reproducing the original DOS versions of Doom and related games, including all original bugs for authenticity.
- **Python Scripting Integration:** Providing seamless Python scripting capabilities to make modding more accessible and powerful for modern audiences.
- **Compatibility:** Maintaining compatibility with DOS demo, configuration, and savegame files.
- **Retro Experience:** Delivering an authentic retro feel with display and input behaviors mirroring the original experience.

More information about the philosophy and design behind Jungle-Doom can be found in the **PHILOSOPHY** file distributed with the source code.

## Setting Up Gameplay

For instructions on how to set up Jungle-Doom for play, see the [INSTALL](INSTALL) file.

## Configuration File

Jungle-Doom is compatible with the DOS Doom configuration file (normally named `default.cfg`). Existing configuration files for DOS Doom should therefore simply work out of the box. Additionally, Jungle-Doom provides extended settings to leverage Python scripting capabilities. These are stored in a separate file named `jungle-doom.cfg`.

The configuration can be edited using the `jungle-setup` tool, which includes options for managing Python scripts and integrating custom mods.

## Python Scripting

Jungle-Doom introduces Python scripting as a core feature, enabling ultimate modding flexibility. Modders can write Python scripts to customize game behavior, create new game modes, and enhance gameplay without delving into complex scripting languages.

### Getting Started with Python Scripting

1. **Script Directory:** Place your Python scripts in the `scripts/` directory within the Jungle-Doom installation folder.
2. **Script Examples:** Jungle-Doom includes sample scripts in the `scripts/examples/` folder to help you get started.
3. **API Documentation:** Comprehensive Python API documentation is available to guide you in creating advanced mods.

### Running Python Scripts

Python scripts are automatically loaded and executed by Jungle-Doom during game initialization. To add or remove scripts, modify the `jungle-doom.cfg` file or use the `jungle-setup` tool.

### Example Usage

```python
# scripts/my_mod.py

import jungledoom

def on_player_move(player):
    print(f"Player {player.name} moved to position: {player.position}")

# Hook the function to the player's move event
jungledoom.events.on_player_move += on_player_move
