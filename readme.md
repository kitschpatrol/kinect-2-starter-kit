<!-- title -->

# Kinect 2 Starter Kit

<!-- /title -->

<!-- badges -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/license/mit)

<!-- /badges -->

<!-- short-description -->

**A kit to simplify getting started with the Kinect 2 in openFrameworks.**

<!-- /short-description -->

## Overview

This kit aligns various stars for your convenience. It aggregates the dependencies and patches required to get the Kinect 2 sensor running in [openFrameworks](https://openframeworks.cc).

This project has two use cases in mind:

### A shortcut to Windows-native development

Simplify configuration for developing Windows-only openFrameworks applications that run natively and have full access to the Kinect 2's API. Help developers get up and running very quickly without having to align various repos. (Potentially helpful for workshops, classes, hackathons, etc.)

### A no-fuss bridge to other platforms

Provide access to a subset of the Kinect 2's functionality for any OSC-compatible platform. In this case, skeleton data from the Kinect is streamed over the network via an instance of the starter kit running on a Windows machine. Since a wide range of platforms can consume OSC, this should make it easier to work with the Kinect 2 on a Mac or mobile device.

I'd eventually like to merge this functionality into a more evolved and generic project like [openTSPS](https://github.com/labatrockwell/openTSPS).

## Getting started

> **November 2015 notes:**
>
> I needed to delete and recreate my openFrameworks fork on GitHub, which might have broken this repo.
>
> I've pointed the [submodule to the closest commit](https://github.com/liquidzym/openFrameworks/commit/fa04d0c3cb64f363a8e817509648353ef8758bbf) and included two patch files in [`patches`](patches) to bring the openFrameworks fork associated with this repo into the correct state. I'm not even sure the fork is necessary anymore. I have yet to revise this project for OF 0.9.0 and VS2015, but it seems less necessary now that the Kinect 2 is better supported and there's less fuss involved in getting everything working.
>
> If anyone still feels this repo is useful then [open an issue](https://github.com/kitschpatrol/kinect-2-starter-kit/issues) and I'll make time for an update.

### Dependencies

Hardware:

- A Windows 8.1 PC or a (very) recent Mac running Windows 8.1 via Boot Camp
- The [Kinect for Windows v2 sensor](https://web.archive.org/web/20140608012935/http://www.microsoft.com/en-us/kinectforwindows/Purchase/developer-sku.aspx) (Microsoft [discontinued this sensor](https://en.wikipedia.org/wiki/Kinect) in April 2015 and directed commercial users to the Kinect for Xbox One with a USB adapter instead.)
- A USB 3.0 port (Currently only USB host controllers based on Renesas or Intel chipsets are supported. I've had luck with [this Renesas-based SIIG PCI card](https://web.archive.org/web/20140718062954/http://www.bhphotovideo.com/c/product/968543-REG/siig_ju_p40311_s1_dp_4_port_usb_3_0.html) in combination with the alpha Kinect 2 hardware.)

Software: Windows 8.1, Visual Studio 2013, Git, and the Kinect for Windows SDK 2.0. The installation steps below cover each one.

Two dependencies are included as Git submodules:

- [openFrameworks](https://github.com/liquidzym/openFrameworks) 0.8.0, from liquidzym's fork with Visual Studio 2013 support, pinned to a March 2014 commit
- [ofxKinectV2](https://github.com/kitschpatrol/ofxKinectV2), a fork of Joshua Noble's addon

The OSC examples also use ofxOsc, which comes with openFrameworks.

The code was last changed in June 2014. The project files use the Visual Studio 2013 (v120) platform toolset.

### Installation

1. Install Windows 8.1 (Boot Camp is fine if your Mac is endowed with USB 3. Virtual machines won't work.)

2. Install [Visual Studio 2013 Express](https://visualstudio.microsoft.com/vs/older-downloads/). (Express is free, but the beefier editions also work fine.) Microsoft now requires a Visual Studio subscription to download older versions.

3. Download and install [Git](https://git-scm.com/install/windows).

4. Install the [Kinect for Windows SDK 2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561).

5. Get the repository and submodules. openFrameworks is included. This might take a very long time:

   ```sh
   git clone --recursive https://github.com/kitschpatrol/kinect-2-starter-kit.git
   ```

6. Apply the two patches in [`patches`](patches) to the openFrameworks submodule (see the November 2015 notes above):

   ```sh
   cd kinect-2-starter-kit
   git -C openFrameworks apply ../patches/901348721b5bf3129466c7f21dda33fe0d03340b.patch.txt ../patches/c62bbdd4cc9ed49906be5611b6bcca1540c96a17.patch.txt
   ```

7. Plug in the Kinect and try the example included in ofxKinectV2 to make sure everything works. `\kinect-2-starter-kit\ofAddons\ofxKinectV2\example-Simple\example-Simple.sln`

## Usage

KinectV2OSC sends skeleton data as individual OSC messages to `127.0.0.1` on port `3001` each time the Kinect delivers a new frame. To send to another machine, edit the `sender.setup("127.0.0.1", 3001);` line in `testApp::setup()` in [`ofApps/KinectV2OSC/src/testApp.cpp`](ofApps/KinectV2OSC/src/testApp.cpp).

Each of the six skeleton slots (`0` to `5`) gets a `tracked` message on every frame. Hand and joint messages are only sent for tracked skeletons.

| Address                                         | Arguments                                                                |
| ----------------------------------------------- | ------------------------------------------------------------------------ |
| `/skeletons/<index>/tracked/`                   | String: `yes` or `no`                                                    |
| `/skeletons/<index>/hands/left/`                | String: `unknown`, `nottracked`, `open`, `closed`, or `lasso`            |
| `/skeletons/<index>/hands/right/`               | String: `unknown`, `nottracked`, `open`, `closed`, or `lasso`            |
| `/skeletons/<index>/joints/<joint>/position`    | Three floats: x, y, z                                                    |
| `/skeletons/<index>/joints/<joint>/orientation` | Four floats: the x, y, z, and w components of the orientation quaternion |
| `/skeletons/<index>/joints/<joint>/tracking`    | String: `untracked`, `inferred`, or `tracked`                            |

`<joint>` is one of the 25 Kinect joints: `spinebase`, `spinemid`, `neck`, `head`, `shoulderleft`, `elbowleft`, `wristleft`, `handleft`, `shoulderright`, `elbowright`, `wristright`, `handright`, `hipleft`, `kneeleft`, `ankleleft`, `footleft`, `hipright`, `kneeright`, `ankleright`, `footright`, `spineshoulder`, `handtipleft`, `thumbleft`, `handtipright`, or `thumbright`.

Press <kbd>T</kbd> in the KinectV2OSC window to send a `/test` message (the int `1`, the float `3.5`, the string `hello`, and the elapsed time as a float), or <kbd>B</kbd> to send the same message in a bundle.

## Examples

### KinectV2OSC

A very simple OSC broadcaster with partial coverage of the Kinect 2 API. Open `ofApps\KinectV2OSC\KinectV2OSC.sln`, then build and run it with the Kinect plugged in. It draws the Kinect's color and depth images with the tracked skeletons on top, and sends skeleton data over OSC as described in [Usage](#usage).

### KinectV2OSCClient

A proof-of-concept receiver for the messages from KinectV2OSC. Open `ofApps\KinectV2OSCClient\KinectV2OSCClient.sln`, then build and run it. It listens on port `3001` and draws the joints of each tracked skeleton as circles: green for tracked, yellow for inferred, and red for not tracked. The background turns black when a skeleton's left hand tip is above its head.

The client only needs openFrameworks and ofxOsc, not the Kinect SDK, so it can run on a different Windows machine than the one with the Kinect.

## Updating bundled libraries

openFrameworks and ofxKinectV2 are Git submodules. The openFrameworks submodule points to [liquidzym's fork](https://github.com/liquidzym/openFrameworks). The ofxKinectV2 submodule points to [a fork](https://github.com/kitschpatrol/ofxKinectV2) of [Joshua Noble's addon](https://github.com/joshuajnoble/ofxKinectV2) with changes that let the addon live outside the openFrameworks directory and that add joint tracking state and hand state to skeletons.

The two patches in [`patches`](patches) were exported with `git format-patch` from the openFrameworks fork this repo used until November 2015:

- `901348721b5bf3129466c7f21dda33fe0d03340b.patch.txt` renames oscpack's internal `NetworkInitializer` class to `OSCNetworkInitializer` to avoid a linker conflict with POCO.
- `c62bbdd4cc9ed49906be5611b6bcca1540c96a17.patch.txt` comments out POCO's "Unsupported Windows version" error.

## Background

### Implementation notes

This repository uses a structure that might be unfamiliar to openFrameworks users. Due to the specificity of the various dependencies, both openFrameworks and the relevant addons are embedded as Git submodules.

To avoid issues with nesting files that belong in the parent repository inside submodules, examples and addons are stored _outside_ the root `/openFrameworks` directory. Paths and linkages have been updated accordingly, but if you try to drag one of the projects in `/ofApps` or `/ofAddons` into their usual locations inside the `/openFrameworks` directory, they will break.

The client's [`KinectTypes.h`](ofApps/KinectV2OSCClient/src/KinectTypes.h) copies a subset of the types from the Kinect SDK's `Kinect.h`, so the client builds without the SDK.

### Known issues

- The openFrameworks submodule is pinned to commit `fa04d0c` in liquidzym's fork, which is no longer on any branch of that fork. GitHub still serves the commit by its hash.
- The code predates the public release of the Kinect for Windows SDK 2.0. The KinectV2OSC project file lists the SDK 2.0 Developer Preview's install path (`C:\Program Files\Microsoft SDKs\Kinect\v2.0-DevPreview1404`) alongside the `KINECTSDK20_DIR` environment variable.

## Maintainers

[kitschpatrol](https://github.com/kitschpatrol)

## Acknowledgments

This repository just glues together a bunch of other people's great work.

- [liquidzym](https://github.com/liquidzym) added VS2013 compatibility to OF
- [Joshua Noble](https://github.com/joshuajnoble) wrote the [KinectV2 plugin](https://github.com/joshuajnoble/ofxKinectV2)

<!-- contributing -->

## Contributing

[Issues](https://github.com/kitschpatrol/kinect-2-starter-kit/issues) are welcome and appreciated.

Please open an issue to discuss changes before submitting a pull request. Unsolicited PRs (especially AI-generated ones) are unlikely to be merged.

This repository uses [MDAT](https://github.com/kitschpatrol/mdat) for readme placeholder expansion.

<!-- /contributing -->

<!-- license -->

## License

[MIT](license.txt) © [Eric Mika](https://ericmika.com)

<!-- /license -->

The MIT license covers the two apps in [`ofApps`](ofApps). The client's [`KinectTypes.h`](ofApps/KinectV2OSCClient/src/KinectTypes.h) includes type definitions copied from the Kinect for Windows SDK's `Kinect.h` header, and the `Kv2Joint` and `Kv2Skeleton` classes from ofxKinectV2.
