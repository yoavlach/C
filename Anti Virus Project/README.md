# AntiVirus Project

A simple command-line antivirus scanner written in C. It scans all files in a given folder and checks whether they contain a given virus signature (binary pattern). Supports two scanning modes: **Normal** and **Quick**.

---

## Features

- Scans all regular files in a specified directory (non-recursive).
- Two scan modes:
  - **Normal Scan**: Checks every possible byte position in each file.
  - **Quick Scan**: Checks only the first and last 20% of each file. If not found and still suspicious, falls back to full scan and reports that the match was outside the 20% ranges.
- Generates a formatted log file (`AntiVirusLog.txt`) with results.
- Alphabetical, deterministic file processing order.
- Handles binary virus signatures of arbitrary length.

---

## How It Works

1. Reads the virus signature file entirely into memory.
2. For each file in the target folder:
   - Reads the file into memory.
   - Performs a byte-by-byte search for the signature over the requested scan range.
3. Reports one of:
   - `Clean`
   - `Infected!`
   - `Infected! (first 20%)`
   - `Infected! (last 20%)`
   - `Infected! (not found in the first and last 20%)`
   - `Clean (file too small to contain virus signature)`
4. Results are printed to the console and saved to `AntiVirusLog.txt` in the scanned folder.

---

## Build

This project uses standard C and POSIX directory APIs.

**Dependencies:**

- A C compiler (`gcc` / `clang` / `MSVC`)
- `dirent.h` (On Windows, you may need a compatibility layer such as a bundled `dirent.h`)

### Example (Linux/macOS)

```bash
gcc -O2 -std=c11 -o antiVirusProject antiVirusProject.c
```

### Example (Windows with MinGW / dirent.h available)

```bash
gcc -O2 -std=c11 -o antiVirusProject.exe antiVirusProject.c
```

If your platform does not provide `dirent.h`, add a compatible implementation or adapt directory scanning accordingly.

---

## Usage

```text
antiVirusProject <folder_to_scan> <virus_signature_file>
```

- **folder\_to\_scan**: Path to the directory containing files to scan.
- **virus\_signature\_file**: Path to a binary file whose contents constitute the virus signature.

On start, you will be prompted to select a scan mode:

- `0` = Normal Scan
- Any other number = Quick Scan

### Example:

```bash
./antiVirusProject ./samples ./signatures/mal.sig
```

**Console interaction:**

- You’ll see a welcome message.
- You’ll be prompted for the scan mode.
- Progress and per-file results will be displayed.

---

## Output

### Console:

- Prints each file path and its status.
- Shows when scanning starts and completes.

### Log File:

- `AntiVirusLog.txt` written to the scanned folder.
- Includes scan metadata (folder, signature path, mode) and per-file results.

---

## Return Codes

- **0**: Success
- **1**: Error (e.g., invalid usage, unable to open the signature file, allocation failures)

---

## File Overview

**antiVirusProject.c**

- `main`: Argument parsing, user prompt, orchestration
- `checkIfThereAreEnoughParameters`: Validates CLI args
- `printWelcomeMsg`: Displays initial info
- `getActionInput`: Reads scan mode from stdin
- `makeFilePath`: Builds file path from folder + name
- `getFile`, `getFileLen`, `readDataFromBinaryFile`: File I/O helpers
- `scanFile`: Core scanning over a `[start%, end%]` window
- `sendNormalScan`, `sendQuickScan`: Scan-mode dispatchers
- `manageFolderReading`: Directory traversal, per-file scanning, message building
- `sortFilesAlphabetically`, `countFilesInFolder`, `freeArrOfPointers`: Utilities
- `createMsgStart`, `insertFileDataToMsg`: Results message assembly
- `insertMsgToFile`: Writes `AntiVirusLog.txt`

---

## Example Signature

To test, you can create a small “signature” file (binary or text) and embed that sequence in a sample file. The scanner will report **"Infected!"** where applicable.

---

## License
This project is free to use and modify for learning or personal purposes. If you share it publicly, please credit me.
