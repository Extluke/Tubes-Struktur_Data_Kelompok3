#ifndef BST_MLL_H_INCLUDED
#define BST_MLL_H_INCLUDED

#include <iostream>
#include <string>

using namespace std;

#define Nil NULL

/* ========================================================================= */
/*                          DEFINISI TIPE DATA & STRUKTUR                    */
/* ========================================================================= */

/* --- ADT Perlengkapan Outdoor (Child Singly Linked List) --- */
struct infotypeAlat {
    string namaAlat;      /* Nama perlengkapan camping/outdoor */
    int jumlah;           /* Jumlah unit perlengkapan yang disewa */
    int hargaPerHari;     /* Harga sewa per unit per hari (Rupiah) */
};

typedef struct ElmAlat *adrAlat;

struct ElmAlat {
    infotypeAlat info;    /* Informasi data alat */
    adrAlat next;         /* Pointer ke elemen alat berikutnya */
};

struct ListAlat {
    adrAlat first;        /* Pointer ke elemen pertama list alat */
};

/* --- ADT Penyewa (Node Binary Search Tree) --- */
struct infotypePenyewa {
    string namaPenyewa;   /* Kunci unik (key BST), terurut alfabetis */
    int durasiSewa;       /* Lama waktu sewa dalam satuan hari */
    string kategoriPaket; /* Kategori: "Solo Camp", "Family Camp", "Hiking/Pendakian", "Ekspedisi" */
};

typedef struct NodePenyewa *adrPenyewa;

struct NodePenyewa {
    infotypePenyewa info; /* Informasi data penyewa */
    ListAlat daftarAlat;  /* Sub-list (MLL) berisi perlengkapan yang disewa */
    adrPenyewa left;      /* Subtree kiri (namaPenyewa lebih kecil) */
    adrPenyewa right;     /* Subtree kanan (namaPenyewa lebih besar) */
};

typedef adrPenyewa BinTree;

/* --- ADT Queue Bantu Khusus Level Order BST (Manual Pointer-Based, Non-STL) --- */
typedef struct ElmQueueBST *adrQueueBST;

struct ElmQueueBST {
    adrPenyewa nodeTree;  /* Pointer node BST yang disimpan dalam antrean */
    adrQueueBST next;     /* Pointer ke elemen antrean berikutnya */
};

struct QueueBST {
    adrQueueBST head;     /* Elemen terdepan antrean */
    adrQueueBST tail;     /* Elemen terbelakang antrean */
};

/* ========================================================================= */
/*                       PROTOTIPE PRIMITIF & SPESIFIKASI                    */
/* ========================================================================= */

/* ----------------- 1. Manajemen List & Tree (Inisialisasi & Alokasi) ----------------- */

/* I.S. T sembarang
   F.S. Terbentuk Binary Search Tree T yang kosong (T = Nil) */
void createTree(BinTree &T);

/* Mengirimkan true jika Binary Search Tree T kosong (T == Nil), false jika tidak */
bool isEmptyTree(BinTree T);

/* I.S. L sembarang
   F.S. Terbentuk ListAlat L yang kosong (L.first = Nil) */
void createListAlat(ListAlat &L);

/* Mengirimkan true jika ListAlat L kosong (L.first == Nil), false jika tidak */
bool isEmptyListAlat(ListAlat L);

/* Mengalokasikan memori dinamis untuk elemen alat baru.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika gagal */
adrAlat alokasiAlat(infotypeAlat x);

/* I.S. P terdefinisi dan tidak Nil
   F.S. Memori yang dialokasikan untuk node P dibebaskan, P bernilai Nil */
void dealokasiAlat(adrAlat &P);

/* Mengalokasikan memori dinamis untuk node penyewa baru dengan daftarAlat kosong, left = Nil, right = Nil.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika gagal */
adrPenyewa alokasiPenyewa(infotypePenyewa x);

/* I.S. P terdefinisi dan tidak Nil
   F.S. Seluruh daftarAlat milik P dihapus, memori node P dibebaskan, P bernilai Nil */
void dealokasiPenyewa(adrPenyewa &P);

/* I.S. L terdefinisi, mungkin kosong
   F.S. Semua elemen pada ListAlat L didealokasikan hingga L kosong (L.first = Nil) */
void delAllAlat(ListAlat &L);

/* I.S. T terdefinisi, mungkin kosong
   F.S. Seluruh node pada Tree T beserta child list alat didealokasikan secara post-order hingga T = Nil */
void delAllTree(BinTree &T);


/* ----------------- 2. Insert & Update (Challenge 1 & 2) ----------------- */

/* I.S. T terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Node P disisipkan ke dalam Binary Search Tree T sesuai aturan BST berdasarkan info.namaPenyewa (case-insensitive).
        Jika namaPenyewa sudah ada (duplikat), tampilkan pesan kesalahan dan batalkan alokasi */
void insertNodePenyewa(BinTree &T, adrPenyewa P);

/* I.S. L terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Node alat P ditambahkan pada posisi terakhir (insert last) pada ListAlat L */
void insertLastAlat(ListAlat &L, adrAlat P);

/* I.S. T terdefinisi, nama dan durasiBaru terdefinisi
   F.S. Jika penyewa dengan namaPenyewa == nama ditemukan, update durasiSewa menjadi durasiBaru.
        Jika tidak ditemukan, tampilkan pesan error bahwa penyewa tidak ditemukan */
void updateDurasi(BinTree &T, string nama, int durasiBaru);

/* I.S. T terdefinisi, nama dan kategoriBaru terdefinisi
   F.S. Jika penyewa ditemukan dan kategoriBaru valid ("Solo Camp", "Family Camp", "Hiking/Pendakian", "Ekspedisi"),
        update kategoriPaket penyewa. Jika tidak valid/ditemukan, tampilkan pesan peringatan */
void updateKategori(BinTree &T, string nama, string kategoriBaru);


/* ----------------- 3. Traversal BST & Display (Challenge 3) ----------------- */

/* I.S. P terdefinisi (tidak Nil)
   F.S. Menampilkan informasi detail penyewa secara rapi dan seragam berbingkai garis pemisah,
        meliputi Nama, Kategori Paket, Durasi Sewa, Rincian Daftar Alat (MLL), dan Estimasi Total Biaya */
void printDetailPenyewa(adrPenyewa P);

/* I.S. P terdefinisi (tidak Nil)
   F.S. Menampilkan informasi penyewa (memanggil printDetailPenyewa) */
void printPenyewa(adrPenyewa P);

/* I.S. L terdefinisi (mungkin kosong)
   F.S. Menampilkan seluruh data alat yang ada di ListAlat L ke layar */
void printListAlat(ListAlat L);

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data seluruh penyewa secara In-Order (Left - Root - Right), terurut alfabetis A-Z */
void printInOrder(BinTree T);

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data seluruh penyewa secara Pre-Order (Root - Left - Right) */
void printPreOrder(BinTree T);

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data seluruh penyewa secara Post-Order (Left - Right - Root) */
void printPostOrder(BinTree T);

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data penyewa level demi level dari atas ke bawah, kiri ke kanan (BFS)
        menggunakan antrean bantu berbasis pointer manual QueueBST (Strictly No-STL) */
void printLevelOrder(BinTree T);


/* ----------------- 4. Search / Pencarian (Challenge 4) ----------------- */

/* Mengirimkan address node penyewa dengan info.namaPenyewa == nama (case-insensitive).
   Mengembalikan Nil jika data penyewa tidak ditemukan */
adrPenyewa findNodePenyewa(BinTree T, string nama);

/* I.S. T dan namaAlat terdefinisi
   F.S. Menampilkan daftar nama penyewa dan jumlah unit yang menyewa perlengkapan namaAlat.
        Jika tidak ada penyewa yang meminjam alat tersebut, tampilkan pesan informasi */
void cariByAlat(BinTree T, string namaAlat);

/* I.S. T dan durasi terdefinisi
   F.S. Menampilkan seluruh penyewa yang memiliki durasiSewa sama dengan durasi */
void cariByDurasi(BinTree T, int durasi);


/* ----------------- 5. Delete / Penghapusan (Challenge 5, 6, 7) ----------------- */

/* Mengirimkan address node dengan nilai nama terkecil (paling kiri) pada subtree dengan akar root.
   Digunakan sebagai successor saat menghapus node dengan 2 anak pada BST */
adrPenyewa mostLeft(adrPenyewa root);

/* I.S. T terdefinisi, nama terdefinisi
   F.S. Jika node dengan namaPenyewa == nama ditemukan, node tersebut dihapus dari BST T
        (menangani kasus: node daun, node 1 anak, atau node 2 anak menggunakan mostLeft successor).
        Memori node beserta daftar alatnya didealokasikan */
void deleteNodePenyewa(BinTree &T, string nama);

/* I.S. T dan namaPenyewa terdefinisi
   F.S. Jika penyewa ditemukan, durasiSewa miliknya direset menjadi 0.
        Jika tidak ditemukan, tampilkan pesan peringatan */
void hapusDurasiPenyewa(BinTree &T, string namaPenyewa);

/* I.S. T dan batasDurasi terdefinisi
   F.S. Seluruh penyewa yang memiliki durasiSewa == batasDurasi direset durasinya menjadi 0 */
void hapusDurasiPenyewa(BinTree &T, int batasDurasi);

/* I.S. T, namaPenyewa, dan namaAlat terdefinisi
   F.S. Jika penyewa ditemukan, alat dengan namaAlat dihapus dari sub-list daftarAlat milik penyewa tersebut.
        Jika alat tidak ditemukan dalam list penyewa, tampilkan pesan pemberitahuan */
void hapusAlatDariPenyewa(BinTree T, string namaPenyewa, string namaAlat);


/* ----------------- 6. Statistika & Analisis (Challenge 10, 11, 12, 13) ----------------- */

/* Menghitung dan mengembalikan total banyaknya penyewa yang tercatat di dalam BST T */
int hitungTotalPenyewa(BinTree T);

/* Menghitung dan mengembalikan total kuantitas unit perlengkapan dalam ListAlat L */
int hitungJumlahAlat(ListAlat L);

/* Menghitung dan mengembalikan jumlah jenis perlengkapan (banyaknya node dalam ListAlat L) */
int hitungJumlahJenisAlat(ListAlat L);

/* Menghitung total estimasi biaya sewa milik seorang penyewa P:
   Total = sum(jumlah * hargaPerHari) * durasiSewa.
   Mengembalikan 0 jika P == Nil atau list alat kosong */
long long hitungTotalBiaya(adrPenyewa P);

/* I.S. T dan kategori terdefinisi
   F.S. Menampilkan daftar penyewa dalam kategoriPaket tertentu dan mencetak total jumlah penyewanya */
void viewDanHitungByKategori(BinTree T, string kategori);

/* I.S. T terdefinisi
   F.S. Menampilkan ringkasan statistik durasi sewa dari seluruh penyewa pada BST T:
        durasi terlama (maksimum), durasi tercepat (minimum), dan rata-rata durasi sewa */
void tampilkanStatistikDurasi(BinTree T);

/* I.S. T dan targetJumlah terdefinisi
   F.S. Menghitung jumlah node di child MLL tiap penyewa, lalu menampilkan penyewa
        yang memiliki jumlah jenis perlengkapan tepat sesuai targetJumlah */
void cariByJumlahAlat(BinTree T, int targetJumlah);


/* ----------------- 7. Primitif Queue Bantu Level Order BST (No-STL) ----------------- */

/* I.S. Q sembarang
   F.S. Terbentuk QueueBST Q yang kosong (Q.head = Nil, Q.tail = Nil) */
void createQueueBST(QueueBST &Q);

/* Mengirimkan true jika QueueBST Q kosong, false jika tidak */
bool isEmptyQueueBST(QueueBST Q);

/* I.S. Q terdefinisi, nodeTree terdefinisi
   F.S. Node tree nodeTree disisipkan ke bagian belakang (tail) QueueBST Q */
void enqueueBST(QueueBST &Q, adrPenyewa nodeTree);

/* I.S. Q terdefinisi dan tidak kosong
   F.S. Node tree terdepan (head) diambil dan disimpan pada nodeTree, elemen antrean didealokasi */
void dequeueBST(QueueBST &Q, adrPenyewa &nodeTree);

/* I.S. Q terdefinisi, mungkin kosong
   F.S. Seluruh elemen antrean pada QueueBST didealokasi hingga antrean kosong */
void delAllQueueBST(QueueBST &Q);

#endif // BST_MLL_H_INCLUDED
