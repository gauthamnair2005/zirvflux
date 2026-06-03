#ifndef ZIRVFLUX_H
#define ZIRVFLUX_H

#include <stdint.h>
#include <stddef.h>

#define ZF_MAX_BUFFERS      32
#define ZF_CONNECTOR_NAME   64
#define ZF_ACCESS_KEY_SIZE  32

typedef enum {
    ZF_FORMAT_RGBX8888 = 0,
} zf_format_t;

/* NOTE: layout must match kernel's dj_display_mode_t exactly */
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint8_t  bpp;
    uint8_t  connected;
    uint8_t  _pad[2];
    char     connector_name[ZF_CONNECTOR_NAME];
} zf_display_info_t;

typedef struct {
    uint32_t id;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint8_t  bpp;
    void    *data;
    size_t   data_size;
} zf_buffer_t;

int  zf_connect(void);
void zf_disconnect(void);
int  zf_get_info(zf_display_info_t *info);

int  zf_create_buffer(uint32_t width, uint32_t height, zf_buffer_t *buf);
void zf_destroy_buffer(zf_buffer_t *buf);
int  zf_write_buffer(zf_buffer_t *buf, const void *data, size_t size);

int  zf_present(const zf_buffer_t *buf);
int  zf_present_region(const zf_buffer_t *buf,
                       uint32_t x, uint32_t y,
                       uint32_t w, uint32_t h);

int  zf_list_buffers(zf_buffer_t *bufs, uint32_t *count);

void zf_reboot(void);
int  zf_set_cursor(int x, int y);

/* ── Mouse / touchpad ──────────────────────────────────────────────────── */
typedef struct {
    int32_t  dx;
    int32_t  dy;
    uint8_t  buttons;
} zf_mouse_event_t;

/* Read next mouse event (non-blocking). Returns 0 on success, -1 if none. */
int zf_read_mouse(zf_mouse_event_t *ev);

/* Set performance mode (hint to compositor). Currently a no-op stub. */
int zf_set_perf_mode(int mode);

#endif
