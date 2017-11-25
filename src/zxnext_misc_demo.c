/*******************************************************************************
 * Stefan Bylund 2017
 *
 * A layer 2 screen demo program for Sinclair ZX Spectrum Next.
 *
 * zcc +zx -vn -SO3 -startup=30 -clib=sdcc_iy --max-allocs-per-node200000
 *   -L<zxnext_layer2>/lib/sdcc_iy -lzxnext_layer2 -I<zxnext_layer2>/include
 *   zxnext_misc_demo.c -o zxnext_misc_demo -create-app [-Cz"--sna"]
 ******************************************************************************/

#include <arch/zx.h>
#include <input.h>
#include <z80.h>
#include <intrinsic.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "zxnext_registers.h"
#include "zxnext_layer2.h"

#pragma output CRT_ORG_CODE = 0x6164
#pragma output REGISTER_SP = 0xBFFF
#pragma output CLIB_MALLOC_HEAP_SIZE = 0
#pragma output CLIB_STDIO_HEAP_SIZE = 0
#pragma output CLIB_FOPEN_MAX = -1

#define printAt(row, col, str) printf("\x16%c%c%s", (row), (col), (str))

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

static void init_hardware(void);

static void init_isr(void);

static void create_start_screen(void);

static void init_tests(void);

static void select_test(void);

static void flip_main_shadow_screen(void);

static void reset_main_shadow_screen(void);

static void test_layer2_over_ula(void);

static void test_ula_over_layer2(void);

static void test_main_screen_in_top_16k(void);

static void test_shadow_screen_in_top_16k(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t test_number = 0;

static bool flip_to_8_11 = false;

/*******************************************************************************
 * Functions
 ******************************************************************************/

static void init_hardware(void)
{
    uint8_t peripheral_2_settings;

    // Put Z80 in 14 MHz turbo mode.

    z80_outp(REGISTER_NUMBER_PORT, PERIPHERAL_2_REGISTER);
    peripheral_2_settings = z80_inp(REGISTER_VALUE_PORT);

    z80_outp(REGISTER_NUMBER_PORT, PERIPHERAL_2_REGISTER);
    z80_outp(REGISTER_VALUE_PORT, 0x80 | peripheral_2_settings);

    z80_outp(REGISTER_NUMBER_PORT, TURBO_MODE_REGISTER);
    z80_outp(REGISTER_VALUE_PORT, 2);
}

static void init_isr(void)
{
    // Set up IM2 interrupt service routine:
    // Put Z80 in IM2 mode with a 257-byte interrupt vector table located
    // at 0x6000 (before CRT_ORG_CODE) filled with 0x61 bytes. Install an
    // empty interrupt service routine at the interrupt service routine
    // entry at address 0x6161.

    intrinsic_di();
    im2_init((void *) 0x6000);
    memset((void *) 0x6000, 0x61, 257);
    z80_bpoke(0x6161, 0xFB);
    z80_bpoke(0x6162, 0xED);
    z80_bpoke(0x6163, 0x4D);
    intrinsic_ei();
}

static void create_start_screen(void)
{
    zx_border(INK_WHITE);
    zx_cls(INK_BLACK | PAPER_WHITE);

    printAt(7,  5, "Press any key to start");
    printAt(15, 1, "Press any key to switch screen");
}

static void init_tests(void)
{
    zx_border(INK_YELLOW);
    zx_cls(INK_BLACK | PAPER_WHITE);
    layer2_configure(true, false, false, 0);
}

static void select_test(void)
{
    switch (test_number)
    {
        case 0:
            test_layer2_over_ula();
            break;
        case 1:
            test_ula_over_layer2();
            break;
        case 2:
            test_main_screen_in_top_16k();
            break;
        case 3:
            test_shadow_screen_in_top_16k();
            break;
        default:
            break;
    }

    test_number = (test_number + 1) % 4;
}

static void flip_main_shadow_screen(void)
{
    if (flip_to_8_11)
    {
        layer2_set_main_screen_ram_bank(8);
        layer2_set_shadow_screen_ram_bank(11);
    }
    else
    {
        layer2_set_main_screen_ram_bank(11);
        layer2_set_shadow_screen_ram_bank(8);
    }

    flip_to_8_11 = !flip_to_8_11;
}

static void reset_main_shadow_screen(void)
{
    layer2_set_main_screen_ram_bank(8);
    layer2_set_shadow_screen_ram_bank(11);
    flip_to_8_11 = false;
}

static void test_layer2_over_ula(void)
{
    // By default, the layer 2 screen is over the ULA screen.
    // Draw a filled rectangle with the transparency colour in the middle
    // of the layer 2 screen so the underlying ULA screen shows through.

    zx_cls(INK_BLACK | PAPER_WHITE);
    printAt(12, 11, "ULA SCREEN");

    layer2_clear_screen(0x37, NULL);
    layer2_draw_text(5, 9, "LAYER 2 SCREEN", 0x88, NULL);
    layer2_fill_rect(64, 80, 128, 40, 0xE3, NULL);

    in_wait_key();
    // Reset to default settings.
    zx_cls(INK_BLACK | PAPER_WHITE);
}

static void test_ula_over_layer2(void)
{
    struct r_Rect8 rect = {8, 16, 10, 5};

    // Set the ULA screen to be over the layer 2 screen.
    // Draw a filled rectangle with the transparency colour in the middle
    // of the ULA screen so the underlying layer 2 screen shows through.

    // FIXME: I can only get ULA transparency to work when
    // the transparency colour is set to black (0x00)...

    zx_cls(INK_BLACK | PAPER_WHITE);
    printAt(5, 11, "ULA SCREEN");
    zx_cls_wc_attr(&rect, PAPER_BLACK);

    layer2_clear_screen(0x37, NULL);
    layer2_draw_text(12, 9, "LAYER 2 SCREEN", 0x88, NULL);

    layer2_set_layer_priorities(LAYER_PRIORITIES_S_U_L);
    layer2_set_global_transparency_color(0x00);

    in_wait_key();
    // Reset to default settings.
    zx_cls(INK_BLACK | PAPER_WHITE);
    layer2_set_layer_priorities(LAYER_PRIORITIES_S_L_U);
    layer2_set_global_transparency_color(0xE3);
}

/*
 * This function demonstrates that it is possible to page the main layer 2
 * screen to the top 16K RAM instead of the bottom 16K and still have any
 * drawing results directly displayed.
 */
static void test_main_screen_in_top_16k(void)
{
    layer2_screen_t main_screen_buffer = {OFF_SCREEN, 8, 9, 10};

    reset_main_shadow_screen();

    layer2_fill_rect(0, 0,   256, 64, 0xFE, &main_screen_buffer);
    layer2_fill_rect(0, 64,  256, 64, 0x7E, &main_screen_buffer);
    layer2_fill_rect(0, 128, 256, 64, 0x9F, &main_screen_buffer);

    layer2_draw_rect(24, 32, 208, 128, 0x6F, &main_screen_buffer);
    layer2_draw_text(12, 5, "Main screen in top 16K", 0x00, &main_screen_buffer);
}

/*
 * This function demonstrates that it is possible to page the shadow layer 2
 * screen (or any layer 2 off-screen buffer stored in consecutive RAM banks)
 * to the top 16K RAM instead of the bottom 16K. We have to flip the main and
 * shadow screens to display the results of the drawing.
 */
static void test_shadow_screen_in_top_16k(void)
{
    layer2_screen_t shadow_screen_buffer = {OFF_SCREEN, 11, 12, 13};

    reset_main_shadow_screen();

    layer2_fill_rect(0, 0,   256, 64, 0xFE, &shadow_screen_buffer);
    layer2_fill_rect(0, 64,  256, 64, 0x7E, &shadow_screen_buffer);
    layer2_fill_rect(0, 128, 256, 64, 0x9F, &shadow_screen_buffer);

    layer2_draw_rect(24, 32, 208, 128, 0x6F, &shadow_screen_buffer);
    layer2_draw_text(12, 4, "Shadow screen in top 16K", 0x00, &shadow_screen_buffer);

    flip_main_shadow_screen();
}

int main(void)
{
    init_hardware();
    init_isr();

    create_start_screen();
    in_wait_key();
    init_tests();

    while (true)
    {
        if (in_inkey())
        {
            in_wait_nokey();
            select_test();
        }
    }

    //return 0;
}