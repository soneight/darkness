# Changelog

All notable changes to the "son-dark-eight" extension will be documented in this file.

Check [Keep a Changelog](https://keepachangelog.com/) for recommendations on how to structure this file.

## [Unreleased]

- tokenColors: language highlighting overhaul for 
    - `as`se`m`bly (x86_64)
    - `c`++
    - `cs`harp
    - `py`thon
    - `sql`
    - `t`ype`s`cript
    - Web (`html`, `css`, `js`)

## [0.1.3] - 2025-02-01

### Added

- add global scopes for Magenta: string, Blue: support, Yellow: entity, Green: storage
- add White: string.quoted
- add White: storage.type.integral (for construct `enum class T : type`)

## [0.1.2] - 2025-01-28

### Changed

> All changes related to C/C++ support (less colorfull, more white and grey)

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