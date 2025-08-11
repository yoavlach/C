# GIF Maker (C + Raylib)

A simple command-line GIF/movie creation tool written in C with **Raylib** for frame display and image effects. It allows creating, editing, saving, loading, and playing animated sequences made from individual image frames.

---

## Features

- Create projects and add frames (images) from disk.
- Edit frames:
  - Change duration of a single frame.
  - Change duration for all frames.
  - Move frames to a new position in the sequence.
- Image effects:
  - Black & White
  - Gaussian Blur
  - Color Tint (red, green, blue, yellow, brown)
- Play animation in a Raylib window with timing based on frame durations.
- Save/Load projects to/from files for persistence.
- CLI Menu to navigate actions and apply changes.

---

## Project Structure

```
.
├── editFiles.c                 # Frame editing operations (durations, reorder)
├── filesInProjectManagement.c  # Create/link/remove frames from linked list
├── GIFMaker.c                  # Main program logic & menus
├── helpers.c                   # Utility functions (memory, lists, search, validation)
├── saveAndLoad.c               # Save/load project files
├── view.c                      # Display/play frames with Raylib & effects
└── include/                    # (Recommended) Headers for each module
```

**Core Concepts**

- **FrameNode**: Linked list node storing `Frame` data (name, duration, path).
- **Linked List Management**: Add/remove/reorder frames.
- **Raylib**: Handles image loading, transformations, and rendering.
- **Persistence**: Custom text-based save format with lengths + data to avoid parsing errors.

---

## Dependencies

- C Compiler: GCC or Clang recommended
- Raylib (>= 4.0)
- Standard C libraries: `stdio.h`, `stdlib.h`, `string.h`, etc.

**On Linux (Debian/Ubuntu):**

```bash
sudo apt install build-essential libraylib-dev
```

**On Windows:**

- Install Raylib via MSYS2 or download precompiled binaries.
- Ensure Raylib’s include & lib paths are configured in your compiler settings.

---

## Building

From the project root:

```bash
gcc GIFMaker.c editFiles.c filesInProjectManagment.c helpers.c saveAndLoad.c view.c -o gifmaker -lraylib -lm -ldl -lpthread
```

For Windows (MinGW example):

```bash
gcc GIFMaker.c editFiles.c filesInProjectManagment.c helpers.c saveAndLoad.c view.c -o gifmaker.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

---

## Usage

Run the program:

```bash
./gifmaker
```

The menu system offers options such as:

1. Create new project
2. Load existing project
3. Add frame
4. Remove frame
5. Change frame duration
6. Move frame
7. Apply effects
8. Play animation
9. Save project
10. Exit

When playing, a Raylib window opens and shows each frame in sequence using its set duration.

---

## Project File Format

Saved project files store each frame's data in the format:

```
[name_length]name[duration][path_length]path
```

- `name_length` → integer, length of frame name
- `duration` → integer (milliseconds)
- `path_length` → integer, length of the image path string

This ensures reliable parsing without relying on delimiters in names or paths.

---

## License

This project is free to use and modify for learning or personal purposes. If you share it publicly, please credit me.
