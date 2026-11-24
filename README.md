# MVVM Settings Menu

A game settings menu for Unreal Engine 5.8, built with **CommonUI** and the **UMG Viewmodel (MVVM)** plugin. I'm building it as a hands-on way to learn Unreal's viewmodel workflow and data binding. The goal is a small, polished, fully navigable menu whose logic lives in C++ viewmodels instead of Blueprint.

> **Status:** Work in progress. All four tabs are built and bound to C++ viewmodels. Apply, Revert, and Reset to defaults work, settings are saved to `GameUserSettings.ini` and survive a restart, and backing out with unsaved changes asks for confirmation. Input, localization, and accessibility (Phase 5) are next. See [Roadmap](#roadmap).

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
- **Persistence** through a `UGameUserSettings` subclass: graphics use the engine's own fields, and the other tabs' settings are added as `Config` properties
- **Gamepad, keyboard, and mouse navigation**, with shoulder-button tab switching and input-aware button prompts
- **Localization** through string tables, with a German test language to prove the layout holds up with longer text

## Architecture

The menu follows Model–View–ViewModel:

- **Model:** the saved settings (`USettingsGameUserSettings`, a `UGameUserSettings` subclass saved to `GameUserSettings.ini`).
- **Viewmodels:** C++ classes deriving from `UMVVMViewModelBase`, one per tab, owned by a parent `USettingsViewModel` that tracks unsaved changes. They load from the model and write back only on Apply.
- **Views:** CommonUI widgets that bind to viewmodel properties. Properties are marked `FieldNotify`, so the UI updates automatically when a value changes. Widgets call viewmodel setters when the player changes something and never touch save data directly.

Keeping the logic out of the widgets means the viewmodels can be tested without any UI, and a second view could reuse the same viewmodels.

### Viewmodels

The viewmodels form a three-level tree:

| Level | Classes | Responsibility |
| --- | --- | --- |
| Root | `USettingsViewModel` | Owns the four tab viewmodels and exposes `HasUnsavedChanges`, recomputed whenever any setting changes |
| Tab | `UGraphicsSettingsViewModel`, `UAudioSettingsViewModel`, `UControlsSettingsViewModel`, `UAccessibilitySettingsViewModel` (base: `USettingsTabViewModelBase`) | Creates the tab's settings, exposes its display name and tab ID, and handles dependencies between settings |
| Row | `USliderSettingViewModel`, `UToggleSettingViewModel`, `UOptionSettingViewModel` (base: `USettingViewModelBase`) | One setting: its value, display name, and whether it's editable, plus a baseline used for dirty tracking and revert |

Each tab exposes its settings as individual `FieldNotify` properties, so a tab widget can bind each row widget to one by name. Each row viewmodel compares its current value against its baseline, so moving a slider back to where it started clears the unsaved state again.

Examples of settings that depend on others:

- **Graphics quality preset:** picking a preset sets view distance, shadows, textures, and effects. Changing any one of them switches the preset to *Custom*, unless all four match a level again.
- **Resolution** is locked in Windowed Fullscreen mode.
- **Subtitle background** is only editable while subtitles are on.

### Views

- **`USettingsScreenWidget`** (C++ base of `WBP_SettingsScreen`) is a `UCommonActivatableWidget`. It creates the root viewmodel and hands it to the Blueprint's *Manual* viewmodel slot through `UMVVMView::SetViewModelByClass`. It also requests menu input mode and handles Back.
- **`WBP_SettingsScreen`** registers each tab with `WBP_SettingsTabList` (a `UCommonTabListWidgetBase`), linked to a widget switcher that holds the four tab widgets. Its bindings pass each tab viewmodel down to its tab widget.
- **Tab buttons** are created by the tab list. When each one is created, the screen looks up its viewmodel with `FindTabByNameId`, and the button's label binds to the tab's `DisplayName`.
- **Row widgets** (`WBP_SliderSetting`, `WBP_ToggleSetting`, `WBP_OptionSetting`) each bind to a single row viewmodel. Every tab is built from these three pieces.

_A diagram and a walkthrough of how one setting flows from the save file to the screen will be added once saving is in place._

### Lessons learned so far

- **Order of events:** a widget's MVVM view is created during `UUserWidget::NativeOnInitialized`, and its bindings first run at Construct. Anything in Blueprint that needs the viewmodel or bindings belongs in Event Construct, not On Initialized.
- **Late viewmodels are fine:** setting a *Manual* viewmodel after Construct still works, because MVVM runs that viewmodel's bindings as soon as it's set.
- **`RegisterTab` doesn't place the button:** it creates the button and adds it to the tab list's selection group, but putting it on screen is up to you (in `HandleTabCreation`). Leave `TabIndex` at `-1` to append; any other value inserts the tab and rebuilds the list, which re-registers tabs in an unpredictable order.
- **The tab list clears its tabs on Destruct**, so guard registration with "tab count is 0" rather than a Do Once.

## Getting started

### Requirements

- Unreal Engine 5.8
- Visual Studio 2022 or later with the C++ game development workload (the included `.vsconfig` lists the required components), or JetBrains Rider
- [Git LFS](https://git-lfs.com/) (binary assets such as `.uasset` and `.umap` are stored in LFS)

### Build and run

1. Clone the repository, then run `git lfs pull` if your Git client didn't fetch LFS files automatically.
2. Right-click `MVVM_Settings.uproject` and choose **Generate Visual Studio project files**.
3. Open `MVVM_Settings.sln`, set the configuration to **Development Editor**, and build.
4. Launch the editor from Visual Studio, or open `MVVM_Settings.uproject` directly.

## Project layout

```
Config/                            Engine, game, and editor settings
Content/Maps/                      MainMenu startup map
Content/UI/Input/                  CommonUI input data and input action table
Content/UI/Widgets/                Settings screen, tab list and buttons, tab widgets, row widgets
Source/MVVM_Settings/Settings/     User settings class (the model)
Source/MVVM_Settings/ViewModels/   Root, tab, and row viewmodels
Source/MVVM_Settings/UI/           C++ base classes for widgets
```

Binary assets under `Content/` are stored in Git LFS.

## Roadmap

- [x] **1. Setup:** enable the ModelViewViewModel and CommonUI plugins, add module dependencies, configure `CommonGameViewportClient` and CommonUI input data
- [x] **2. One working binding:** an audio viewmodel with one `FieldNotify` property, bound two-way to a slider
- [x] **3. All four tabs:** per-tab viewmodels, a CommonUI tabbed screen, reusable row widgets, graphics preset logic
- [x] **4. Apply, revert, and save:** persistence and an unsaved-changes confirmation dialog
- [ ] **5. Input, localization, and accessibility:** full navigation, string tables, German test language, text scaling
- [ ] **6. Polish and profile:** transitions and focus states, an Unreal Insights profiling pass, and a short demo video

**Stretch goals:** key remapping with Enhanced Input, a comparison of CommonUI's data-table input actions with its Enhanced Input support, a brightness calibration screen, automated viewmodel tests, and a second compact view bound to the same viewmodels.

## Resources

- [UMG Viewmodel for Unreal Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/umg-viewmodel-for-unreal-engine) (Epic documentation)
- [UMVVMViewModelBase API reference](https://dev.epicgames.com/documentation/unreal-engine/API/Plugins/ModelViewViewModel/UMVVMViewModelBase?lang=en-US)
- [Inside Unreal: Introduction to Common UI](https://forums.unrealengine.com/t/inside-unreal-introduction-to-common-ui/610301)
