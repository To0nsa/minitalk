# Doxygen Style Guide for `minitalk`

This guide defines how to write consistent, clean, and useful Doxygen documentation for all `.h` and `.c` files in the `minitalk` project.

Following this guide helps:

- Keep documentation readable and structured
- Generate clean HTML/PDF output with Doxygen
- Improve code discoverability and collaboration

---

## 1. File Header Format (`@file`)

Each `.h` and `.c` file should start with a Doxygen `@file` block, **before** anything else.

```c
/**
 * @file ft_array.h
 * @author Toonsa
 * @date YYYY/MM/DD
 * @brief Brief description of this file's purpose.
 *
 * @details
 * Optional longer description of what the file does,
 * what it implements, and any special notes.
 * 
 * @note Optional informations about common features of the 
 * content of the file (ex. "- The `content` pointer in each
 * node can point to any type of data." for linked list)
 *
 * @ingroup group_name
 */
```

- `@file` => Names the file
- `@brief` => One or two-line summary
- A blank line, then `@details` block if needed
- A blank line, then optionally `@note` block if needed
- `@ingroup` => Ties this file to an existing group (optional if not relevant)

---

## 2. Groups and `@defgroup` Template

Groups categorize related functions or types under a label like array_utils, math_utils, etc.

```c
/**
 * @defgroup array_utils Array Utilities
 * @brief Functions for working with pointer and integer arrays.
 * 
 * @details
 * Longer description and more details on the group
 * 
 * @note
 * Specific notes about what the user should be aware of.
 * 
 * @{
 */

/** @} */ // end of array_utils group
```

Place this in the header file that introduces those functions. Then, inside .c files, reference it with `@ingroup` array_utils.

### Grouping guidelines

- One group per .h with @defgroup
- Use `@{ ... @}` to wrap the grouped declarations
- Use `@ingroup` in .c to connect the function doc to the group
- It’s okay to define the group in .h and mention it in .c, Doxygen merges them
- Don’t define the same group in multiple places

---

## 3. Function Documentation Template

**In Headers (.h)**
Keep function doc short: @brief, plus param/return, @note if needed.

```c
/**
 * @brief One-liner summary of what this function does.
 *
 * @note Any caveats, NULL-safety, or usage context.
 * 
 * @param [name] Description of the parameter.
 * @return Description of the return value.
 *
 * @see Related functions
 */
```

**In Source Files (.c)**
Provide details, edge cases, notes, etc.:

```c
/**
 * @brief One-liner summary of what this function does.
 *
 * @details
 * Longer explanation. Include:
 * - Expected behavior
 * - Special edge cases
 * - Internal logic if helpful
 * - Any algorithm insights
 * 
 * @note Additional usage contexts or disclaimers.
 *
 * @param [name] Description of the parameter.
 * @return Description of the return value.
 *
 * @see Related functions
 * @ingroup group_name
 */
```

---

## 4. Struct and Typedef Documentation

Use `@typedef` and `@brief` when documenting public structs. Example:

```c
/**
 * @typedef t_point
 * @brief A 2D point with integer coordinates.
 *
 * @details
 * Contains fields:
 * - x: integer X coordinate
 * - y: integer Y coordinate
 */
typedef struct s_point {
    int x; ///< X coordinate
    int y; ///< Y coordinate
} t_point;
```

- If documenting fields, prefer ///< inline with the field.
- Always annotate typedef or struct doc in the group or module if relevant.

---

## 5. Internal/Static Functions

- Document non-trivial static functions in .c files with `@internal` if you want them hidden from public docs.
- Skip documentation for trivial helpers.

```c
/**
 * @internal
 * @brief Internal helper to clamp a value.
 */
static int clamp_internal(int value, int min, int max);
```

---

## 6. Doxygen Tags Reference

```text
Tag             Description                                          Use In
-------------------------------------------------------------------------------
@file           Describes the file                                  Top of .c / .h
@brief          Short function or file summary                      Functions / files
@details        Optional full description                           Functions / files
@param          Describes each function parameter                   Functions
@return         Describes the return value                          Functions
@note           Extra notes (ex. "Safe to call with NULL.")         Optional
@see            References to related functions                     Optional
@ingroup        Assigns function to a documentation group           Required in .c files
@defgroup       Defines a new documentation group                   In .h headers
@addtogroup     Groups related functions in .c files                Optional
@{ @}           Marks start/end of a group block                    Optional
@internal       Marks internal/private code                         .c only
```

---

## 7. General Style

- Use full sentences with punctuation.
- Prefer present tense.
- Do not document implementation details in header files.
- All `@param` entries must be aligned and ordered as in the function signature.

---

## 8. Block-Specific Rules

- The `@brief` must be a concise summary of one or two lines maximum.  
  - Avoid multi-line descriptions — use `@details` instead.

- The `@details` tag must appear **alone** on its own line, immediately after a **blank line** following `@brief`.

- Always use a `@details` block in:
  - `@file` documentation
  - `@typedef` blocks
  - Any function block where the explanation is more than one sentence

- For `@file` and `@typedef` blocks, always include:
  - A `@brief` line summary
  - A blank line
  - A `@details` block for context, usage, or structure explanation

- Do **not** add `@param`, `@return`, or `@ingroup` in `@typedef` blocks unless absolutely necessary.

---

## 9. Tag Usage and Semantics

- Use `@note` for additional information (e.g. “Safe to call with NULL.”).
- Use `@warning` for probable misuse or edge cases.
- Each `@defgroup` must include its relevant `typedef`s, structs, and core definitions.

## 10. Using Doxyfile

- Generate a defaul config:

```bash
doxygen -g Doxyfile
```

- Replace or fill its content withe one below.

- Run:

```bash
doxygen Doxyfile
```

- Place output in /docs/ → GitHub Pages will detect it.
- Enable GitHub Pages in your repo settings → docs/ branch or folder.

### Doxyfile recommended settings for GitHubPage

```ini
#----------------------------------
# Project Info
#----------------------------------
PROJECT_NAME           = "minitalk"
PROJECT_BRIEF          = "..."
PROJECT_LOGO           =
OUTPUT_DIRECTORY       = docs
INPUT                  = inc/ srcs/ README.md
RECURSIVE              = YES
FILE_PATTERNS          = *.h *.c
EXCLUDE_PATTERNS       = */.git/*

#----------------------------------
# Build Configuration
#----------------------------------
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = YES
EXTRACT_STATIC         = YES
EXTRACT_LOCAL_CLASSES  = YES
OPTIMIZE_OUTPUT_FOR_C  = YES
EXTENSION_MAPPING      = c=C

#----------------------------------
# Member Grouping and Sections
#----------------------------------
SORT_MEMBER_DOCS       = YES
SORT_BRIEF_DOCS        = YES
SORT_BY_SCOPE_NAME     = YES
SHOW_USED_FILES        = YES
SHOW_FILES             = YES
SHOW_INCLUDE_FILES     = YES
SHOW_GROUPED_MEMB_INC  = YES
INLINE_GROUPED_CLASSES = YES

#----------------------------------
# Output Options
#----------------------------------
GENERATE_HTML          = YES
HTML_OUTPUT            = html
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100
HTML_COLORSTYLE_GAMMA  = 80
HTML_DYNAMIC_SECTIONS  = YES

#----------------------------------
# Warnings and Cleanliness
#----------------------------------
WARN_IF_UNDOCUMENTED   = YES
WARN_IF_DOC_ERROR      = YES
WARN_NO_PARAMDOC       = YES
QUIET                  = NO
WARNINGS               = YES

#----------------------------------
# GitHub Pages Friendly Output
#----------------------------------
GENERATE_LATEX         = NO
GENERATE_MAN           = NO
GENERATE_RTF           = NO
GENERATE_XML           = NO
USE_MDFILE_AS_MAINPAGE = README.md
MARKDOWN_SUPPORT       = YES
```
