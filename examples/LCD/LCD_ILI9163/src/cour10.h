const uint8_t courBitmaps[] = {
  0xF4, 0xB6, 0x80, 0x2A, 0xBE, 0xA5, 0x7D, 0x54, 0x27, 0x84, 0x3E, 0x44,
  0x4A, 0x7E, 0x52, 0x74, 0x4B, 0xF0, 0xE0, 0x6A, 0xA4, 0x95, 0x58, 0x27,
  0xC8, 0xA0, 0x21, 0x3E, 0x42, 0x00, 0x68, 0xF0, 0xC0, 0x11, 0x22, 0x44,
  0x88, 0x69, 0x99, 0x96, 0x61, 0x08, 0x42, 0x7C, 0x69, 0x24, 0x8F, 0x69,
  0x12, 0x96, 0x32, 0xA5, 0xF1, 0x1C, 0x74, 0x61, 0x96, 0x74, 0xE9, 0x9E,
  0xF9, 0x12, 0x22, 0x69, 0x69, 0x96, 0x79, 0x97, 0x2E, 0x90, 0x41, 0xA0,
  0x19, 0x30, 0x41, 0x80, 0xF8, 0x3E, 0xC1, 0x06, 0x4C, 0x00, 0x69, 0x12,
  0x02, 0x74, 0x67, 0x59, 0xC1, 0xE0, 0x61, 0x14, 0xE8, 0xEC, 0xF2, 0x5C,
  0x94, 0xF8, 0x79, 0x88, 0x87, 0xF2, 0x52, 0x94, 0xF8, 0xFA, 0x58, 0x84,
  0xFC, 0xFA, 0x58, 0x84, 0x70, 0x74, 0x21, 0x79, 0x30, 0xDD, 0x27, 0x92,
  0x4B, 0xB0, 0xE9, 0x25, 0xC0, 0x38, 0x85, 0x29, 0x30, 0xED, 0x25, 0x1C,
  0x4B, 0xB0, 0xE2, 0x10, 0x84, 0xFC, 0xDF, 0x6D, 0xAA, 0x8B, 0x70, 0xDD,
  0xA6, 0x96, 0x5B, 0xA0, 0x69, 0x99, 0x96, 0xF2, 0x52, 0xE4, 0x70, 0x69,
  0x99, 0x96, 0x30, 0xF1, 0x24, 0x9C, 0x4B, 0x90, 0xF9, 0x61, 0x9E, 0xFD,
  0x48, 0x42, 0x38, 0xDD, 0x24, 0x92, 0x48, 0xC0, 0xCE, 0x14, 0x92, 0x48,
  0xC0, 0xED, 0x15, 0x55, 0x54, 0xA0, 0xDA, 0x88, 0x45, 0x6C, 0xED, 0x12,
  0x84, 0x10, 0xE0, 0xF9, 0x24, 0x9F, 0xEA, 0xAC, 0x84, 0x44, 0x22, 0x11,
  0xD5, 0x5C, 0x06, 0x90, 0xFC, 0x90, 0x70, 0xBD, 0xF0, 0xC2, 0x1C, 0x94,
  0xF8, 0x78, 0x87, 0x30, 0x9D, 0x29, 0x3C, 0x6F, 0x87, 0x34, 0xF4, 0x4F,
  0x7C, 0xA4, 0xE1, 0x70, 0xC1, 0x07, 0x12, 0x4B, 0xB0, 0x43, 0x25, 0xC0,
  0x20, 0xF1, 0x11, 0x1F, 0xC2, 0x16, 0xC5, 0x6C, 0xC9, 0x25, 0xC0, 0xE9,
  0x55, 0x75, 0xF1, 0x24, 0xB3, 0x69, 0x96, 0xF2, 0x52, 0xE4, 0x70, 0x7C,
  0xA4, 0xE1, 0x1C, 0xF4, 0x4F, 0xFE, 0x1E, 0x47, 0xD0, 0x93, 0x00, 0xD9,
  0x24, 0x8F, 0xCD, 0x23, 0x0C, 0xED, 0x55, 0x4A, 0xD9, 0x15, 0xB0, 0xED,
  0x22, 0x84, 0x21, 0x80, 0xFA, 0x5F, 0x29, 0x64, 0x88, 0xFE, 0x89, 0x34,
  0xA0, 0xDA };

const GFXglyph courGlyphs[] = {
  {     0,   0,   0,   6,    0,    1 },   // 0x20 ' '
  {     0,   1,   6,   6,    3,   -5 },   // 0x21 '!'
  {     1,   3,   3,   6,    2,   -5 },   // 0x22 '"'
  {     3,   5,   8,   6,    0,   -6 },   // 0x23 '#'
  {     8,   4,   8,   6,    1,   -6 },   // 0x24 '$'
  {    12,   4,   6,   6,    1,   -5 },   // 0x25 '%'
  {    15,   4,   5,   6,    1,   -4 },   // 0x26 '&'
  {    18,   1,   3,   6,    3,   -5 },   // 0x27 '''
  {    19,   2,   7,   6,    2,   -5 },   // 0x28 '('
  {    21,   2,   7,   6,    2,   -5 },   // 0x29 ')'
  {    23,   5,   4,   6,    1,   -5 },   // 0x2A '*'
  {    26,   5,   5,   6,    0,   -5 },   // 0x2B '+'
  {    30,   2,   3,   6,    2,   -1 },   // 0x2C ','
  {    31,   4,   1,   6,    1,   -3 },   // 0x2D '-'
  {    32,   2,   1,   6,    2,    0 },   // 0x2E '.'
  {    33,   4,   8,   6,    0,   -6 },   // 0x2F '/'
  {    37,   4,   6,   6,    1,   -5 },   // 0x30 '0'
  {    40,   5,   6,   6,    1,   -5 },   // 0x31 '1'
  {    44,   4,   6,   6,    1,   -5 },   // 0x32 '2'
  {    47,   4,   6,   6,    1,   -5 },   // 0x33 '3'
  {    50,   5,   6,   6,    1,   -5 },   // 0x34 '4'
  {    54,   4,   6,   6,    1,   -5 },   // 0x35 '5'
  {    57,   4,   6,   6,    1,   -5 },   // 0x36 '6'
  {    60,   4,   6,   6,    1,   -5 },   // 0x37 '7'
  {    63,   4,   6,   6,    1,   -5 },   // 0x38 '8'
  {    66,   4,   6,   6,    1,   -5 },   // 0x39 '9'
  {    69,   1,   4,   6,    2,   -3 },   // 0x3A ':'
  {    70,   2,   6,   6,    2,   -3 },   // 0x3B ';'
  {    72,   5,   5,   6,    0,   -4 },   // 0x3C '<'
  {    76,   5,   3,   6,    1,   -4 },   // 0x3D '='
  {    78,   5,   5,   6,    1,   -4 },   // 0x3E '>'
  {    82,   4,   6,   6,    1,   -5 },   // 0x3F '?'
  {    85,   5,   7,   6,    1,   -5 },   // 0x40 '@'
  {    90,   5,   6,   6,    1,   -5 },   // 0x41 'A'
  {    94,   5,   6,   6,    0,   -5 },   // 0x42 'B'
  {    98,   4,   6,   6,    1,   -5 },   // 0x43 'C'
  {   101,   5,   6,   6,    0,   -5 },   // 0x44 'D'
  {   105,   5,   6,   6,    0,   -5 },   // 0x45 'E'
  {   109,   5,   6,   6,    1,   -5 },   // 0x46 'F'
  {   113,   5,   6,   6,    1,   -5 },   // 0x47 'G'
  {   117,   6,   6,   6,    0,   -5 },   // 0x48 'H'
  {   122,   3,   6,   6,    2,   -5 },   // 0x49 'I'
  {   125,   5,   6,   6,    1,   -5 },   // 0x4A 'J'
  {   129,   6,   6,   6,    0,   -5 },   // 0x4B 'K'
  {   134,   5,   6,   6,    0,   -5 },   // 0x4C 'L'
  {   138,   6,   6,   6,    0,   -5 },   // 0x4D 'M'
  {   143,   6,   6,   6,    0,   -5 },   // 0x4E 'N'
  {   148,   4,   6,   6,    1,   -5 },   // 0x4F 'O'
  {   151,   5,   6,   6,    0,   -5 },   // 0x50 'P'
  {   155,   4,   7,   6,    1,   -5 },   // 0x51 'Q'
  {   159,   6,   6,   6,    0,   -5 },   // 0x52 'R'
  {   164,   4,   6,   6,    1,   -5 },   // 0x53 'S'
  {   167,   5,   6,   6,    0,   -5 },   // 0x54 'T'
  {   171,   6,   6,   6,    0,   -5 },   // 0x55 'U'
  {   176,   6,   6,   6,    0,   -5 },   // 0x56 'V'
  {   181,   6,   6,   6,    0,   -5 },   // 0x57 'W'
  {   186,   5,   6,   6,    0,   -5 },   // 0x58 'X'
  {   190,   6,   6,   6,    0,   -5 },   // 0x59 'Y'
  {   195,   4,   6,   6,    1,   -5 },   // 0x5A 'Z'
  {   198,   2,   7,   6,    2,   -5 },   // 0x5B '['
  {   200,   4,   8,   6,    0,   -6 },   // 0x5C '\'
  {   204,   2,   7,   6,    2,   -5 },   // 0x5D ']'
  {   206,   4,   3,   6,    1,   -6 },   // 0x5E '^'
  {   208,   6,   1,   6,    0,    3 },   // 0x5F '_'
  {   209,   2,   2,   6,    2,   -5 },   // 0x60 '`'
  {   210,   5,   4,   6,    1,   -3 },   // 0x61 'a'
  {   213,   5,   6,   6,    0,   -5 },   // 0x62 'b'
  {   217,   4,   4,   6,    1,   -3 },   // 0x63 'c'
  {   219,   5,   6,   6,    1,   -5 },   // 0x64 'd'
  {   223,   4,   4,   6,    1,   -3 },   // 0x65 'e'
  {   225,   4,   6,   6,    1,   -5 },   // 0x66 'f'
  {   228,   5,   6,   6,    1,   -3 },   // 0x67 'g'
  {   232,   6,   6,   6,    0,   -5 },   // 0x68 'h'
  {   237,   3,   6,   6,    2,   -5 },   // 0x69 'i'
  {   240,   4,   8,   6,    1,   -5 },   // 0x6A 'j'
  {   244,   5,   6,   6,    0,   -5 },   // 0x6B 'k'
  {   248,   3,   6,   6,    1,   -5 },   // 0x6C 'l'
  {   251,   6,   4,   6,    0,   -3 },   // 0x6D 'm'
  {   254,   6,   4,   6,    0,   -3 },   // 0x6E 'n'
  {   257,   4,   4,   6,    1,   -3 },   // 0x6F 'o'
  {   259,   5,   6,   6,    0,   -3 },   // 0x70 'p'
  {   263,   5,   6,   6,    1,   -3 },   // 0x71 'q'
  {   267,   4,   4,   6,    1,   -3 },   // 0x72 'r'
  {   269,   4,   4,   6,    1,   -3 },   // 0x73 's'
  {   271,   5,   5,   6,    0,   -4 },   // 0x74 't'
  {   275,   6,   4,   6,    0,   -3 },   // 0x75 'u'
  {   278,   6,   4,   6,    0,   -3 },   // 0x76 'v'
  {   281,   6,   4,   6,    0,   -3 },   // 0x77 'w'
  {   284,   5,   4,   6,    0,   -3 },   // 0x78 'x'
  {   287,   6,   6,   6,    0,   -3 },   // 0x79 'y'
  {   292,   4,   4,   6,    1,   -3 },   // 0x7A 'z'
  {   294,   3,   7,   6,    1,   -5 },   // 0x7B '{'
  {   297,   1,   7,   6,    3,   -5 },   // 0x7C '|'
  {   298,   3,   7,   6,    2,   -5 },   // 0x7D '}'
  {   301,   4,   2,   6,    1,   -3 } }; // 0x7E '~'

const GFXfont cour = 
{
  (uint8_t  *)courBitmaps,
  (GFXglyph *)courGlyphs,
  0x20, 0x7E, 11, 6
};

// Approx. 974 bytes
