#include <stdio.h>
#include <string.h>
#include <math.h>

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
	JenisKendaraan preferensi;
	TipeKendaraan tipe;
	int angkot_dialokasikan;
	int bus_dialokasikan;
	int permintaan_terpenuhi; // Hanya sebagai boolean
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

void alokasikanKendaraan(Sektor *s, int *jumlahBus, int *jumlahAngkot)
{
	int sisa = s->demand;

	if (s->jalan_raya == 'y' && *jumlahBus > 0 && sisa > 0)
	{
		int kapasBus = kapasitas(BUS);
		int butuhBus = (sisa + kapasBus - 1) / kapasBus;
		int pakaiBus = (butuhBus > *jumlahBus) ? *jumlahBus : butuhBus;

		s->bus_dialokasikan = pakaiBus;
		*jumlahBus -= pakaiBus;

		sisa -= pakaiBus * kapasBus;
	}

	if (sisa > 0 && *jumlahAngkot > 0)
	{
		int kapasAngkot = kapasitas(ANGKOT);
		int butuhAngkot = (sisa + kapasAngkot - 1) / kapasAngkot;
		int pakaiAngkot = (butuhAngkot > *jumlahAngkot) ? *jumlahAngkot : butuhAngkot;

		s->angkot_dialokasikan = pakaiAngkot;
		*jumlahAngkot -= pakaiAngkot;

		sisa -= pakaiAngkot * kapasAngkot;
	}

	s->permintaan_terpenuhi = (sisa > 0 && *jumlahBus <= 0 && *jumlahAngkot <= 0) ? 0 : 1;
}

int main()
{
	int jumlahSektor, jumlahAngkot, jumlahBus, sisaAngkot, sisaBus;
	Sektor sektor[MAX_SEKTOR];

	printf("Masukkan jumlah angkot awal (kapasitas 7 orang): ");
	scanf("%d", &jumlahAngkot);
	printf("Masukkan jumlah bis awal (kapasitas 20 orang): ");
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

		sektor[i].tipe = (sektor[i].jarak >= 30) ? LISTRIK : DIESEL;
		sektor[i].angkot_dialokasikan = 0;
		sektor[i].bus_dialokasikan = 0;

		// Mulai alokasi kendaraan
		alokasikanKendaraan(&sektor[i], &jumlahBus, &jumlahAngkot);
	}

	printf("\n======= HASIL DISTRIBUSI KENDARAAN =======\n");
	for (int i = 0; i < jumlahSektor; i++)
	{
		printf("SEKTOR %d (%s)\n", i + 1, sektor[i].nama);
		printf("  - Jarak					: %.1f km\n", sektor[i].jarak);
		printf("  - Demand					: %d orang\n", sektor[i].demand);
		printf("  - Bus 					: %d\n", sektor[i].bus_dialokasikan);
		printf("  - Angkot					: %d\n", sektor[i].angkot_dialokasikan);
		printf("  - Kapasitas total 		: %d orang\n", sektor[i].bus_dialokasikan * 20 + sektor[i].angkot_dialokasikan * 7);
		printf("  - Tipe kendaraan 			: %s\n", tipeToStr(sektor[i].tipe));
		printf("  - Permintaan terpenuhi	: %s\n", sektor[i].permintaan_terpenuhi ? "Ya" : "Tidak");
		printf("--------------------------------------\n");
	}

	printf("\n======= SISA KENDARAAN =======\n");
	printf("  - Angkot: %d\n", jumlahAngkot);
	printf("  - Bis   : %d\n", jumlahBus);

	// JAM SIBUK
	printf("\n\n======= SIMULASI JAM SIBUK =======");
	int tambahanAngkot[MAX_SEKTOR] = {0}, tambahanBus[MAX_SEKTOR] = {0};
	int totalKebutuhan = 0, totalTerpenuhi = 0;
	int demandBaru[MAX_SEKTOR];

	for (int i = 0; i < jumlahSektor; i++)
	{
		printf("\nSEKTOR %s\n", sektor[i].nama);
		printf("Demand baru: ");
		scanf("%d", &demandBaru[i]);
	}

	for (int i = 0; i < jumlahSektor; i++)
	{
		sektor[i].demand = demandBaru[i]; // Update demand
		int kapasAngkot = kapasitas(ANGKOT);
		int kapasBus = kapasitas(BUS);

		int kapasAwal = sektor[i].angkot_dialokasikan * kapasAngkot + sektor[i].bus_dialokasikan * kapasBus;
		int kekurangan = demandBaru[i] - kapasAwal;

		totalKebutuhan += demandBaru[i];
		totalTerpenuhi += kapasAwal;

		if (kekurangan <= 0)
			continue; // Sudah cukup

		tambahanBus[i] = 0; // Reset tambahan setiap sektor
		tambahanAngkot[i] = 0;

		for (int j = 0; j < jumlahSektor; j++)
		{

			if (i == j)
				continue;

			int kapasJ = sektor[j].angkot_dialokasikan * kapasAngkot + sektor[j].bus_dialokasikan * kapasBus;
			int kelebihan = kapasJ - sektor[j].demand; // gunakan sektor[j].demand yg baru juga!

			if (kelebihan <= 0)
				continue;

			// Coba alihkan BUS dulu
			if (sektor[i].jalan_raya == 'y' && sektor[j].bus_dialokasikan > 0)
			{
				int maxDialihkan = kelebihan / kapasBus;
				int butuhBus = (kekurangan + kapasBus - 1) / kapasBus;
				int temp = (maxDialihkan < sektor[j].bus_dialokasikan) ? maxDialihkan : sektor[j].bus_dialokasikan;
				int dialihkan = (butuhBus < temp) ? butuhBus : temp;

				tambahanBus[i] += dialihkan;
				sektor[j].bus_dialokasikan -= dialihkan;
				kekurangan -= dialihkan * kapasBus;
				totalTerpenuhi += dialihkan * kapasBus;
			}

			// Lalu angkot jika masih kurang
			if (kekurangan > 0 && sektor[j].angkot_dialokasikan > 0)
			{
				// Hitung ulang kapasitas sektor j untuk angkot
				kapasJ = sektor[j].angkot_dialokasikan * kapasAngkot + sektor[j].bus_dialokasikan * kapasBus;
				kelebihan = kapasJ - sektor[j].demand;
				if (kelebihan <= 0)
					continue;

				int maxDialihkan = kelebihan / kapasAngkot;
				int butuhAngkot = (kekurangan + kapasAngkot - 1) / kapasAngkot;
				int temp2 = (maxDialihkan < sektor[j].angkot_dialokasikan) ? maxDialihkan : sektor[j].angkot_dialokasikan;
				int dialihkan = (butuhAngkot < temp2) ? butuhAngkot : temp2;

				tambahanAngkot[i] += dialihkan;
				sektor[j].angkot_dialokasikan -= dialihkan;
				kekurangan -= dialihkan * kapasAngkot;
				totalTerpenuhi += dialihkan * kapasAngkot;
			}

			if (kekurangan <= 0)
				break;
		}
		sektor[i].bus_dialokasikan += tambahanBus[i];
		sektor[i].angkot_dialokasikan += tambahanAngkot[i];
	}

	printf("\n======= DISTRIBUSI TAMBAHAN JAM SIBUK =======\n");
	for (int i = 0; i < jumlahSektor; i++)
	{
		printf("%s:\n", sektor[i].nama);
		printf("  - Tambahan Bus   : %d\n", tambahanBus[i]);
		printf("  - Tambahan Angkot: %d\n", tambahanAngkot[i]);
	}

	for (int i = 0; i < jumlahSektor; i++)
	{
		printf("SEKTOR %d (%s)\n", i + 1, sektor[i].nama);
		printf("  - Jarak				: %.1f km\n", sektor[i].jarak);
		printf("  - Demand				: %d orang\n", sektor[i].demand);
		printf("  - Bus 				: %d\n", sektor[i].bus_dialokasikan);
		printf("  - Angkot				: %d\n", sektor[i].angkot_dialokasikan);
		printf("  - Kapasitas total 	: %d orang\n", (sektor[i].bus_dialokasikan + tambahanBus[i]) * 20 + (sektor[i].angkot_dialokasikan + tambahanAngkot[i]) * 7);
		printf("  - Tipe kendaraan 		: %s\n", tipeToStr(sektor[i].tipe));
		printf("--------------------------------------\n");
	}

	printf("\n======= EFEKTIVITAS DISTRIBUSI =======\n");
	float efektivitas = (float)totalTerpenuhi / totalKebutuhan * 100.0;
	printf("  Permintaan total	: %d orang\n", totalKebutuhan);
	printf("  Terpenuhi total 	: %d orang\n", totalTerpenuhi);
	printf("  Efektivitas     	: %.2f%%\n", efektivitas);

	printf("\nSISA KENDARAAN DI DEPO:\n  - Bus   : %d\n  - Angkot: %d\n", jumlahBus, jumlahAngkot);

	return 0;
}
