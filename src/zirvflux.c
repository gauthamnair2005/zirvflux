#include "zirvflux.h"
#include <string.h>

#define SYS_DJ_CONNECT         110
#define SYS_DJ_DISCONNECT      111
#define SYS_DJ_CREATE_SURFACE  112
#define SYS_DJ_DESTROY_SURFACE 113
#define SYS_DJ_PRESENT         114
#define SYS_DJ_GET_MODE        115
#define SYS_DJ_SURFACE_WRITE   116
#define SYS_DJ_SURFACE_READ    117
#define SYS_DJ_LIST_SURFACES   118
#define SYS_REBOOT             100
#define SYS_SHUTDOWN           101
#define SYS_MOUSE_READ         122
#define SYS_DJ_SET_CURSOR      123
#define SYS_DJ_SET_CURSOR_STYLE 128
#define SYS_DJ_SET_MODE        129

static long zf_syscall0(long n)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
    return ret;
}

static long zf_syscall1(long n, long a1)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
    return ret;
}

static long zf_syscall2(long n, long a1, long a2)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2) : "rcx", "r11", "memory");
    return ret;
}

static long zf_syscall3(long n, long a1, long a2, long a3)
{
    long ret;
    __asm__ volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");
    return ret;
}

int zf_connect(void)
{
    int ret = (int)zf_syscall0(SYS_DJ_CONNECT);
    return ret;
}

void zf_disconnect(void)
{
    zf_syscall0(SYS_DJ_DISCONNECT);
}

int zf_get_info(zf_display_info_t *info)
{
    if (!info) return -1;
    return (int)zf_syscall1(SYS_DJ_GET_MODE, (long)info);
}

int zf_create_buffer(uint32_t width, uint32_t height, zf_buffer_t *buf)
{
    if (!buf) return -1;

    uint32_t id;
    int ret = (int)zf_syscall2(SYS_DJ_CREATE_SURFACE, width, height);
    if (ret < 0) return ret;
    id = (uint32_t)ret;

    buf->id        = id;
    buf->width     = width;
    buf->height    = height;
    buf->stride    = width * 4;
    buf->bpp       = 32;
    buf->data_size = (size_t)buf->stride * height;
    buf->data      = 0;

    return 0;
}

void zf_destroy_buffer(zf_buffer_t *buf)
{
    if (!buf) return;
    zf_syscall1(SYS_DJ_DESTROY_SURFACE, buf->id);
    memset(buf, 0, sizeof(*buf));
}

int zf_write_buffer(zf_buffer_t *buf, const void *data, size_t size)
{
    if (!buf || !data) return -1;

    size_t expected = (size_t)buf->stride * buf->height;
    if (size > expected) size = expected;

    return (int)zf_syscall3(SYS_DJ_SURFACE_WRITE, buf->id,
                            (long)data, (long)size);
}

int zf_present(const zf_buffer_t *buf)
{
    if (!buf) return -1;
    return (int)zf_syscall1(SYS_DJ_PRESENT, buf->id);
}

int zf_present_region(const zf_buffer_t *buf,
                      uint32_t x, uint32_t y,
                      uint32_t w, uint32_t h)
{
    (void)x; (void)y; (void)w; (void)h;
    if (!buf) return -1;
    return (int)zf_syscall1(SYS_DJ_PRESENT, buf->id);
}

int zf_list_buffers(zf_buffer_t *bufs, uint32_t *count)
{
    if (!bufs || !count) return -1;
    return (int)zf_syscall2(SYS_DJ_LIST_SURFACES, (long)bufs, (long)count);
}

void zf_reboot(void)
{
    zf_syscall0(SYS_REBOOT);
}

void zf_shutdown(void)
{
    zf_syscall0(SYS_SHUTDOWN);
}

int zf_set_cursor(int x, int y)
{
    return (int)zf_syscall2(SYS_DJ_SET_CURSOR, (long)x, (long)y);
}

int zf_set_cursor_style(int style)
{
    return (int)zf_syscall1(SYS_DJ_SET_CURSOR_STYLE, (long)style);
}

int zf_set_mode(uint32_t width, uint32_t height)
{
    return (int)zf_syscall2(SYS_DJ_SET_MODE, (long)width, (long)height);
}

int zf_set_perf_mode(int mode)
{
    (void)mode;
    return 0;
}

int zf_read_mouse(zf_mouse_event_t *ev)
{
    if (!ev) return -1;
    return (int)zf_syscall1(SYS_MOUSE_READ, (long)ev);
}
