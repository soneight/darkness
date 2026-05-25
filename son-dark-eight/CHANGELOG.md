# CHANGELOG
> [keep a changelog](https://keepachangelog.com/en/1.1.0/) with [semantic versioning](https://semver.org/spec/v2.0.0.html)

## [Unreleased]

- TODO:
  - change implicit default colors to explicit ones
  - as for now mostly all colors are in format `X0Y0Z0`, before release such colors can be used as initial value indicator and later should be reviewed and changed to format 'XYZ' (Finalized)
- most colors from versions:
  - `1.49-1.57`
  - `1.58-1.69`
  - `1.70-1.77`
  - `1.78-1.88`
  - `1.88-1.91`
- no colors from version 1.92 and above

- tokenColors: language highlighting overhaul for
    - `as`se`m`bly (x86_64)
    - `c`++
    - Web (`html`, `css`, `js`)

## [0.3.1] - 2026-05-25

### Fixed

- variable token colors (it is messy right now to fix so it is preferably render them as white then falsely apply cyan for anything that parsed as variable-like)

### Finalized

- editor:
  - background (dark grey) and foreground (white) colors
  - line highlight background and border (black)
  - editor cursor foreground (light yellow) and OVR background (dark blue)
  - editor indent guide background (grey) and active (light grey)
  - editor line number foreground (light grey) and active (light yellow)
- git decoration resource
  - ignored (light grey)
  - modified (cyan)
  - staged modified (light blue)
  - conflicting (yellow)
  - added (light green)
  - removed (magenta)
  - staged removed (light red)

### Added

- extension button colors
- "Undertalic" (underline italic) token color style for include headers

### Changed

- lower package minimum required vscode version from 1.88 to 1.64
- debugConsole and debugView colors from default to temporary
- other default colors to temporary

### Removed

- chat colors
- outline colors
- notebook colors
- profile badge colors

## [0.3.0] - 2026-05-22
> first beta-release on open-vsx

### Added

- colors for elements prior to version 1.48 of VS Code

### Removed

- comment about future support for `ts`

## [0.2.2] - 2025-05-10

### Added

- few missing theme options prior to version 1.43 of VS Code

### Removed

- comments about future support for python, csharp and sql

## [0.2.1] - 2025-03-23

### Added

- colors for elements prior to version 1.43 of VS Code

### Changed

- adjust cmake colors for entity, storage and string to be related to C++ colors
- `const` and `let` make white to reflect C/C++
- fix css property value to be white

## [0.1.3] - 2025-02-01

### Added

- add global scopes for Magenta: string, Blue: support, Yellow: entity, Green: storage
- add White: string.quoted
- add White: storage.type.integral (for construct `enum class T : type`)

## [0.1.2] - 2025-01-28

### Changed

> All changes related to C/C++ support (less colorful, more white and grey)

- global `punctuation` `white` instead of `red` except for `punctuation.definition`
- namespace related punctuation become `grey` as namespace itself
- most types become `white` instead of `green`

## [0.1.1] - 2025-01-23

> mostly all changes related to C/C++ support

### Added

- `bold` style for `variable.language`
- `keyword.other` for `Magenta`
- `Cyan` for link scopes (markup and string)
- `White` for enum members (as constants)

### Changed

- `keyword` are global for `Red` color instead of magenta
- `storage.type.template` for `Blue` instead of `storage.modifier` that become `White`
- namespaces related scopes become `Grey` instead of `White`

### Removed

- overuse `bold` style for common scopes

## [0.1.0] - 2025-01-16

### Added

- allegedly all colors for elements introduced prior to version 1.31 of VS Code
- few colors after version 1.30 of VS Code
- advanced tokenColors for C/C++
- basic tokenColors for html
