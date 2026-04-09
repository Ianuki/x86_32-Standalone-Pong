#define SQUARE_WIDTH 2
#define SQUARE_HEIGHT 1
#define COLUMNS 80
#define ROWS 24

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

void clear_buffer();
void draw_square(u32, u32);
void delay(u32);
void show_data();
void putsat(const char*, u32, u32, u8);

__attribute__((section(".text._start"))) void _start() {
    int x_speed = 2;
    int y_speed = 1;
    int x_pos = 0;
    int y_pos = 0;

    while (1) {
        x_pos += x_speed;
        y_pos += y_speed;

        if (x_pos >= (COLUMNS - SQUARE_WIDTH) || x_pos <= 0) {
            x_speed = -x_speed;
        }

        if (y_pos >= (ROWS - SQUARE_HEIGHT) || y_pos <= 0) {
            y_speed = -y_speed;
        }

        clear_buffer();
        show_data();
        draw_square((u32)x_pos, (u32)y_pos);
        delay(200);
    }
}

void delay(u32 count) {
    for (volatile u32 i = 0; i < count * 100000; i++);
}

void putsat(const char* buffer, u32 i, u32 j, u8 color) {
    volatile unsigned char* vga = (unsigned char*)0xB8000;
    u32 x = i;
    u32 y = j;

    for (u32 char_index = 0; buffer[char_index] != '\0'; char_index++) {
        u32 index = (y * COLUMNS + x) * 2;

        vga[index] = buffer[char_index];
        vga[index + 1] = color; 

        x++;
    }
}

void clear_buffer() {
    volatile unsigned char* vga = (unsigned char*)0xb8000;
    for (int i = COLUMNS * 2; i < COLUMNS * ROWS * 2 + COLUMNS * 2; i += 2) {
        vga[i] = '.';
        vga[i + 1] = 0x09;
    }
}

void show_data() {
    volatile unsigned char* vga = (unsigned char*)0xb8000;
    
    for (u32 column = 0; column < COLUMNS * 2; column += 2) {
        vga[column] = 0xdb;
        vga[column + 1] = 0x0f;
    }

    putsat(" say wallahi bruh ", 31, 0, 0x0f);
    putsat(" PLAYER A: 0 ", 1, 0, 0x74);
    putsat(" PLAYER B: 0 ", 66, 0, 0x71);
}

void draw_square(u32 x, u32 y) {
    volatile unsigned char* vga = (unsigned char*)0xb8000;
    for (u32 row = 0; row < SQUARE_HEIGHT; row++) {
        for (u32 col = 0; col < SQUARE_WIDTH; col++) {
            u32 index = ((y + row + 1) * COLUMNS + (x + col)) * 2;
            vga[index] = 0xdb;
            vga[index + 1] = 0x05; 
        }
    }
}
