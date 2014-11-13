#define KEY_ESC         0x01
#define KEY_1     0x02
#define KEY_2     0x03
#define KEY_3     0x04
#define KEY_4     0x05
#define KEY_5     0x06
#define KEY_6     0x07
#define KEY_7     0x08
#define KEY_8     0x09
#define KEY_9     0x0A
#define KEY_0     0x0B
#define KEY_DASH  0x0C
#define KEY_EQUAL 0x0D

#define KEY_A   0x1E
#define KEY_B     0x30
#define KEY_C     0x2E
#define KEY_D     0x20
#define KEY_E     0x12
#define KEY_F     0x21
#define KEY_G     0x22
#define KEY_H     0x23
#define KEY_I     0x17
#define KEY_J     0x24
#define KEY_K     0x25
#define KEY_L     0x26
#define KEY_M     0x32
#define KEY_N     0x31
#define KEY_O     0x18
#define KEY_P     0x19
#define KEY_Q     0x10
#define KEY_R     0x13
#define KEY_S     0x1F
#define KEY_T     0x14
#define KEY_U     0x16
#define KEY_V     0x2F
#define KEY_W     0x11
#define KEY_X     0x2D
#define KEY_Y     0x15
#define KEY_Z     0x2C

#define KEY_SPACE   0x39
#define KEY_HOME        0x47
#define KEY_UP          0x48
#define KEY_PGUP        0x49
#define KEY_MINUS       0x4A
#define KEY_LEFT        0x4B
#define KEY_CENTER      0x4C
#define KEY_RIGHT       0x4D
#define KEY_PLUS        0x4E
#define KEY_END         0x4F
#define KEY_DOWN        0x50
#define KEY_PGDOWN      0x51
#define KEY_INS         0x52
#define KEY_DEL         0x53
#define KEY_CTRL  0x1D
#define KEY_ALT   0x38
#define KEY_BACK  0x0E
#define KEY_TAB   0x0F
#define KEY_ENTER 0x1C
#define KEY_LSHIFT  0x2A
#define KEY_RSHIFT  0x38

#define KEY_WAS_PRESSED 128

#define PIC   0x20
#define NONSPECIFIC_EOI 0x20

#define KEYBOARD_PORT     0x60
#define KEYBOARD_XT_CLEAR 0x61

void install_new_key_handler(void);
void uninstall_new_key_handler(void);
void clear_key_buffer(void);
void far interrupt new_int09_handler(void);
void far interrupt (*old_int09_handler)();

typedef int             BOOL;
typedef unsigned int    DWORD;

unsigned char far *keybuf;

static int new_key_handler_installed = 0;

void install_new_key_handler(void)
{
  keybuf = farmalloc(KEY_WAS_PRESSED * 2);
  clear_key_buffer();

  old_int09_handler = getvect(9);
  setvect(9, new_int09_handler);
  new_key_handler_installed = 1;
}

void far interrupt new_int09_handler(void)
{
  unsigned scan_code;
  unsigned temp;

  scan_code = inportb(KEYBOARD_PORT);

  temp = inportb(KEYBOARD_XT_CLEAR);
  outportb(KEYBOARD_XT_CLEAR, temp | 0x80);
  outportb(KEYBOARD_XT_CLEAR, temp & 0x7F);

  if (scan_code & 0x80) {
    scan_code &= 0x7F;
    keybuf[scan_code] = 0;
  }
  else {
    keybuf[scan_code] = 1;
    keybuf[KEY_WAS_PRESSED+scan_code] = 1;
  }

  outportb(PIC, NONSPECIFIC_EOI);
}

void clear_key_buffer(void)
{
  int i;
  for (i = 0; i < KEY_WAS_PRESSED * 2; i++)
    keybuf[i] = 0;
}

void uninstall_new_key_handler(void)
{
  if (new_key_handler_installed) {
    farfree(keybuf);
    setvect(9, old_int09_handler);
  }
}
