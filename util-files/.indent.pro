// SPDX-License-Identifier: GPL-3.0
// Made by myself, seems like it works nicely when combined with clang-format (clang-format is still needed, there's a lot of bad things that `indent` will leave as-is without correcting them, and there are some minor things that indent does wrong). Use with `indent`, with .indent.pro being placed either in the same directory as the directory you execute `indent` in, with a path to this file in `INDENT_PROFILE` or with .indent.pro being placed in your home directory
--k-and-r-style
--break-after-boolean-operator
--no-space-after-casts
--line-length80
--comment-line-length80
--dont-line-up-parentheses
--remove-preprocessor-space
--no-tabs
--no-blank-lines-after-declarations
--dont-break-procedure-type
