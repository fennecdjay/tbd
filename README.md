# TBD

## Acquiring

### Source Tree

To download the source tree, run the following command:  
`$ git clone --recursive https://github.com/cometodukey/tbd`

If you have already cloned without `--recursive`, you can run this command to recursively download submodules:  
`$ git submodule update --init --recursive`

### Meson

Meson should be installed via your systems package manager.

If Meson is not available on your systems repositories, you can find a guide on how to install it [here](https://mesonbuild.com/FAQ.html#how-to-use-meson-on-a-host-where-it-is-not-available-in-system-packages).

## Building

Run this command to setup Meson:  
`$ meson setup build`

If your version of Meson is newer than 0.55.0, you can run this:  
`$ meson compile`

Otherwise, run this:  
`$ ninja -C build/`

## TODO

* Apply const, restrict wherever sensible
* internal_XXX for internal functions
* XXX_new for constructors
* XXX_delete for destructors
* first parameter should be `self`
* Improve macro hygiene with temp vars
* Proper Unicode and UTF-8 support
* Only use term colours if std{out,err} is a terminal and supports them
* Debug/release builds
