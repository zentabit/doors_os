#include "screen.h"

// // void print_char(char character, int col, int row, char attribute_byte){
// //     unsigned char *vmem = (unsigned char *) VIDEO_ADDRESS;

// //     if (!attribute_byte) {
// //         attribute_byte = WHITE_ON_BLACK;
// //     }

// //     int offset;

// //     if (col >= 0 && row >= 0){
// //         offset = get_screen_offset(col, row);
// //     } else {
// //         offset = get_cursor();
// //     }

// //     if (character = '\n'){
// //         int rows = offset / (2*MAX_COLS);
// //     } else {
// //         vmem[offset] = character;
// //         vmem[offset+1] = attribute_byte;
// //     }

// //     offset += 2;
// //     offset = handle_scrolling(offset);

// //     set_cursor(offset);
// // }

// int get_screen_offset(col, row){
//     return (row * MAX_COLS + col) * 2;
// }

// int get_cursor(){
//     port_byte_out(REG_SCREEN_CTRL, 14);
//     int offset = port_byte_in(REG_SCREEN_DATA) << 8;
//     port_byte_out(REG_SCREEN_CTRL, 15);
//     offset += port_byte_in(REG_SCREEN_DATA);
//     return offset * 2;
// }

// void set_cursor(int offset){
//     offset /= 2;
//     port_byte_out(REG_SCREEN_CTRL, 14);
//     port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
//     port_byte_out(REG_SCREEN_CTRL, 15);
//     port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
// }

// void print_at(char* msg, int col, int row){
//     if (col >= 0 && row >= 0){
//         set_cursor(get_screen_offset(col, row));
//     }

//     int i = 0;
//     while (msg[i] != 0){
//         print_char(msg[i++], col, row, WHITE_ON_BLACK);
//     }
// }

// void print(char* msg){
//     print_at(msg, -1, -1);
// }

// void cls(){
//     int row = 0;
//     int col = 0;

//     for (row = 0; row<MAX_ROWS; row++){
//         for (col = 0; col<MAX_COLS; col++){
//             print_char(' ', col, row, WHITE_ON_BLACK);
//         }
//     }

//     set_cursor(get_screen_offset(0,0));
// }

// int handle_scrolling(int cursor_offset){
//     if (cursor_offset < MAX_ROWS*MAX_COLS*2){
//         return cursor_offset;
//     }

//     for (int i = 1; i < MAX_ROWS; i++){
//         memcpy(get_screen_offset(0,i) + VIDEO_ADDRESS, get_screen_offset(0, i-1) + VIDEO_ADDRESS, MAX_COLS*2);
//     }

//     char* last_line = get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
//     for (int i = 0; i < MAX_COLS*2;i++){
//         last_line[i] = 0;
//     }

//     cursor_offset -= 2*MAX_COLS;

//     return cursor_offset;
// }


int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void print_at(char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void print(char *message) {
    print_at(message, -1, -1);
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = WHITE_ON_BLACK;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++) 
            memcpy(get_offset(0, i) + VIDEO_ADDRESS,
                        get_offset(0, i-1) + VIDEO_ADDRESS,
                        MAX_COLS * 2);

        /* Blank last line */
        char *last_line = get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
        for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }