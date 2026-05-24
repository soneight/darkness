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

## Extensions

Token colors adjustments:
  - for official vscode `ms` extension pack for C++ is used
  - for oss vscode version `kylinideteam` extension pack for C++ and `jeff-hykin` better C++ syntax is used

## Base and Token Colors

- White
    - colors: default
    - tokens: constant, operators, quoted strings, types, punctuation
- Grey
    - colors: disabled elements, line numbers, squigglies, breadcrumbs
    - tokens: comment, invalid, namespace related
- Cyan
    - colors: active elements, selection, scrollbars, borders, links, git modified
    - tokens: variable, html attributes
- Magenta
    - colors: underused
    - tokens: string (not quoted), other keywords, escape sequences
- Blue
    - colors: underused
    - tokens: support, templates
- Yellow
    - colors: active line number
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
