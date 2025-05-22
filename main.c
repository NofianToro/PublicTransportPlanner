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
	return (jenis == ANGKOT) ? 4 : 20;
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
		printf("Demand kendaraan: ");
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
	
	return 0;
}
