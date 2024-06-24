# use-nvidia

##  ***this software is almost ready for usage, but still has a few outstanding bugs if you're willing to work around them until I fix them*** (this is their list, sorted by descending priority to me)

- **sudo prompts can sometimes hang or never take user input** [Affects all] {Fix: run sudo on another command before using `unv`}
- **PCI rescans can hang occasionally** [Affects all]
- UNV can hang waiting for the audio device to turn off (which it doesnt turn off) [Commands affected: `unv on`, `unv run`]
- UNV can state the NVIDIA card is on when its off (sometimes, just depends on `nvidia` module being loaded) [Commands affected: `unv status`]
- Some modules' states aren't being grabbed correctly, leading status grabs to sometimes be incorrect [Commands affected: `unv status`]

## Now back to the UNV README

[nvidia-exec](https://github.com/pedro00dk/nvidia-exec) but daemonless.

## Usage

This section is a WIP. This will improve later.

```sh
Usage: unv [action] [options]
Actions:
  on - Just turn your NVIDIA GPU on.
  off - Just turn your NVIDIA GPU off.
  status - Query the status of your NVIDIA GPU.
  ps - List all the processes running on your NVIDIA GPU.
  run - Run a program on your NVIDIA GPU.
Options:
  --verbose - Get output from UNVLogger.

Please note if you are using (unv run), you cannot apply --verbose.
A simple workaround is to edit your config, and change verbose in the app section to true.
```

## Why UNV

UNV was created for one simple purpose - battery consumption.  
UNV is a simple binary that allows you (without any systemd services) to: (on the fly!)

- start any program on your NVIDIA GPU
- turn your NVIDIA GPU on/off as you wish
- see the programs running on it

## Installation

Eventually, when this software becomes much more stable I'll put it on the AUR and there'll be instructions on how to do that.  

However, for now, follow the build instructions.

## Building

Here's the instruction set to build UNV.

```sh
# Clone the git repo
git clone https://github.com/ia74/use-nvidia

# Go into the repo!
cd use-nvidia

# Make the program! And install it to /usr/bin!
make install

# Now...
unv run glxinfo | grep "OpenGL render"
# and see the magic for yourself!
```
