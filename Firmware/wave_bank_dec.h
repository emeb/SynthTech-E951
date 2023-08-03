const int8_t wave_bank_dec[] = {
	/* wave 0 */
	0x01, 0x11, 0x20, 0x30, 0x3F, 0x4E, 0x5C, 0x6B, 
	0x7A, 0x75, 0x63, 0x52, 0x41, 0x30, 0x20, 0x0F, 
	0xFF, 0xF0, 0xE0, 0xD1, 0xC2, 0xB3, 0xA4, 0x96, 
	0x87, 0x8B, 0x9D, 0xAE, 0xBF, 0xCF, 0xDF, 0xF0, 
	/* wave 1 */
	0x00, 0xF8, 0xF0, 0xE8, 0xE0, 0xD8, 0xD0, 0xC8, 
	0xC1, 0xB9, 0xB1, 0xA9, 0xA1, 0x99, 0x91, 0x89, 
	0x7F, 0x77, 0x6F, 0x67, 0x5F, 0x57, 0x4F, 0x47, 
	0x3F, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 
	/* wave 2 */
	0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 
	/* wave 3 */
	0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
	0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 
	/* wave 4 */
	0x00, 0x19, 0x30, 0x46, 0x5A, 0x69, 0x75, 0x7C, 
	0x7F, 0x7C, 0x75, 0x69, 0x5A, 0x46, 0x30, 0x19, 
	0xFE, 0xEE, 0xDE, 0xCD, 0xBD, 0xAE, 0x9E, 0x8E, 
	0x81, 0x85, 0x96, 0xA8, 0xBA, 0xCC, 0xDD, 0xEF, 
	/* wave 5 */
	0x01, 0x12, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 
	0x7F, 0x7C, 0x6A, 0x58, 0x45, 0x34, 0x22, 0x11, 
	0xE1, 0x96, 0x82, 0x81, 0x81, 0x81, 0x81, 0x81, 
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x8C, 
	/* wave 6 */
	0x32, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
	0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7D, 
	0x9D, 0x89, 0x91, 0x99, 0xA1, 0xA9, 0xB1, 0xB9, 
	0xC1, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 
	/* wave 7 */
	0x81, 0x9A, 0xB3, 0xCB, 0xE2, 0xF9, 0x0E, 0x22, 
	0x35, 0x45, 0x54, 0x61, 0x6C, 0x74, 0x7A, 0x7E, 
	0x7F, 0x7E, 0x7A, 0x74, 0x6C, 0x61, 0x54, 0x45, 
	0x35, 0x22, 0x0E, 0xF9, 0xE2, 0xCB, 0xB3, 0x9A, 
	/* wave 8 */
	0x00, 0x19, 0x31, 0x47, 0x5A, 0x6A, 0x75, 0x7D, 
	0x7F, 0x7D, 0x75, 0x6A, 0x5A, 0x47, 0x31, 0x19, 
	0x00, 0xE7, 0xCF, 0xB9, 0xA6, 0x96, 0x8B, 0x83, 
	0x81, 0x83, 0x8B, 0x96, 0xA6, 0xB9, 0xCF, 0xE7, 
	/* wave 9 */
	0x00, 0x31, 0x5A, 0x75, 0x7F, 0x75, 0x5A, 0x31, 
	0x00, 0xCF, 0xA6, 0x8B, 0x81, 0x8B, 0xA6, 0xCF, 
	0x00, 0x31, 0x5A, 0x75, 0x7F, 0x75, 0x5A, 0x31, 
	0x00, 0xCF, 0xA6, 0x8B, 0x81, 0x8B, 0xA6, 0xCF, 
	/* wave 10 */
	0x00, 0x47, 0x75, 0x7D, 0x5A, 0x19, 0xCF, 0x96, 
	0x81, 0x96, 0xCF, 0x19, 0x5A, 0x7D, 0x75, 0x47, 
	0x00, 0xB9, 0x8B, 0x83, 0xA6, 0xE7, 0x31, 0x6A, 
	0x7F, 0x6A, 0x31, 0xE7, 0xA6, 0x83, 0x8B, 0xB9, 
	/* wave 11 */
	0x00, 0x5A, 0x7F, 0x5A, 0x00, 0xA6, 0x81, 0xA6, 
	0x00, 0x5A, 0x7F, 0x5A, 0x00, 0xA6, 0x81, 0xA6, 
	0x00, 0x5A, 0x7F, 0x5A, 0x00, 0xA6, 0x81, 0xA6, 
	0x00, 0x5A, 0x7F, 0x5A, 0x00, 0xA6, 0x81, 0xA6, 
	/* wave 12 */
	0x00, 0x6A, 0x75, 0x19, 0xA6, 0x83, 0xCF, 0x47, 
	0x7F, 0x47, 0xCF, 0x83, 0xA6, 0x19, 0x75, 0x6A, 
	0x00, 0x96, 0x8B, 0xE7, 0x5A, 0x7D, 0x31, 0xB9, 
	0x81, 0xB9, 0x31, 0x7D, 0x5A, 0xE7, 0x8B, 0x96, 
	/* wave 13 */
	0x00, 0x75, 0x5A, 0xCF, 0x81, 0xCF, 0x5A, 0x75, 
	0x00, 0x8B, 0xA6, 0x31, 0x7F, 0x31, 0xA6, 0x8B, 
	0x00, 0x75, 0x5A, 0xCF, 0x81, 0xCF, 0x5A, 0x75, 
	0x00, 0x8B, 0xA6, 0x31, 0x7F, 0x31, 0xA6, 0x8B, 
	/* wave 14 */
	0x00, 0x7D, 0x31, 0x96, 0xA6, 0x47, 0x75, 0xE7, 
	0x81, 0xE7, 0x75, 0x47, 0xA6, 0x96, 0x31, 0x7D, 
	0x00, 0x83, 0xCF, 0x6A, 0x5A, 0xB9, 0x8B, 0x19, 
	0x7F, 0x19, 0x8B, 0xB9, 0x5A, 0x6A, 0xCF, 0x83, 
	/* wave 15 */
	0x00, 0x7F, 0x00, 0x81, 0x00, 0x7F, 0x00, 0x81, 
	0x00, 0x7F, 0x00, 0x81, 0x00, 0x7F, 0x00, 0x81, 
	0x00, 0x7F, 0x00, 0x81, 0x00, 0x7F, 0x00, 0x81, 
	0x00, 0x7F, 0x00, 0x81, 0x00, 0x7F, 0x00, 0x81, 
	/* wave 16 */
	0x07, 0x31, 0x4D, 0x77, 0x7B, 0x74, 0x51, 0x38, 
	0x05, 0xDC, 0xB7, 0xAA, 0xA1, 0xA5, 0xB7, 0xCC, 
	0xDE, 0xE4, 0xEB, 0xE2, 0xD3, 0xBA, 0xA8, 0x94, 
	0x86, 0x80, 0x85, 0x8F, 0x9B, 0x9B, 0xB2, 0xD8, 
	/* wave 17 */
	0x09, 0x4E, 0x77, 0x70, 0x4D, 0x1A, 0xF1, 0xD2, 
	0xD4, 0xE7, 0x07, 0x10, 0x03, 0xDA, 0xB7, 0x9D, 
	0x9B, 0xA0, 0xB2, 0xBF, 0xC7, 0xBB, 0xA5, 0x8A, 
	0x81, 0x84, 0x94, 0x9E, 0xA6, 0x9C, 0xA1, 0xC4, 
	/* wave 18 */
	0x07, 0x54, 0x6E, 0x7C, 0x5A, 0x37, 0xF5, 0xDB, 
	0xD0, 0xF2, 0x00, 0x10, 0x00, 0xF2, 0xC8, 0xAA, 
	0x93, 0x9F, 0xAE, 0xC0, 0xC2, 0xC1, 0xB1, 0x9C, 
	0x87, 0x82, 0x8C, 0x99, 0xA4, 0xA2, 0x9E, 0xCC, 
	/* wave 19 */
	0x0D, 0x59, 0x7E, 0x5C, 0x2D, 0xF0, 0xE1, 0xDC, 
	0xFC, 0x11, 0x20, 0x0A, 0xE0, 0xB8, 0xA2, 0xB0, 
	0xC0, 0xD7, 0xD3, 0xCA, 0xAF, 0x9D, 0x90, 0x96, 
	0xA7, 0xB4, 0xB9, 0xAB, 0x9E, 0x8E, 0x8A, 0xBC, 
	/* wave 20 */
	0x02, 0x6D, 0x7D, 0x6E, 0x3D, 0x19, 0x05, 0x14, 
	0x38, 0x35, 0x14, 0xE4, 0xD6, 0xCE, 0xD5, 0xDD, 
	0xDF, 0xCC, 0xAD, 0xA3, 0xA8, 0xB1, 0xAF, 0xAF, 
	0xA6, 0x99, 0x90, 0x95, 0x9F, 0x9F, 0x88, 0xB7, 
	/* wave 21 */
	0x05, 0x41, 0x50, 0x78, 0x6B, 0x55, 0x31, 0xFB, 
	0xD3, 0xB4, 0xA3, 0xAB, 0xBE, 0xD1, 0xF0, 0xF3, 
	0xF7, 0xE5, 0xC9, 0xAF, 0x92, 0x82, 0x80, 0x86, 
	0x99, 0xAE, 0xBD, 0xCA, 0xBC, 0xB8, 0xCD, 0xE5, 
	/* wave 22 */
	0x00, 0x30, 0x58, 0x76, 0x7F, 0x7B, 0x60, 0x3D, 
	0x0F, 0xE1, 0xBB, 0x9C, 0x90, 0x8F, 0x9E, 0xB6, 
	0xD2, 0xEF, 0x05, 0x12, 0x13, 0x0A, 0xF6, 0xDD, 
	0xC2, 0xA8, 0x96, 0x8C, 0x8A, 0x8B, 0xAD, 0xD3, 
	/* wave 23 */
	0x03, 0x37, 0x65, 0x7F, 0x6E, 0x44, 0x10, 0xE1, 
	0xCB, 0xC5, 0xCA, 0xE7, 0xFD, 0xF8, 0xEF, 0xE0, 
	0xBF, 0xA1, 0x98, 0x97, 0x9B, 0xA8, 0xB6, 0xBC, 
	0xB8, 0xAC, 0xA0, 0x95, 0x8A, 0x87, 0x84, 0xB8, 
	/* wave 24 */
	0x07, 0x32, 0x76, 0x7F, 0x56, 0x30, 0xE4, 0xAB, 
	0x9D, 0x94, 0xB0, 0xD5, 0xE2, 0xEB, 0xDB, 0xB9, 
	0xA0, 0x88, 0x81, 0x8F, 0x9F, 0xB4, 0xC1, 0xBF, 
	0xB6, 0xA6, 0x96, 0x8F, 0x8F, 0x97, 0x88, 0xC1, 
	/* wave 25 */
	0x08, 0x6D, 0x75, 0x2A, 0xEA, 0xD3, 0xD9, 0xFD, 
	0x12, 0xE7, 0xAA, 0x9C, 0xAD, 0xBC, 0xC7, 0xC3, 
	0xA1, 0x87, 0x96, 0xAE, 0xB0, 0xAA, 0xA1, 0x92, 
	0x8E, 0xA0, 0xAD, 0xA4, 0x9A, 0x98, 0x8B, 0xAF, 
	/* wave 26 */
	0x0A, 0x72, 0x7F, 0x40, 0xFA, 0xCC, 0xD6, 0x06, 
	0x14, 0xF2, 0xC3, 0xA1, 0x9F, 0xBF, 0xD4, 0xC8, 
	0xAF, 0x98, 0x92, 0xA5, 0xB8, 0xB7, 0xAA, 0x9B, 
	0x94, 0x9D, 0xAB, 0xAF, 0xA9, 0x9F, 0x8E, 0xB0, 
	/* wave 27 */
	0x14, 0x64, 0x5F, 0x23, 0xEA, 0xD4, 0xF1, 0x13, 
	0xFA, 0xB7, 0x9A, 0xB1, 0xC5, 0xC0, 0xAE, 0x99, 
	0x8F, 0x98, 0xA9, 0xA9, 0x9A, 0x8C, 0x8C, 0x98, 
	0xA2, 0x9C, 0x8F, 0x8C, 0x93, 0x99, 0x8C, 0xB5, 
	/* wave 28 */
	0x03, 0x78, 0x67, 0x26, 0xFA, 0x07, 0x30, 0x0C, 
	0xCF, 0xC7, 0xCF, 0xD7, 0xC9, 0xA5, 0xA6, 0xB3, 
	0xAE, 0xA8, 0x9C, 0x98, 0xA5, 0xA2, 0x9B, 0x9A, 
	0x97, 0x9C, 0x9E, 0x97, 0x98, 0x99, 0x99, 0x98, 
	/* wave 29 */
	0x04, 0x52, 0x79, 0x62, 0x2E, 0xE4, 0xB5, 0x93, 
	0xAA, 0xCB, 0xF8, 0xF9, 0xE9, 0xBD, 0x9A, 0x80, 
	0x85, 0x9E, 0xBE, 0xD1, 0xCE, 0xBC, 0xA0, 0x8D, 
	0x86, 0x94, 0xA8, 0xBD, 0xC2, 0xB5, 0xAE, 0xCE, 
	/* wave 30 */
	0x00, 0x3A, 0x68, 0x7E, 0x68, 0x3D, 0xFC, 0xBE, 
	0x93, 0x81, 0x8F, 0xAF, 0xD8, 0xFB, 0x0A, 0x05, 
	0xEB, 0xC7, 0xA5, 0x8D, 0x88, 0x93, 0xAB, 0xC7, 
	0xDE, 0xE9, 0xE5, 0xD5, 0xBF, 0x90, 0xA6, 0xBE, 
	/* wave 31 */
	0x0B, 0x3E, 0x76, 0x6A, 0x24, 0xE3, 0xC9, 0xB9, 
	0xE2, 0xF0, 0xE0, 0xD7, 0xA1, 0x98, 0x97, 0x9F, 
	0xB5, 0xB4, 0xA6, 0x9E, 0x8C, 0x8D, 0x9A, 0x9A, 
	0xA6, 0x9E, 0x96, 0x94, 0x8E, 0x94, 0x93, 0x92, 
};
