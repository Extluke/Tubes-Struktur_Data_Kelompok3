#ifndef QUEUE_STACK_H_INCLUDED
#define QUEUE_STACK_H_INCLUDED

#include <iostream>
#include <string>

using namespace std;

#define Nil NULL

/* ========================================================================= */
/*                          DEFINISI TIPE DATA & STRUKTUR                    */
/* ========================================================================= */

/* --- 1. ADT Queue Inspeksi & Pencucian (Pointer / FIFO - Challenge 8) --- */
struct infotypeQueue {
    string namaPenyewa;   /* Nama penyewa yang mengembalikan alat */
    string ringkasanAlat; /* Ringkasan/daftar nama alat yang dikembalikan */
    string statusCek;     /* Status: "Menunggu Cek", "Sedang Dicuci", "Selesai Bersih" */
};

typedef struct ElmQueue *adrQueue;

struct ElmQueue {
    infotypeQueue info;   /* Informasi data antrean inspeksi */
    adrQueue next;        /* Pointer ke elemen antrean berikutnya */
};

struct QueueInspeksi {
    adrQueue head;        /* Pointer ke elemen terdepan antrean (keluar pertama) */
    adrQueue tail;        /* Pointer ke elemen terbelakang antrean (masuk terakhir) */
};


/* --- 2. ADT Stack Riwayat Transaksi Selesai (Pointer / LIFO - Challenge 9) --- */
struct infotypeStack {
    string namaPenyewa;   /* Nama penyewa transaksi yang telah selesai */
    string kategoriPaket; /* Paket yang disewa */
    int durasiSewa;       /* Total durasi sewa (hari) */
    long long totalBiaya; /* Total nominal pembayaran sewa (Rupiah) */
};

typedef struct ElmStack *adrStack;

struct ElmStack {
    infotypeStack info;   /* Informasi data riwayat transaksi */
    adrStack next;        /* Pointer ke elemen di bawahnya dalam stack */
};

struct StackRiwayat {
    adrStack Top;         /* Pointer ke elemen teratas stack (LIFO) */
};

/* ========================================================================= */
/*                       PROTOTIPE PRIMITIF & SPESIFIKASI                    */
/* ========================================================================= */

/* ----------------- Primitif Queue Inspeksi (FIFO) ----------------- */

/* I.S. Q sembarang
   F.S. Terbentuk Queue Q yang kosong dengan Q.head = Nil dan Q.tail = Nil */
void createQueue(QueueInspeksi &Q);

/* Mengirimkan true jika antrean Q kosong (Q.head == Nil), false jika tidak */
bool isEmptyQueue(QueueInspeksi Q);

/* Mengalokasikan memori dinamis untuk elemen queue baru dengan info x dan next = Nil.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika alokasi gagal */
adrQueue alokasiQueue(infotypeQueue x);

/* I.S. P terdefinisi dan tidak Nil
   F.S. Memori yang ditempati oleh node P dibebaskan, P bernilai Nil */
void dealokasiQueue(adrQueue &P);

/* I.S. Q terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Elemen P disisipkan ke posisi paling belakang (tail) antrean Q */
void enQueue(QueueInspeksi &Q, adrQueue P);

/* I.S. Q terdefinisi dan tidak kosong
   F.S. Elemen terdepan (head) antrean dikeluarkan dan disimpan ke P.
        Head bergeser ke elemen berikutnya. Jika antrean menjadi kosong, tail = Nil */
void deQueue(QueueInspeksi &Q, adrQueue &P);

/* I.S. Q terdefinisi (mungkin kosong)
   F.S. Menampilkan seluruh data antrean inspeksi dan pencucian alat dari posisi head hingga tail */
void printQueue(QueueInspeksi Q);

/* I.S. Q terdefinisi, mungkin kosong
   F.S. Seluruh elemen antrean pada Q didealokasikan sampai Q kosong (head = Nil, tail = Nil) */
void delAllQueue(QueueInspeksi &Q);


/* ----------------- Primitif Stack Riwayat (LIFO) ----------------- */

/* I.S. S sembarang
   F.S. Terbentuk Stack S yang kosong dengan S.Top = Nil */
void createStack(StackRiwayat &S);

/* Mengirimkan true jika stack S kosong (S.Top == Nil), false jika tidak */
bool isEmptyStack(StackRiwayat S);

/* Mengalokasikan memori dinamis untuk elemen stack baru dengan info x dan next = Nil.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika alokasi gagal */
adrStack alokasiStack(infotypeStack x);

/* I.S. P terdefinisi dan tidak Nil
   F.S. Memori yang ditempati oleh node P dibebaskan, P bernilai Nil */
void dealokasiStack(adrStack &P);

/* I.S. S terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Elemen P ditambahkan menjadi elemen teratas (Top) pada Stack S */
void push(StackRiwayat &S, adrStack P);

/* I.S. S terdefinisi dan tidak kosong
   F.S. Elemen teratas Stack S dikeluarkan dan disimpan ke P.
        Top bergeser ke elemen di bawahnya */
void pop(StackRiwayat &S, adrStack &P);

/* Mengirimkan address elemen teratas (Top) dari Stack S tanpa menghapusnya.
   Mengembalikan Nil jika Stack S kosong */
adrStack peekTop(StackRiwayat S);

/* I.S. S terdefinisi (mungkin kosong)
   F.S. Menampilkan seluruh riwayat transaksi yang tersimpan di Stack S dari Top ke Bottom */
void printStack(StackRiwayat S);

/* I.S. S terdefinisi, mungkin kosong
   F.S. Seluruh elemen stack pada S didealokasikan sampai S kosong (S.Top = Nil) */
void delAllStack(StackRiwayat &S);

#endif // QUEUE_STACK_H_INCLUDED
