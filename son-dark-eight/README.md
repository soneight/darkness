# son-dark-eight README

> Eight color based theme for [VS Code](https://code.visualstudio.com/)
> with primary color being cyan.

## Overview

- Advanced C/C++ support
- Basic HTML support
- Future support for: assembly (x86_64), C#, Python, SQL, TypeScript, Web (CSS, JS)

## Base colors

- Grey
    - colors: disabled elements, line numbers, squigglies, breadcrumbs
    - tokens: comments, namespaces
- White
    - tokens: constant values, operators, quoted strings 
- Yellow
    - colors: active line number
    - tokens: functions, overloaded operators
- Blue
    - tokens: templates
- Green
    - tokens: user defined and built-in types
- Cyan
    - colors: active elements, selection, scrollbars, borders, links
    - tokens: variables, html attributes
- Magenta
    - tokens: rare keywords, escape sequences
- Red
    - colors: error related elements
    - tokens: keywords, html tags

## Styles

- Italics: constant values
- Bold: punctuation, language variables (this, self)
- Strikethrough: pointer access operator (e.g. `->`)

## Notes (for future reference)

- all
    - avoid using `meta` rules
    - `White` for constants values and const modifiers
- cxx (C/C++)
    - `keyword.operator` and `storage.modifier` should have same styles for parsing issues with templates 

###### Color components are only this eight values (in hex): 10 20 40 60 80 A0 C0 E0 (with few exceptions).

