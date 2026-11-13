# MVVM Settings Menu

A game settings menu for Unreal Engine 5.8, built with **CommonUI** and the **UMG Viewmodel (MVVM)** plugin. I'm building it as a hands-on way to learn Unreal's viewmodel workflow and data binding. The goal is a small, polished, fully navigable menu whose logic lives in C++ viewmodels instead of Blueprint.

> **Status:** Early work in progress. The project skeleton is in place; plugin setup (Phase 1) is next. See [Roadmap](#roadmap).

## What it will include

A pause-menu style settings screen with four tabs:

| Tab | Settings | What it exercises |
| --- | --- | --- |
| Graphics | Window mode, resolution, quality preset, frame-rate limit, VSync | Option lists and dependent values (a preset changes the individual settings) |
| Audio | Master, music, effects, and dialogue volume | Two-way slider binding with live preview |
| Controls | Look sensitivity, invert Y, vibration | Bools and floats; gamepad-only settings |
| Accessibility | Text size, subtitles, subtitle background, colorblind mode | Text scaling that reflows the whole menu |

Plus:

- **Apply, Revert, and Reset to defaults**, with a "discard changes?" prompt when backing out with unsaved edits
- **Persistence** through `UGameUserSettings` (graphics) and a save object for everything else
- **Gamepad, keyboard, and mouse navigation**, with shoulder-button tab switching and input-aware button prompts
- **Localization** through string tables, with a German test language to prove the layout holds up with longer text

## Architecture

The menu follows Model–View–ViewModel:

- **Model:** the saved settings (`UGameUserSettings` and a save object).
- **Viewmodels:** C++ classes deriving from `UMVVMViewModelBase`, one per tab, owned by a parent `USettingsViewModel` that tracks unsaved changes. They load from the model and write back only on Apply.
- **Views:** CommonUI widgets that bind to viewmodel properties. Properties are marked `FieldNotify`, so the UI updates automatically when a value changes. Widgets call viewmodel setters when the player changes something and never touch save data directly.

Keeping the logic out of the widgets means the viewmodels can be tested without any UI, and a second view could reuse the same viewmodels.

_A diagram and a walkthrough of how one setting flows from the save file to the screen will be added as the project takes shape._

## Getting started

### Requirements

- Unreal Engine 5.8
- Visual Studio 2022 or later with the C++ game development workload (the included `.vsconfig` lists the required components)
- [Git LFS](https://git-lfs.com/) (binary assets such as `.uasset` and `.umap` are stored in LFS)

### Build and run

1. Clone the repository, then run `git lfs pull` if your Git client didn't fetch LFS files automatically.
2. Right-click `MVVM_Settings.uproject` and choose **Generate Visual Studio project files**.
3. Open `MVVM_Settings.sln`, set the configuration to **Development Editor**, and build.
4. Launch the editor from Visual Studio, or open `MVVM_Settings.uproject` directly.

## Project layout

```
Config/                 Engine, game, and editor settings
Content/                Widgets, input data, string tables (Git LFS)
Source/MVVM_Settings/   C++ game module (viewmodels and supporting code)
```

## Roadmap

- [ ] **1. Setup:** enable the ModelViewViewModel and CommonUI plugins, add module dependencies, configure `CommonGameViewportClient` and CommonUI input data
- [ ] **2. One working binding:** an audio viewmodel with one `FieldNotify` property, bound two-way to a slider
- [ ] **3. All four tabs:** per-tab viewmodels, a CommonUI tabbed screen, reusable row widgets, graphics preset logic
- [ ] **4. Apply, revert, and save:** persistence and an unsaved-changes confirmation dialog
- [ ] **5. Input, localization, and accessibility:** full navigation, string tables, German test language, text scaling
- [ ] **6. Polish and profile:** transitions and focus states, an Unreal Insights profiling pass, and a short demo video

**Stretch goals:** key remapping with Enhanced Input, a brightness calibration screen, automated viewmodel tests, and a second compact view bound to the same viewmodels.

## Resources

- [UMG Viewmodel for Unreal Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/umg-viewmodel-for-unreal-engine) (Epic documentation)
- [UMVVMViewModelBase API reference](https://dev.epicgames.com/documentation/unreal-engine/API/Plugins/ModelViewViewModel/UMVVMViewModelBase?lang=en-US)
- [Inside Unreal: Introduction to Common UI](https://forums.unrealengine.com/t/inside-unreal-introduction-to-common-ui/610301)
