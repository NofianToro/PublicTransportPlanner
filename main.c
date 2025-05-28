#include <stdio.h>
#include <string.h>

#define MAX_SEKTOR 10

typedef enum
{
	ANGKOT,
	BUS
} JenisKendaraan;

typedef enum
{
	LISTRIK,
	DIESEL
} TipeKendaraan;

typedef struct
{
	char nama[50];
	float jarak;
	char jalan_raya;
	int demand;
	JenisKendaraan jenis;
	TipeKendaraan tipe;
	int kendaraan_dialokasikan;
} Sektor;

int kapasitas(JenisKendaraan jenis)
{
	return (jenis == ANGKOT) ? 7 : 20;
}

const char *jenisToStr(JenisKendaraan jenis)
{
	return (jenis == ANGKOT) ? "Angkot" : "Bus";
}

const char *tipeToStr(TipeKendaraan tipe)
{
	return (tipe == LISTRIK) ? "Listrik" : "Diesel";
}

int main()
{
	int jumlahSektor, jumlahAngkot, jumlahBus;
	Sektor sektor[MAX_SEKTOR];

	printf("Masukkan jumlah angkot awal: ");
	scanf("%d", &jumlahAngkot);
	printf("Masukkan jumlah bis awal: ");
	scanf("%d", &jumlahBus);
	printf("Masukkan jumlah sektor: ");
	scanf("%d", &jumlahSektor);

	for (int i = 0; i < jumlahSektor; i++)
	{
		printf("\nSEKTOR %d\n", i + 1);
		printf("Nama sektor: ");
		scanf(" %[^\n]", sektor[i].nama);
		printf("Jarak dari pusat (km): ");
		scanf("%f", &sektor[i].jarak);
		printf("Melalui jalan raya? (y/n): ");
		scanf(" %c", &sektor[i].jalan_raya);
		printf("Demand : ");
		scanf("%d", &sektor[i].demand);

		// Tentukan jenis kendaraan
		if (sektor[i].jalan_raya == 'n')
		{
			sektor[i].jenis = ANGKOT;
		}
		else
		{
			sektor[i].jenis = (sektor[i].demand >= 100) ? BUS : ANGKOT;
		}

		// Tentukan tipe kendaraan
		sektor[i].tipe = (sektor[i].jarak >= 30) ? LISTRIK : DIESEL;

		sektor[i].kendaraan_dialokasikan = 0;
	}
	
	for (int i = 0; i < jumlahSektor; i++) {
        int kapas = kapasitas(sektor[i].jenis);
        sektor[i].kendaraan_dialokasikan = sektor[i].demand / kapas;
        if (sektor[i].demand % kapas != 0)
            sektor[i].kendaraan_dialokasikan += 1;

        if (sektor[i].jenis == ANGKOT) {
            jumlahAngkot -= sektor[i].kendaraan_dialokasikan;
        } else {
            jumlahBus -= sektor[i].kendaraan_dialokasikan;
        }
    }

    for (int i = 0; i < jumlahSektor; i++) {
        if ((sektor[i].jenis == ANGKOT && jumlahAngkot < 0) && jumlahBus > 0) {
            sektor[i].jenis = BUS;
            int kapas = kapasitas(BUS);
            sektor[i].kendaraan_dialokasikan = sektor[i].demand / kapas;
            if (sektor[i].demand % kapas != 0)
                sektor[i].kendaraan_dialokasikan += 1;

            jumlahAngkot += sektor[i].kendaraan_dialokasikan;
            jumlahBus -= sektor[i].kendaraan_dialokasikan;
        }
    }

	printf("\n======= HASIL DISTRIBUSI KENDARAAN =======\n");
    for (int i = 0; i < jumlahSektor; i++) {
        printf("SEKTOR %d (%s)\n", i + 1, sektor[i].nama);
        printf("  - Jarak: %.1f km\n", sektor[i].jarak);
        printf("  - Demand: %d\n", sektor[i].demand);
        printf("  - Jenis kendaraan: %s\n", jenisToStr(sektor[i].jenis));
        printf("  - Tipe kendaraan : %s\n", tipeToStr(sektor[i].tipe));
        printf("  - Jumlah kendaraan dialokasikan: %d\n", sektor[i].kendaraan_dialokasikan);
        printf("--------------------------------------\n");
    }
	printf("\n======= SISA KENDARAAN =======\n");
    printf("  - Angkot: %d\n", jumlahAngkot);
    printf("  - Bis   : %d\n", jumlahBus);
	
	return 0;
}
