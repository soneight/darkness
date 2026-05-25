# SON-DARK-EIGHT

> Eight color based theme for [VS Code](https://code.visualstudio.com/) with primary color being cyan

## Overview

- colors was chosen based on low brightness screen settings
- Advanced C/C++ support
- Basic Web support (HTML, CSS, JS)
- Future support for: assembly (x86_64)

## Not supported (yet?)

- colors added above vscode version 1.92
- most colors added above vscode version 1.48
- notebook related colors
- no "default" colors until vscode minimal version requirement is below 1.96
- outline color options

## Extensions

Token colors adjustments:
  - for official vscode `ms` extension pack for C++ is used
  - for oss vscode version `kylinideteam` extension pack for C++ and `jeff-hykin` better C++ syntax is used

## Base and Token Colors

- White
    - colors: default
    - tokens: constant and punctuation, operators, quoted strings, types
- Grey
    - colors: disabled elements, line numbers, squigglies, breadcrumbs
    - tokens: comment and invalid, namespace related
- Cyan
    - colors: active elements, selection, scrollbars, borders, links, git modified
    - tokens: variable, html attributes
- Magenta
    - colors: underused
    - tokens: string (not quoted), other keywords, escape sequences
- Blue
    - colors: editor cursor OVR background
    - tokens: support, template typename
- Yellow
    - colors: active line number, editor cursor
    - tokens: entity, functions, overloaded operators
- Green
    - colors: git added
    - tokens: storage
- Red
    - colors: error related elements, debug icons
    - tokens: keyword, html tags

## Styles

- Italics: constant values
- Bold: punctuation, language variables (this, self)
- Strike-through: pointer access operator (e.g. `->`)

## Notes

- all
    - avoid using `meta` rules
    - `White` for constants values and const modifiers
    - `White` should be most common color, other colors should be used sparingly
    - `Bold` style for punctuation

- cxx (C/C++)
    - `keyword.operator` and `storage.modifier` should have same styles because of parsing issues with templates

###### Color components are only this eight values: 10 20 40 60 80 A0 C0 E0 (with few exceptions).
