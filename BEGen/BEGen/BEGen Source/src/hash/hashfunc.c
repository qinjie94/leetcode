/*
 * hashfun.c
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 */

typedef unsigned int Index;

Index hash1_HASH(const char *Key, int TableSize) {
	unsigned int HashVal = 0;

	while (*Key != '\0')
		HashVal += *Key++;

	return HashVal % TableSize;
}


Index hash2_HASH(const char *Key, int TableSize) {
	return (Key[0] + 27 * Key[1] + 729 * Key[2]) % TableSize;
}


Index hash3_HASH(const char *Key, int TableSize) {
	unsigned int HashVal = 0;

	while (*Key != '\0')
		HashVal = (HashVal << 5) + *Key++;

	return HashVal % TableSize;
}
