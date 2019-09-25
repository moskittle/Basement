#pragma once

// From glfw3.h
#define BM_KEY_SPACE              32
#define BM_KEY_APOSTROPHE         39  /* ' */
#define BM_KEY_COMMA              44  /* , */
#define BM_KEY_MINUS              45  /* - */
#define BM_KEY_PERIOD             46  /* . */
#define BM_KEY_SLASH              47  /* / */
#define BM_KEY_0                  48
#define BM_KEY_1                  49
#define BM_KEY_2                  50
#define BM_KEY_3                  51
#define BM_KEY_4                  52
#define BM_KEY_5                  53
#define BM_KEY_6                  54
#define BM_KEY_7                  55
#define BM_KEY_8                  56
#define BM_KEY_9                  57
#define BM_KEY_SEMICOLON          59  /* ; */
#define BM_KEY_EQUAL              61  /* = */
#define BM_KEY_A                  65
#define BM_KEY_B                  66
#define BM_KEY_C                  67
#define BM_KEY_D                  68
#define BM_KEY_E                  69
#define BM_KEY_F                  70
#define BM_KEY_G                  71
#define BM_KEY_H                  72
#define BM_KEY_I                  73
#define BM_KEY_J                  74
#define BM_KEY_K                  75
#define BM_KEY_L                  76
#define BM_KEY_M                  77
#define BM_KEY_N                  78
#define BM_KEY_O                  79
#define BM_KEY_P                  80
#define BM_KEY_Q                  81
#define BM_KEY_R                  82
#define BM_KEY_S                  83
#define BM_KEY_T                  84
#define BM_KEY_U                  85
#define BM_KEY_V                  86
#define BM_KEY_W                  87
#define BM_KEY_X                  88
#define BM_KEY_Y                  89
#define BM_KEY_Z                  90
#define BM_KEY_LEFT_BRACKET       91  /* [ */
#define BM_KEY_BACKSLASH          92  /* \ */
#define BM_KEY_RIGHT_BRACKET      93  /* ] */
#define BM_KEY_GRAVE_ACCENT       96  /* ` */
#define BM_KEY_WORLD_1            161 /* non-US #1 */
#define BM_KEY_WORLD_2            162 /* non-US #2 */

// Function Keys
#define BM_KEY_ESCAPE             256
#define BM_KEY_ENTER              257
#define BM_KEY_TAB                258
#define BM_KEY_BACKSPACE          259
#define BM_KEY_INSERT             260
#define BM_KEY_DELETE             261
#define BM_KEY_RIGHT              262
#define BM_KEY_LEFT               263
#define BM_KEY_DOWN               264
#define BM_KEY_UP                 265
#define BM_KEY_PAGE_UP            266
#define BM_KEY_PAGE_DOWN          267
#define BM_KEY_HOME               268
#define BM_KEY_END                269
#define BM_KEY_CAPS_LOCK          280
#define BM_KEY_SCROLL_LOCK        281
#define BM_KEY_NUM_LOCK           282
#define BM_KEY_PRINT_SCREEN       283
#define BM_KEY_PAUSE              284
#define BM_KEY_F1                 290
#define BM_KEY_F2                 291
#define BM_KEY_F3                 292
#define BM_KEY_F4                 293
#define BM_KEY_F5                 294
#define BM_KEY_F6                 295
#define BM_KEY_F7                 296
#define BM_KEY_F8                 297
#define BM_KEY_F9                 298
#define BM_KEY_F10                299
#define BM_KEY_F11                300
#define BM_KEY_F12                301
#define BM_KEY_F13                302
#define BM_KEY_F14                303
#define BM_KEY_F15                304
#define BM_KEY_F16                305
#define BM_KEY_F17                306
#define BM_KEY_F18                307
#define BM_KEY_F19                308
#define BM_KEY_F20                309
#define BM_KEY_F21                310
#define BM_KEY_F22                311
#define BM_KEY_F23                312
#define BM_KEY_F24                313
#define BM_KEY_F25                314
#define BM_KEY_KP_0               320
#define BM_KEY_KP_1               321
#define BM_KEY_KP_2               322
#define BM_KEY_KP_3               323
#define BM_KEY_KP_4               324
#define BM_KEY_KP_5               325
#define BM_KEY_KP_6               326
#define BM_KEY_KP_7               327
#define BM_KEY_KP_8               328
#define BM_KEY_KP_9               329
#define BM_KEY_KP_DECIMAL         330
#define BM_KEY_KP_DIVIDE          331
#define BM_KEY_KP_MULTIPLY        332
#define BM_KEY_KP_SUBTRACT        333
#define BM_KEY_KP_ADD             334
#define BM_KEY_KP_ENTER           335
#define BM_KEY_KP_EQUAL           336
#define BM_KEY_LEFT_SHIFT         340
#define BM_KEY_LEFT_CONTROL       341
#define BM_KEY_LEFT_ALT           342
#define BM_KEY_LEFT_SUPER         343
#define BM_KEY_RIGHT_SHIFT        344
#define BM_KEY_RIGHT_CONTROL      345
#define BM_KEY_RIGHT_ALT          346
#define BM_KEY_RIGHT_SUPER        347
#define BM_KEY_MENU               348

// Mouse Buttons
#define BM_MOUSE_BUTTON_1         0
#define BM_MOUSE_BUTTON_2         1
#define BM_MOUSE_BUTTON_3         2
#define BM_MOUSE_BUTTON_4         3
#define BM_MOUSE_BUTTON_5         4
#define BM_MOUSE_BUTTON_6         5
#define BM_MOUSE_BUTTON_7         6
#define BM_MOUSE_BUTTON_8         7
#define BM_MOUSE_BUTTON_LAST      BM_MOUSE_BUTTON_8
#define BM_MOUSE_BUTTON_LEFT      BM_MOUSE_BUTTON_1
#define BM_MOUSE_BUTTON_RIGHT     BM_MOUSE_BUTTON_2
#define BM_MOUSE_BUTTON_MIDDLE    BM_MOUSE_BUTTON_3

// Joystick
#define BM_JOYSTICK_1             0
#define BM_JOYSTICK_2             1
#define BM_JOYSTICK_3             2
#define BM_JOYSTICK_4             3
#define BM_JOYSTICK_5             4
#define BM_JOYSTICK_6             5
#define BM_JOYSTICK_7             6
#define BM_JOYSTICK_8             7
#define BM_JOYSTICK_9             8
#define BM_JOYSTICK_10            9
#define BM_JOYSTICK_11            10
#define BM_JOYSTICK_12            11
#define BM_JOYSTICK_13            12
#define BM_JOYSTICK_14            13
#define BM_JOYSTICK_15            14
#define BM_JOYSTICK_16            15
#define BM_JOYSTICK_LAST          BM_JOYSTICK_16

// Console Controller
#define BM_GAMEPAD_BUTTON_A               0
#define BM_GAMEPAD_BUTTON_B               1
#define BM_GAMEPAD_BUTTON_X               2
#define BM_GAMEPAD_BUTTON_Y               3
#define BM_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define BM_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define BM_GAMEPAD_BUTTON_BACK            6
#define BM_GAMEPAD_BUTTON_START           7
#define BM_GAMEPAD_BUTTON_GUIDE           8
#define BM_GAMEPAD_BUTTON_LEFT_THUMB      9
#define BM_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define BM_GAMEPAD_BUTTON_DPAD_UP         11
#define BM_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define BM_GAMEPAD_BUTTON_DPAD_DOWN       13
#define BM_GAMEPAD_BUTTON_DPAD_LEFT       14
#define BM_GAMEPAD_BUTTON_LAST            BM_GAMEPAD_BUTTON_DPAD_LEFT

#define BM_GAMEPAD_BUTTON_CROSS       BM_GAMEPAD_BUTTON_A
#define BM_GAMEPAD_BUTTON_CIRCLE      BM_GAMEPAD_BUTTON_B
#define BM_GAMEPAD_BUTTON_SQUARE      BM_GAMEPAD_BUTTON_X
#define BM_GAMEPAD_BUTTON_TRIANGLE    BM_GAMEPAD_BUTTON_Y