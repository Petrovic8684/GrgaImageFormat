# GRGA Image Format, Viewer & Converter

## Overview

**GRGA Image Viewer** is a lightweight image viewer designed to showcase and interact with images stored in a custom file format: `.grga`. The `.grga` format is a specialized image format optimized for simplicity and extensibility, incorporating compressed pixel data obtained by applying the Huffman compression algorithm to raw RGB pixel data.

![welcome](https://i.imgur.com/J2yySd6.jpeg)

This project includes:

- A custom `.grga` image format definition.
- An SDL2-based graphical viewer for `.grga` images.
- A SDL2-based graphical utility for converting images from `.png` or `.jpeg` to `.grga`.
- A focus on extensibility, allowing future enhancements like metadata support, advanced compression, and filters.

## Key Features

- **Custom `.grga` Format**:

  - Stores image dimensions, pixel depth, and channel count in a concise header.
  - Supports Huffman-compressed pixel data.
  - Easily extensible for metadata, annotations, or additional compression methods.

- **Interactive Viewer**:

  - Drag-and-drop support for opening `.grga` images or directories containing `.grga` files.
  - Adjustable zoom functionality with mouse scroll or an on-screen slider.
  - Smooth panning through images using horizontal and vertical scrollbars.
  - Switching between images within a directory via arrow keys or on-screen buttons.

![landscape](https://i.imgur.com/xT9qyNd.jpeg)

![waterfall](https://i.imgur.com/QdW0Vmi.jpeg)

- **Conversion Utility**:
  - A tool for converting `.png` and `.jpeg` images to `.grga` format.
  - Extensible codebase for supporting other formats in the future.

## The `.grga` Format

The `.grga` format is designed to balance simplicity and power.

### Header Structure

- `GRGA` Identifier (4 bytes)
- `Width` (4 bytes, unsigned int)
- `Height` (4 bytes, unsigned int)
- `Pixel Depth` (1 byte)
- `Channel Count` (1 byte)

### Pixel Data

Compressed pixel data obtained by applying the Huffman compression algorithm to raw RGB pixel data.

### Example Workflow

1. Convert any `.png` or `.jpeg` image to `.grga` using the GrgaConverter.
2. Load and view in GrgaViewer.

## Installation

### Dependencies

This project is designed with minimal dependencies to ensure simplicity and maximum control over its functionality.

- **SDL2**: Used for rendering, window management, and handling user input.
- **SDL_image** and **SDL_ttf** libraries used for rendering window icons and text.
- **stb_image.h**: A lightweight, single-header library for loading image files like PNG and JPEG into raw pixel data.
- **GCC**: Required to compile the project source code.

Notably, even the GUI for this project has been implemented manually, rather than relying on external libraries. This approach allows for fine-grained control over the user interface and reduces dependency complexity.

### Building the Project

This project includes all required dependencies (such as `SDL2` and `stb_image.h`), so you don't need to install them separately. To build the project, simply clone the repository, navigate to the project directory, and run the `make` command. This will compile the source code and generate the executable files.

```bash
git clone https://github.com/Petrovic8684/GrgaImageFormat.git
cd GrgaImageFormat
make
```

### Credits

- GrgaViewer magnifier icon: <a target="_blank" href="https://icons8.com/icon/12773/search">Search</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>
- GrgaConverter loop icon: <a target="_blank" href="https://icons8.com/icon/13113/refresh">convert</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>
