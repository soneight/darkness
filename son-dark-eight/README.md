# son-dark-eight README

> Eight color based theme for [VS Code](https://code.visualstudio.com/)
> with primary color being cyan.

## Overview

- Advanced C/C++ support
- Basic Web support (HTML, CSS, JS)
- Future support for: assembly (x86_64), TypeScript

## Base colors

- White
    - colors: default
    - tokens: constant values, operators, quoted strings, types, punctuation 
- Grey
    - colors: disabled elements, line numbers, squigglies, breadcrumbs
    - tokens: comments, invalid, namespace related
- Cyan
    - colors: active elements, selection, scrollbars, borders, links, git modified
    - tokens: variables, html attributes
- Magenta
    - colors: underused
    - tokens: not quoted string, other keywords, escape sequences
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
    - tokens: keywords, html tags

## Styles

- Italics: constant values
- Bold: punctuation, language variables (this, self)
- Strikethrough: pointer access operator (e.g. `->`)

## Notes (for future reference)

- all
    - avoid using `meta` rules
    - `White` for constants values and const modifiers
    - `White` should be most common color, other colors should be used sparingly
    - `Bold` style for punctuation

- cxx (C/C++)
    - `keyword.operator` and `storage.modifier` should have same styles for parsing issues with templates 

###### Color components are only this eight values (in hex): 10 20 40 60 80 A0 C0 E0 (with few exceptions).
