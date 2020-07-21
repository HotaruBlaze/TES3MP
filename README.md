OpenMW
======

[![Build Status](https://api.travis-ci.org/OpenMW/openmw.svg)](https://travis-ci.org/OpenMW/openmw) [![Build status](https://ci.appveyor.com/api/projects/status/github/openmw/openmw?svg=true)](https://ci.appveyor.com/project/psi29a/openmw)  [![Coverity Scan Build Status](https://scan.coverity.com/projects/3740/badge.svg)](https://scan.coverity.com/projects/3740) [![pipeline status](https://gitlab.com/OpenMW/openmw/badges/master/pipeline.svg)](https://gitlab.com/OpenMW/openmw/commits/master)

OpenMW is an open-source game engine that supports playing Morrowind by Bethesda Softworks. You need to own the game for OpenMW to play Morrowind.

OpenMW also comes with OpenMW-CS, a replacement for Bethesda's Construction Set.

* Version: 0.47.0
* License: GPLv3 (see [LICENSE](https://github.com/OpenMW/openmw/blob/master/LICENSE) for more information)
* Website: https://www.openmw.org
* IRC: #openmw on irc.freenode.net

Font Licenses:
* DejaVuLGCSansMono.ttf: custom (see [files/mygui/DejaVu Font License.txt](https://github.com/OpenMW/openmw/blob/master/files/mygui/DejaVu%20Font%20License.txt) for more information)

Current Status
--------------

The main quests in Morrowind, Tribunal and Bloodmoon are all completable. Some issues with side quests are to be expected (but rare). Check the [bug tracker](https://gitlab.com/OpenMW/openmw/issues?label_name%5B%5D=1.0) for a list of issues we need to resolve before the "1.0" release. Even before the "1.0" release however, OpenMW boasts some new [features](https://wiki.openmw.org/index.php?title=Features), such as improved graphics and user interfaces. 

Pre-existing modifications created for the original Morrowind engine can be hit-and-miss. The OpenMW script compiler performs more thorough error-checking than Morrowind does, meaning that a mod created for Morrowind may not necessarily run in OpenMW. Some mods also rely on quirky behaviour or engine bugs in order to work. We are considering such compatibility issues on a case-by-case basis - in some cases adding a workaround to OpenMW may be feasible, in other cases fixing the mod will be the only option. If you know of any mods that work or don't work, feel free to add them to the [Mod status](https://wiki.openmw.org/index.php?title=Mod_status) wiki page.

Getting Started
---------------

* [Official forums](https://forum.openmw.org/)
* [Installation instructions](https://wiki.openmw.org/index.php?title=Installation_Instructions)
* [Build from source](https://wiki.openmw.org/index.php?title=Development_Environment_Setup)
* [Testing the game](https://wiki.openmw.org/index.php?title=Testing)
* [How to contribute](https://wiki.openmw.org/index.php?title=Contribution_Wanted)
* [Report a bug](https://gitlab.com/OpenMW/openmw/issues) - read the [guidelines](https://wiki.openmw.org/index.php?title=Bug_Reporting_Guidelines) before submitting your first bug!
* [Known issues](https://gitlab.com/OpenMW/openmw/issues?label_name%5B%5D=Bug)

The data path
-------------

The data path tells OpenMW where to find your Morrowind files. If you run the launcher, OpenMW should be able to pick up the location of these files on its own, if both Morrowind and OpenMW are installed properly (installing Morrowind under WINE is considered a proper install).

Command line options
--------------------

    Syntax: openmw <options>
    Allowed options:
      --help                                print help message
      --version                             print version information and quit
      --data arg (=data)                    set data directories (later directories
                                            have higher priority)
      --data-local arg                      set local data directory (highest
                                            priority)
      --fallback-archive arg (=fallback-archive)
                                            set fallback BSA archives (later
                                            archives have higher priority)
      --resources arg (=resources)          set resources directory
      --start arg                           set initial cell
      --content arg                         content file(s): esm/esp, or
                                            omwgame/omwaddon
      --no-sound [=arg(=1)] (=0)            disable all sounds
      --script-verbose [=arg(=1)] (=0)      verbose script output
      --script-all [=arg(=1)] (=0)          compile all scripts (excluding dialogue
                                            scripts) at startup
      --script-all-dialogue [=arg(=1)] (=0) compile all dialogue scripts at startup
      --script-console [=arg(=1)] (=0)      enable console-only script
                                            functionality
      --script-run arg                      select a file containing a list of
                                            console commands that is executed on
                                            startup
      --script-warn [=arg(=1)] (=1)         handling of warnings when compiling
                                            scripts
                                            0 - ignore warning
                                            1 - show warning but consider script as
                                            correctly compiled anyway
                                            2 - treat warnings as errors
      --script-blacklist arg                ignore the specified script (if the use
                                            of the blacklist is enabled)
      --script-blacklist-use [=arg(=1)] (=1)
                                            enable script blacklisting
      --load-savegame arg                   load a save game file on game startup
                                            (specify an absolute filename or a
                                            filename relative to the current
                                            working directory)
      --skip-menu [=arg(=1)] (=0)           skip main menu on game startup
      --new-game [=arg(=1)] (=0)            run new game sequence (ignored if
                                            skip-menu=0)
      --fs-strict [=arg(=1)] (=0)           strict file system handling (no case
                                            folding)
      --encoding arg (=win1252)             Character encoding used in OpenMW game
                                            messages:

                                            win1250 - Central and Eastern European
                                            such as Polish, Czech, Slovak,
                                            Hungarian, Slovene, Bosnian, Croatian,
                                            Serbian (Latin script), Romanian and
                                            Albanian languages

                                            win1251 - Cyrillic alphabet such as
                                            Russian, Bulgarian, Serbian Cyrillic
                                            and other languages

                                            win1252 - Western European (Latin)
                                            alphabet, used by default
      --fallback arg                        fallback values
      --no-grab                             Don't grab mouse cursor
      --export-fonts [=arg(=1)] (=0)        Export Morrowind .fnt fonts to PNG
                                            image and XML file in current directory
      --activate-dist arg (=-1)             activation distance override
      --random-seed arg (=<impl defined>)   seed value for random number generator

VR
======

This fork is a VR port of openmw using the openxr VR standard.
You can grab the latest binaries/sources under [Releases](https://gitlab.com/madsbuvi/openmw/-/releases)
Or grab a development build from [Appveyor](https://ci.appveyor.com/project/madsbuvi/openmw-vr) (Click on any build in the jobs table, and look at the artifacts tab)

Current Status
--------------

The VR fork was written on windows with access only to the Oculus Rift headset. Consequentially the current state of the port may have any number of Oculus specific idiosyncrasies, only has default bindings for the Oculus Touch controllers, and will not compile on linux.
Users with the vive, the index, or any other openxr supporting VR headsets are encourage to suggest or contribute bindings for their respective headsets.

Compatibility with general mods should be high, but this has not been thoroughly tested. Shaders have not been tested at all and may spawn spiders inside your headset.

Installation (openmw vr)
------------------------

Installing the VR port is similar to openmw, but you should manually edit settings.cfg under my games/openmw to input your real life height under the VR section so the game can correctly scale you.
Reference VR settings exist in settings-default.cfg.

Installation (OpenXR)
---------------------
If openxr fails to load, it's nonetheless possible your VR headset offers a preview release of openxr.
[This article](https://uploadvr.com/oculus-rift-basic-openxr-support/) from uploadvr explains how to enable the oculus preview on windows.
Note that although the article states you need to opt into public test channels, this is no longer necessary. And you no longer need to write your own json file, you can point to the one present in the oculus' runtime's directory.

Building
--------
The fork should build with the same instructions as building openmw.

Oculus Touch controls
---------------------
[Here](https://imgur.com/a/PNgfLGz) is a simple graphic showing the default bindings for the oculus touch controllers.

Ambiguous buttons souch as Y and B fire the alternative action when you hold instead of tap.

The R-Trigger by default uses the readied tool/spell, but activates objects and actors instead when Pointer mode is active.

While in menus the A and B keys, and the thumbsticks change behaviour to navigate menus. But past the main menu you can navigate using point and click.

Known Issues
------------
- The loading screen does not show any backgrounds
- Movies play upside down
- Cannot point and click in the main menu, you must use the right hand controller as explained above
- Performance is shite, you will likely have to play without fancy shadows or water shaders.