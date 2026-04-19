# PlantUML and Graphviz Install Guide for Windows

This guide installs PlantUML, connects it to Graphviz, adds both tools to `PATH`, and documents the PlantUML command-line options available from the local installation.

The local setup used while preparing this guide reported:

```text
PlantUML version 1.2026.2
Graphviz dot version 14.1.5
Java runtime: OpenJDK 64-bit
```

## Contents

- [1. Prerequisites](#1-prerequisites)
- [2. Create the PlantUML Folder](#2-create-the-plantuml-folder)
- [3. Download PlantUML](#3-download-plantuml)
- [4. Download and Install Graphviz](#4-download-and-install-graphviz)
- [5. Create the PlantUML Command Wrapper](#5-create-the-plantuml-command-wrapper)
- [6. Add PlantUML and Graphviz to PATH](#6-add-plantuml-and-graphviz-to-path)
- [7. Reopen PowerShell](#7-reopen-powershell)
- [8. Verify the Installation](#8-verify-the-installation)
  - [Verify Java](#verify-java)
  - [Verify Graphviz](#verify-graphviz)
  - [Verify PlantUML](#verify-plantuml)
  - [Verify PlantUML and Graphviz Integration](#verify-plantuml-and-graphviz-integration)
- [9. Render a Diagram](#9-render-a-diagram)
- [10. Basic Usage](#10-basic-usage)
- [11. PlantUML Command Reference](#11-plantuml-command-reference)
  - [Wildcards](#wildcards)
  - [General Commands](#general-commands)
  - [Input and Preprocessing Commands](#input-and-preprocessing-commands)
  - [Execution Control Commands](#execution-control-commands)
  - [Metadata and Asset Commands](#metadata-and-asset-commands)
  - [Output Control Commands](#output-control-commands)
  - [Output Format Commands](#output-format-commands)
  - [Statistics Commands](#statistics-commands)
- [12. Common Workflows](#12-common-workflows)
  - [Render All Diagrams to SVG](#render-all-diagrams-to-svg)
  - [Validate All Diagrams for CI](#validate-all-diagrams-for-ci)
  - [Render Only When Source Changed](#render-only-when-source-changed)
  - [Render to a Clean Output Folder](#render-to-a-clean-output-folder)
  - [Generate Preprocessed Source](#generate-preprocessed-source)
  - [Extract Source from a Generated Diagram](#extract-source-from-a-generated-diagram)
- [13. Exit Codes](#13-exit-codes)
- [14. Troubleshooting](#14-troubleshooting)
  - [`plantuml` Is Not Recognized](#plantuml-is-not-recognized)
  - [`dot` Is Not Recognized](#dot-is-not-recognized)
  - [PlantUML Cannot Find Graphviz](#plantuml-cannot-find-graphviz)
  - [Diagram Rendering Is Slow or Times Out](#diagram-rendering-is-slow-or-times-out)

## 1. Prerequisites

Install or confirm Java before running PlantUML:

```powershell
java --version
```

If Java is installed correctly, PowerShell prints the installed Java version.

[Back to Contents](#contents)

## 2. Create the PlantUML Folder

Create this folder:

```text
C:\Apps\PlantUML
```

You can create it from PowerShell:

```powershell
New-Item -ItemType Directory -Force -Path "C:\Apps\PlantUML"
```

[Back to Contents](#contents)

## 3. Download PlantUML

Open the PlantUML download page:

```text
https://plantuml.com/download
```

Download:

```text
LGPL -> Compiled jar
```

Save the file as:

```text
C:\Apps\PlantUML\plantuml.jar
```

[Back to Contents](#contents)

## 4. Download and Install Graphviz

Open the Graphviz download page:

```text
https://graphviz.org/download/
```

Download the Windows 64-bit installer. For example:

```text
graphviz-14.1.5 (64-bit) EXE installer
```

Install it normally. The installer usually installs Graphviz here:

```text
C:\Program Files\Graphviz
```

That location is fine. You do not need to force Graphviz into `C:\Apps\Graphviz`.

After installation, confirm that this file exists:

```text
C:\Program Files\Graphviz\bin\dot.exe
```

[Back to Contents](#contents)

## 5. Create the PlantUML Command Wrapper

Create this file:

```text
C:\Apps\PlantUML\plantuml.bat
```

If Graphviz is installed under `C:\Program Files\Graphviz`, use this content:

```bat
@echo off
java -DGRAPHVIZ_DOT="C:\Program Files\Graphviz\bin\dot.exe" -jar "C:\Apps\PlantUML\plantuml.jar" %*
```

Alternative wrapper using PlantUML's `--dot-path` option:

```bat
@echo off
java -jar "C:\Apps\PlantUML\plantuml.jar" --dot-path "C:\Program Files\Graphviz\bin\dot.exe" %*
```

Use only one wrapper version.

[Back to Contents](#contents)

## 6. Add PlantUML and Graphviz to PATH

Open Environment Variables:

1. Open the Start menu.
2. Search for `Environment Variables`.
3. Open `Edit the system environment variables`.
4. Click `Environment Variables`.
5. Under `System variables`, select `Path`.
6. Click `Edit`.
7. Click `New`.
8. Add:

```text
C:\Apps\PlantUML
```

9. Click `New`.
10. Add the Graphviz bin path:

```text
C:\Program Files\Graphviz\bin
```

If you installed Graphviz somewhere else, add that installation's `bin` folder instead, for example:

```text
C:\Apps\Graphviz\bin
```

11. Click `OK` on all windows.

[Back to Contents](#contents)

## 7. Reopen PowerShell

Close the current PowerShell window and open a new one.

This is required because PowerShell reads `PATH` when the shell starts.

[Back to Contents](#contents)

## 8. Verify the Installation

### Verify Java

```powershell
java --version
```

[Back to Contents](#contents)

### Verify Graphviz

```powershell
dot -V
```

Expected output is similar to:

```text
dot - graphviz version ...
```

[Back to Contents](#contents)

### Verify PlantUML

```powershell
plantuml --version
```

or:

```powershell
plantuml -version
```

[Back to Contents](#contents)

### Verify PlantUML and Graphviz Integration

Preferred current command:

```powershell
plantuml --check-graphviz
```

Compatibility command:

```powershell
plantuml -testdot
```

Expected result:

```text
Installation seems OK. File generation OK
```

[Back to Contents](#contents)

## 9. Render a Diagram

Render this sprint diagram:

```powershell
plantuml "C:\path\to\architecture.puml"
```

Expected output:

```text
C:\path\to\architecture.png
```

[Back to Contents](#contents)

## 10. Basic Usage

Render one `.puml` file to PNG:

```powershell
plantuml diagram.puml
```

Render every `.puml` file in a folder:

```powershell
plantuml "C:\path\to\diagrams"
```

Render recursively using a wildcard:

```powershell
plantuml "**/*.puml"
```

Render to SVG:

```powershell
plantuml --svg diagram.puml
```

Render to PDF:

```powershell
plantuml --pdf diagram.puml
```

Write output to a separate folder:

```powershell
plantuml --output-dir out diagram.puml
```

Check syntax without generating files:

```powershell
plantuml --check-syntax diagram.puml
```

[Back to Contents](#contents)

## 11. PlantUML Command Reference

The commands below are based on the local `plantuml --help-more` output.

General usage:

```text
java -jar plantuml.jar [options] [file|dir]...
java -jar plantuml.jar [options] --gui
```

When using the wrapper from this guide, replace `java -jar plantuml.jar` with:

```text
plantuml
```

[Back to Contents](#contents)

### Wildcards

| Pattern | Meaning |
| --- | --- |
| `*` | Any characters except `/` and `\` |
| `?` | Exactly one character except `/` and `\` |
| `**` | Any characters across directories, recursively |

Quote wildcard patterns in PowerShell:

```powershell
plantuml "**/*.puml"
```

[Back to Contents](#contents)

### General Commands

| Command | Description |
| --- | --- |
| `--author` | Show information about PlantUML authors |
| `--check-graphviz` | Check Graphviz installation |
| `--dark-mode` | Render diagrams in dark mode |
| `--duration` | Print total processing time |
| `--gui` | Launch the graphical user interface |
| `-h`, `--help` | Show help and usage information |
| `--help-more` | Show extended help with advanced options |
| `--http-server[:<port>]` | Start the internal HTTP server for rendering; default port is `8080` |
| `--progress-bar` | Show a textual progress bar |
| `--splash-screen` | Show splash screen with progress bar |
| `-v`, `--verbose` | Enable verbose logging |
| `--version` | Show PlantUML and Java version |

Examples:

```powershell
plantuml --version
plantuml --help
plantuml --help-more
plantuml --check-graphviz
plantuml --dark-mode diagram.puml
plantuml --duration diagram.puml
plantuml --verbose diagram.puml
plantuml --http-server
plantuml --http-server:9090
plantuml --gui
```

Compatibility commands used by older PlantUML examples:

| Command | Current equivalent | Description |
| --- | --- | --- |
| `-version` | `--version` | Show PlantUML and Java version |
| `-testdot` | `--check-graphviz` | Check Graphviz installation |

[Back to Contents](#contents)

### Input and Preprocessing Commands

| Command | Description |
| --- | --- |
| `--charset <name>` | Use a specific input charset |
| `--config <file>` | Specify a configuration file |
| `-d`, `--define <VAR>=<value>` | Define a preprocessing variable, equivalent to `!define <var> <value>` |
| `--exclude <pattern>` | Exclude input files matching the given pattern |
| `-I`, `--include <file>` | Include an external file, equivalent to `!include <file>` |
| `-p`, `--pipe` | Read source from stdin and write the result to stdout |
| `-P`, `--pragma <key>=<value>` | Set a pragma, equivalent to `!pragma <key> <value>` |
| `--skinparam <key>=<value>` | Set a skin parameter, equivalent to `skinparam <key> <value>` |
| `--theme <name>` | Apply a theme |

Examples:

```powershell
plantuml --charset UTF-8 diagram.puml
plantuml --config plantuml.config diagram.puml
plantuml --define ENV=prod diagram.puml
plantuml -DENV=prod diagram.puml
plantuml --exclude "*draft*" "**/*.puml"
plantuml --include common.puml diagram.puml
plantuml --pragma layout=smetana diagram.puml
plantuml --skinparam backgroundColor=white diagram.puml
plantuml --theme cerulean diagram.puml
```

Pipe example in PowerShell:

```powershell
Get-Content diagram.puml | plantuml --svg --pipe > diagram.svg
```

[Back to Contents](#contents)

### Execution Control Commands

| Command | Description |
| --- | --- |
| `--check-before-run` | Pre-check syntax of all inputs and stop faster on error |
| `--check-syntax` | Check diagram syntax without generating images |
| `--graphviz-timeout <seconds>` | Set Graphviz processing timeout in seconds |
| `--ignore-startuml-filename` | Ignore `@startuml <name>` and always derive output filenames from input files |
| `--no-error-image` | Do not generate error images for diagrams with syntax errors |
| `--stop-on-error` | Stop at the first syntax error |
| `--threads <n|auto>` | Use a specific number of threads; `auto` uses available processors |

Examples:

```powershell
plantuml --check-syntax diagram.puml
plantuml --check-before-run "**/*.puml"
plantuml --stop-on-error "**/*.puml"
plantuml --no-error-image diagram.puml
plantuml --graphviz-timeout 120 diagram.puml
plantuml --threads auto "**/*.puml"
plantuml --threads 4 "**/*.puml"
plantuml --ignore-startuml-filename diagram.puml
```

[Back to Contents](#contents)

### Metadata and Asset Commands

| Command | Description |
| --- | --- |
| `--decode-url <string>` | Decode a PlantUML encoded URL back to its source |
| `--disable-metadata` | Do not include metadata in generated files |
| `--dot-path <path-to-dot-exe>` | Specify the path to the Graphviz `dot` executable |
| `--encode-url` | Generate an encoded PlantUML URL from a source file |
| `--extract-source` | Extract embedded PlantUML source from PNG or SVG metadata |
| `--ftp-server` | Start a local FTP server for diagram rendering; rarely used |
| `--list-keywords` | Print the list of PlantUML language keywords |
| `--skip-fresh` | Skip PNG or SVG files that are already up to date using metadata |
| `--sprite <4|8|16[z]> <file>` | Encode a sprite definition from an image file |

Examples:

```powershell
plantuml --dot-path "C:\Program Files\Graphviz\bin\dot.exe" diagram.puml
plantuml --disable-metadata diagram.puml
plantuml --extract-source diagram.png
plantuml --encode-url diagram.puml
plantuml --decode-url "SoWkIImgAStDuNBAJrBGjLDmibBmICt9oUS2"
plantuml --list-keywords
plantuml --skip-fresh "**/*.puml"
plantuml --sprite 16z icon.png
plantuml --ftp-server
```

[Back to Contents](#contents)

### Output Control Commands

| Command | Description |
| --- | --- |
| `--output-dir <dir>` | Generate output files in the specified directory |
| `--overwrite` | Allow overwriting read-only output files |

Examples:

```powershell
plantuml --output-dir out diagram.puml
plantuml --overwrite diagram.puml
```

[Back to Contents](#contents)

### Output Format Commands

Use either the direct format command or `--format <name>`.

| Command | Equivalent | Description |
| --- | --- | --- |
| `--eps` | `--format eps` | Generate EPS output |
| `--html` | `--format html` | Generate HTML files for class diagrams |
| `--latex` | `--format latex` | Generate LaTeX/TikZ output |
| `--latex-nopreamble` | `--format latex-nopreamble` | Generate LaTeX/TikZ output without preamble |
| `--obfuscate` | `--format obfuscate` | Replace text in diagrams with obfuscated strings |
| `--pdf` | `--format pdf` | Generate PDF output |
| `--png` | `--format png` | Generate PNG output; this is the default |
| `--preproc` | `--format preproc` | Generate preprocessed source after applying includes and defines |
| `--scxml` | `--format scxml` | Generate SCXML files for state diagrams |
| `--svg` | `--format svg` | Generate SVG output |
| `--txt` | `--format txt` | Generate ASCII art diagrams |
| `--utxt` | `--format utxt` | Generate Unicode ASCII art diagrams |
| `--vdx` | `--format vdx` | Generate VDX files |
| `--xmi` | `--format xmi` | Generate XMI files for class diagrams |

Examples:

```powershell
plantuml --png diagram.puml
plantuml --svg diagram.puml
plantuml --pdf diagram.puml
plantuml --eps diagram.puml
plantuml --latex diagram.puml
plantuml --latex-nopreamble diagram.puml
plantuml --txt diagram.puml
plantuml --utxt diagram.puml
plantuml --preproc diagram.puml
plantuml --obfuscate diagram.puml
plantuml --format svg diagram.puml
plantuml --format pdf --output-dir out diagram.puml
```

[Back to Contents](#contents)

### Statistics Commands

| Command | Description |
| --- | --- |
| `--disable-stats` | Disable statistics collection; this is the default behavior |
| `--enable-stats` | Enable statistics collection |
| `--export-stats` | Export collected statistics to a text report and exit |
| `--export-stats-html` | Export collected statistics to an HTML report and exit |
| `--html-stats` | Output general statistics in HTML format |
| `--loop-stats` | Continuously print usage statistics during execution |
| `--realtime-stats` | Generate statistics in real time during processing |
| `--xml-stats` | Output general statistics in XML format |

Examples:

```powershell
plantuml --enable-stats diagram.puml
plantuml --export-stats
plantuml --export-stats-html
plantuml --html-stats
plantuml --xml-stats
plantuml --loop-stats diagram.puml
plantuml --realtime-stats diagram.puml
plantuml --disable-stats diagram.puml
```

[Back to Contents](#contents)

## 12. Common Workflows

### Render All Diagrams to SVG

```powershell
plantuml --svg "**/*.puml"
```

[Back to Contents](#contents)

### Validate All Diagrams for CI

```powershell
plantuml --check-syntax --stop-on-error "**/*.puml"
```

[Back to Contents](#contents)

### Render Only When Source Changed

```powershell
plantuml --skip-fresh "**/*.puml"
```

[Back to Contents](#contents)

### Render to a Clean Output Folder

```powershell
plantuml --output-dir "out" "**/*.puml"
```

[Back to Contents](#contents)

### Generate Preprocessed Source

Use this when debugging `!include`, `!define`, variables, or themes:

```powershell
plantuml --preproc diagram.puml
```

[Back to Contents](#contents)

### Extract Source from a Generated Diagram

This works only if metadata was not disabled when the image was generated:

```powershell
plantuml --extract-source diagram.png
```

[Back to Contents](#contents)

## 13. Exit Codes

| Exit code | Meaning |
| --- | --- |
| `0` | Success |
| `50` | No file found |
| `100` | No diagram found in files |
| `200` | Some diagrams have syntax errors |

[Back to Contents](#contents)

## 14. Troubleshooting

### `plantuml` Is Not Recognized

Confirm this folder is in `PATH`:

```text
C:\Apps\PlantUML
```

Then close and reopen PowerShell.

[Back to Contents](#contents)

### `dot` Is Not Recognized

Confirm this folder is in `PATH`:

```text
C:\Program Files\Graphviz\bin
```

Then close and reopen PowerShell.

[Back to Contents](#contents)

### PlantUML Cannot Find Graphviz

Run:

```powershell
plantuml --check-graphviz
```

If it fails, update `C:\Apps\PlantUML\plantuml.bat` so it points to the real `dot.exe` path:

```bat
@echo off
java -DGRAPHVIZ_DOT="C:\Program Files\Graphviz\bin\dot.exe" -jar "C:\Apps\PlantUML\plantuml.jar" %*
```

[Back to Contents](#contents)

### Diagram Rendering Is Slow or Times Out

Increase the Graphviz timeout:

```powershell
plantuml --graphviz-timeout 120 diagram.puml
```

For many diagrams, use multiple threads:

```powershell
plantuml --threads auto "**/*.puml"
```

[Back to Contents](#contents)
