#define SQUARE_WIDTH 2
#define SQUARE_HEIGHT 1

#define PAD_HEIGHT 4

#define COLUMNS 80
#define ROWS 24

#define DE3 0b00
#define VAIA2 0b01
#define VAIA3 0b10
#define QFG 0b11

#define KEYCODE_W 0x11
#define KEYCODE_S 0x1f
#define KEYCODE_I 0x17
#define KEYCODE_K 0x25
#define KEYCODE_SPACE 0x39

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

u32 putsatmiddle(const char* buffer, u32 y, u8 color);
void clear_buffer();
void draw_rect(u32, u32, u32, u32, u32);
void delay(u32);
void show_data(u8, u8, u8);
u8 inb(u16);
u32 putsat(const char*, u32, u32, u8);
void clear_vga();

__attribute__((section(".text._start"))) void _start() {
    volatile unsigned char* vga = (unsigned char*)0xB8000;

    clear_vga();
    putsatmiddle("PONG", ROWS / 2 - 1, 0x0f);
    putsatmiddle("Press space", ROWS / 2 + 1, 0x0f);
    while(inb(0x60) != KEYCODE_SPACE);

    START:

    u8 key_w_down = 0, key_s_down = 0, key_i_down = 0, key_k_down = 0;
    u8 scancode = inb(0x60);
    u8 mode = DE3;
    u8 old_mode = 4;

    int x_speed = 2;
    int y_speed = 1;
    int x_pos = COLUMNS / 2;
    int y_pos = ROWS / 2;

    int player_a_pad_y = ROWS / 2 - PAD_HEIGHT / 2;
    int player_a_pad_direction = 0;
    int player_b_pad_y = ROWS / 2 - PAD_HEIGHT / 2;
    int player_b_pad_direction = 0;

    u8 player_a_score = 0;
    u8 player_b_score = 0;

    u8 speed_modifier = 4;
    u8 cycles = 0;

    u8 start = 0;

    char winner;

    delay(2000);

    while (1) {
        if (cycles % speed_modifier == 0) {
            x_pos += x_speed;
            y_pos += y_speed;
        }

        scancode = inb(0x60);
        if (scancode == KEYCODE_W) key_w_down = 1;
        else if (scancode == (KEYCODE_W | 0x80)) key_w_down = 0;
        if (scancode == KEYCODE_S) key_s_down = 1;
        else if (scancode == (KEYCODE_S | 0x80)) key_s_down = 0;
        if (scancode == KEYCODE_I) key_i_down = 1;
        else if (scancode == (KEYCODE_I | 0x80)) key_i_down = 0;
        if (scancode == KEYCODE_K) key_k_down = 1;
        else if (scancode == (KEYCODE_K | 0x80)) key_k_down = 0;

        if (
            x_pos <= 2 && 
            y_pos >= player_a_pad_y &&
            y_pos <= player_a_pad_y + PAD_HEIGHT
        ) {
            x_speed = (x_speed < 0) ? -x_speed : x_speed;
            u32 absolute_y = (y_speed < 0) ? -y_speed : y_speed;
            if (player_a_pad_direction < 0) y_speed = -absolute_y;
            else if (player_a_pad_direction > 0) y_speed = absolute_y;
            x_pos = 2;
            
            if (speed_modifier > 1) {
                speed_modifier--;
            }
        }

        if (
            x_pos >= COLUMNS - 3 && 
            y_pos >= player_b_pad_y && 
            y_pos <= player_b_pad_y + PAD_HEIGHT
        ) {
            x_speed = (x_speed > 0) ? -x_speed : x_speed;
            u32 absolute_y = (y_speed < 0) ? -y_speed : y_speed;
            if (player_b_pad_direction < 0) y_speed = -absolute_y;
            else if (player_b_pad_direction > 0) y_speed = absolute_y;
            x_pos = COLUMNS - 3;

            if (speed_modifier > 1) {
                speed_modifier--;
            }
        }

        if (x_pos > (COLUMNS - SQUARE_WIDTH) || x_pos < 0) {
            if (x_pos > (COLUMNS - SQUARE_WIDTH)) {
                player_a_score++;
            }
            else {
                player_b_score++;
            }

            switch (mode) {
                case DE3:
                    if (player_a_score == 2 && player_b_score == 2) {
                        mode = VAIA2;
                        player_a_score = 0;
                        player_b_score = 0;
                    }
                    else {
                        if (player_a_score == 3) {
                            winner = 'a';
                            goto END;
                        }
                        if (player_b_score == 3) {
                            winner = 'b';
                            goto END;
                        }
                    }

                    break;

                case VAIA2:
                    if (player_a_score == 1 && player_b_score == 1) {
                        mode = VAIA3;
                        player_a_score = 0;
                        player_b_score = 0;
                    }
                    else {
                        if (player_a_score == 2) {
                            winner = 'a';
                            goto END;
                        }
                        if (player_b_score == 2) {
                            winner = 'b';
                            goto END;
                        }
                    }

                    break;
                
                case VAIA3:
                    if (player_a_score == 2 && player_b_score == 2) {
                        mode = QFG;
                    }
                    else {
                        if (player_a_score == 3) {
                            winner = 'a';
                            goto END;
                        }
                        if (player_b_score == 3) {
                            winner = 'b';
                            goto END;
                        }
                    }

                    break;
                case QFG:
                    if (player_a_score == 3) {
                        winner = 'a';
                        goto END;
                    }
                    if (player_b_score == 3) {
                        winner = 'b';
                        goto END;
                    }

                    break;
            }
            
            player_a_pad_y = ROWS / 2 - PAD_HEIGHT / 2;
            player_b_pad_y = ROWS / 2 - PAD_HEIGHT / 2;
            x_pos = COLUMNS / 2;
            y_pos = ROWS / 2;
            speed_modifier = 4;
            start = 1;

            x_speed = -x_speed;
        }

        if (y_pos <= 0) {
            y_speed = (y_speed < 0) ? -y_speed : y_speed;
            y_pos = 0;
        } else if (y_pos >= ROWS - SQUARE_HEIGHT) {
            y_speed = (y_speed > 0) ? -y_speed : y_speed;
            y_pos = ROWS - SQUARE_HEIGHT;
        }

        player_a_pad_direction = 0;
        player_b_pad_direction = 0;

        if (key_w_down) {
            if (player_a_pad_y > 0) {
                player_a_pad_direction = -1;
            }
        }

        if (key_s_down) {
            if (player_a_pad_y + PAD_HEIGHT < ROWS) {
                player_a_pad_direction = 1;
            }
        }

        if (key_i_down) {
            if (player_b_pad_y > 0) {
                player_b_pad_direction = -1;
            }
        }

        if (key_k_down) {
            if (player_b_pad_y + PAD_HEIGHT < ROWS) {
                player_b_pad_direction = 1;
            }
        }

        player_a_pad_y += player_a_pad_direction;
        player_b_pad_y += player_b_pad_direction;

        clear_buffer();
        show_data(player_a_score, player_b_score, mode);
        draw_rect((u32)x_pos, (u32)y_pos, SQUARE_WIDTH, SQUARE_HEIGHT, 0x05);
        draw_rect(1, player_a_pad_y, 1, PAD_HEIGHT, 0x0f);
        draw_rect(COLUMNS - 2, player_b_pad_y, 1, PAD_HEIGHT, 0x0f);
        delay(130);

        if (start == 1) {
            start = 0;
            delay(1000);
        }

        if (old_mode != mode) {
            old_mode = mode;
            delay(3500);
            vga[0] = '!'; vga[1] = 0x0f;
            vga[COLUMNS * 2 - 2] = '!'; vga[COLUMNS * 2 - 1] = 0x0f;
            delay(2000);
        }

        cycles++;
    }

    END:

    if (winner == 'a') {
        putsatmiddle("Player A WON", ROWS / 2 - 1, 0x0e);
    }
    else {
        putsatmiddle("Player B WON", ROWS / 2 - 1, 0x0e);
    }

    putsatmiddle("Press space to restart", ROWS / 2, 0x0f);

    while(inb(0x60) != KEYCODE_SPACE);
    goto START;
}

u8 inb(u16 port) {
    u8 result;

    asm volatile(
        "\n\tinb %1, %0" 
        : "=a"(result) 
        : "Nd"(port)
    );

    return result;
}


void delay(u32 count) {
    for (volatile u32 i = 0; i < count * 100000; i++);
}

u32 putsat(const char* buffer, u32 i, u32 j, u8 color) {
    volatile unsigned char* vga = (unsigned char*)0xB8000;
    u32 x = i;
    u32 y = j;

    for (u32 char_index = 0; buffer[char_index] != '\0'; char_index++) {
        u32 index = (y * COLUMNS + x) * 2;

        vga[index] = buffer[char_index];
        vga[index + 1] = color; 

        x++;
    }

    return (y * COLUMNS + x - 2) * 2;
}

u32 strlen(const char* buffer) {
    u32 length = 0;
    while (buffer[length] != '\0') length++;

    return length;
}

u32 putsatmiddle(const char* buffer, u32 y, u8 color) {
    volatile unsigned char* vga = (unsigned char*)0xB8000;
    u32 x = COLUMNS / 2 - strlen(buffer) / 2;

    for (u32 char_index = 0; buffer[char_index] != '\0'; char_index++) {
        u32 index = (y * COLUMNS + x) * 2;

        vga[index] = buffer[char_index];
        vga[index + 1] = color; 

        x++;
    }

    return (y * COLUMNS + x - 2) * 2;
}

void clear_buffer() {
    volatile unsigned char* vga = (unsigned char*)0xb8000;
    for (int i = COLUMNS * 2; i < COLUMNS * ROWS * 2 + COLUMNS * 2; i += 2) {
        vga[i] = '.';
        vga[i + 1] = 0x08;
    }
}

void clear_vga() {
    volatile unsigned char* vga = (unsigned char*)0xb8000;
    for (int i = 0; i < COLUMNS * ROWS * 2 + COLUMNS * 2; i += 2) {
        vga[i] = '.';
        vga[i + 1] = 0x08;
    }
}

void show_data(u8 a_score, u8 b_score, u8 mode) {
    volatile unsigned char* vga = (unsigned char*)0xb8000;
    
    for (u32 column = 0; column < COLUMNS * 2; column += 2) {
        vga[column] = 0xdb;
        vga[column + 1] = 0x00;
    }

    switch (mode) {
        case DE3:
            putsatmiddle("vai a tres", 0, 0x0f); break;
        case VAIA2:
            putsatmiddle("vai a dois", 0, 0x0f); break;
        case VAIA3:
            putsatmiddle("vai a 3 sem vai a 2", 0, 0x0f); break;
        case QFG:
            putsatmiddle("QUEM FIZER GANHA", 0, 0x04); break;
    }

    u32 next;

    next = putsat(" PLAYER A: 0 ", 1, 0, 0x09);
    vga[next] = (char)(a_score + 0x30);
    vga[next + 1] = 0x0e;
    next = putsat(" PLAYER B: 0 ", 66, 0, 0x0c);
    vga[next] = (char)(b_score + 0x30);
    vga[next + 1] = 0x0e;
}

void draw_rect(u32 x, u32 y, u32 width, u32 height, u32 color) {
    volatile unsigned char* vga = (unsigned char*)0xb8000;
    for (u32 row = 0; row < height; row++) {
        for (u32 col = 0; col < width; col++) {
            u32 index = ((y + row + 1) * COLUMNS + (x + col)) * 2;
            vga[index] = 0xdb;
            vga[index + 1] = color; 
        }
    }
}
