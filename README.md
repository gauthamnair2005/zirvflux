# ZirvFlux — MOSIX Display Framework Library

A user-space display framework for MOSIX, providing a compositor-facing API
for display enumeration, buffer management, and presentation. Analogous to
Mesa for Linux — the reference library that ZirvUI and other MOSIX compositors
link against.

| | |
|---|---|
| **Website** | [zirvworld](https://github.com/gauthamnair2005/zirvworld) |
| **Part of** | Zirvium reference MOSIX implementation |
| **License** | GPLv3 |

---

## API

`libzirvflux.a` exposes the following operations:

- `zf_connect()` / `zf_disconnect()` — open/close the display device
- `zf_get_info()` — query display dimensions and connector name
- `zf_create_buffer()` / `zf_destroy_buffer()` — allocate/release GPU surfaces (up to 32)
- `zf_write_buffer()` — write pixel data to a buffer
- `zf_present()` / `zf_present_region()` — flip a buffer to the screen
- `zf_list_buffers()` — enumerate all allocated buffers
- `zf_set_cursor()` — set hardware cursor position
- `zf_set_perf_mode()` — performance hint to compositor (no-op stub)
- `zf_read_mouse()` — read next mouse event (non-blocking)
- `zf_reboot()` — system reboot

The library communicates with the kernel display driver (DisplayJet) via
custom MOSIX syscalls (110–120).

---

## Building

```bash
make
```

Produces `libzirvflux.a`. Requires a freestanding C compiler (`x86_64-elf-gcc`
or `gcc`).

---

## Usage

Link your compositor against `libzirvflux.a`:

```c
#include <zirvflux.h>

zf_connect();
zf_display_info_t info;
zf_get_info(&info);
// ...
```

See [ZirvUI](https://github.com/gauthamnair2005/zirvui) for a reference
compositor implementation using this library.
