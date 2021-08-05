#ifndef __KEY_H__
#define __KEY_H__

#if 0
 ____    ___________________    ___________________    ___________________
|    |  |    |    |    |    |  |    |    |    |    |  |    |    |    |    |
|Esc |  |F1  |F2  |F3  |F4  |  |F5  |F6  |F7  |F8  |  |F9  |F10 |F11 |F12 |
|    |  |    |    |    |    |  |    |    |    |    |  |    |    |    |    |
|  01|  |  3B|  3C|  3D|  3E|  |  3F|  40|  41|  42|  |  43|  44|  57|  58|
|____|  |____|____|____|____|  |____|____|____|____|  |____|____|____|____|

 __________________________________________________________________________
|    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
|~   |!   |@   |#   |$   |%   |^   |&   |*   |(   |)   |_   |+   ||   |bksp|
|`   |1   |2   |3   |4   |5   |6   |7   |8   |9   |0   |-   |=   |\   |    |
|  29|  02|  03|  04|  05|  06|  07|  08|  09|  0A|  0B|  0C|  0D|  2B|  0E|
|____|____|____|____|____|____|____|____|____|____|____|____|____|____|____|
|    |    |    |    |    |    |    |    |    |    |    |    |    |         |
|Tab |Q   |W   |E   |R   |T   |Y   |U   |I   |O   |P   |{   |}   |         |
|    |    |    |    |    |    |    |    |    |    |    |[   |]   |         |
|  0F|  10|  11|  12|  13|  14|  15|  16|  17|  18|  19|  1A|  1B|         |
|____|____|____|____|____|____|____|____|____|____|____|____|____|         |
|    |    |    |    |    |    |    |    |    |    |    |    |              |
|Caps|A   |S   |D   |F   |G   |H   |J   |K   |L   |:   |"   |     Enter    |
|    |    |    |    |    |    |    |    |    |    |;   |'   |              |
|  3A|  1E|  1F|  20|  21|  22|  23|  24|  25|  26|  27|  28|            1C|
|____|____|____|____|____|____|____|____|____|____|____|____|______________|
|         |    |    |    |    |    |    |    |    |    |    |              |
| L Shift |Z   |X   |C   |V   |B   |N   |M   |<   |>   |?   |   R Shift    |
|         |    |    |    |    |    |    |    |,   |.   |/   |              |
|       2A|  2C|  2D|  2E|  2F|  30|  31|  32|  33|  34|  35|            36|
|_________|____|____|____|____|____|____|____|____|____|____|______________|
|       |       |       |                  |       |       |       |       |
|L Ctrl | L win | L Alt |       space      | R Alt | R win | menu  |R Ctrl |
|       |[1]    |       |                  |       |[1]    |[1]    |       |
|     1D|   E05B|     38|                39|   E038|   E05C|   E05D|   E01D|
|_______|_______|_______|__________________|_______|_______|_______|_______|
#endif

// Some common key constants as returned by keyGet()
#define BKEY_UP     0xC8	// Up arrow
#define BKEY_DOWN   0xD0	// Down arrow
#define BKEY_LEFT   0xCB	// Left arrow
#define BKEY_RIGHT  0xCD	// Right arrow
#define BKEY_ENTER  0x1C	// Enter/Return key
#define BKEY_ESCAPE 0x01	// Escape key
#define BKEY_Y      0x15	// 'Y' key
#define BKEY_N      0x31	// 'N' key
#define BKEY_W		0x11	// 'W' key
#define BKEY_A      0x1E	// 'A' key
#define BKEY_S		0x1F	// 'S' key
#define BKEY_D		0x20	// 'D' key

void  keyFlushStream();
uint8 keyGet();

#endif // __KEY_H__