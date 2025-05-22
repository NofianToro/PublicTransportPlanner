#include <stdio.h>
#include <string.h>

#define MAX_SEKTOR 10

typedef enum {
    ANGKOT,
    BUS
} JenisKendaraan;

typedef enum {
    LISTRIK,
    DIESEL
} TipeKendaraan;

typedef struct {
    char nama[50];
    float jarak;
    char jalan_raya;
    int demand;
    JenisKendaraan jenis;
    TipeKendaraan tipe;
    int kendaraan_dialokasikan;
} Sektor;

int main() {
	
	return 0;
}