#include "queue_stack.h"
#include <iomanip>

/* ========================================================================= */
/*                         FUNGSI UTILITAS BANTU INTERNAL                    */
/* ========================================================================= */

/* Format mata uang Rupiah untuk estetika output */
static string formatRupiah(long long nominal) {
    string s = to_string(nominal);
    int n = s.length() - 3;
    while (n > 0) {
        s.insert(n, ".");
        n -= 3;
    }
    return "Rp " + s;
}


/* ========================================================================= */
/*       1. PRIMITIF QUEUE INSPEKSI & PENCUCIAN (FIFO - CHALLENGE 8)         */
/* ========================================================================= */

/* I.S. Q sembarang
   F.S. Terbentuk Queue Q yang kosong dengan Q.head = Nil dan Q.tail = Nil */
void createQueue(QueueInspeksi &Q) {
    /* Logika: Mengatur kedua pointer batas antrean (head dan tail) ke Nil */
    Q.head = Nil;
    Q.tail = Nil;
}

/* Mengirimkan true jika antrean Q kosong (Q.head == Nil), false jika tidak */
bool isEmptyQueue(QueueInspeksi Q) {
    /* Logika: Antrean kosong ditandai dengan head menunjuk ke Nil */
    return (Q.head == Nil);
}

/* Mengalokasikan memori dinamis untuk elemen queue baru dengan info x dan next = Nil.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika alokasi gagal */
adrQueue alokasiQueue(infotypeQueue x) {
    /* Logika: Meminta alokasi memori dinamis di heap untuk elemen ElmQueue */
    adrQueue P = new ElmQueue;
    if (P != Nil) {
        P->info = x;
        P->next = Nil;
    }
    return P;
}

/* I.S. P terdefinisi dan tidak Nil
   F.S. Memori yang ditempati oleh node P dibebaskan, P bernilai Nil */
void dealokasiQueue(adrQueue &P) {
    /* Logika: Menghapus memori elemen antrean dengan delete dan mereset pointer ke Nil */
    if (P != Nil) {
        delete P;
        P = Nil;
    }
}

/* I.S. Q terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Elemen P disisipkan ke posisi paling belakang (tail) antrean Q */
void enQueue(QueueInspeksi &Q, adrQueue P) {
    /* Logika:
       - Jika antrean kosong (isEmptyQueue): P menjadi elemen pertama sekaligus terakhir (head = P, tail = P).
       - Jika antrean sudah memiliki elemen: sambungkan elemen tail saat ini ke P (Q.tail->next = P),
         lalu geser pointer tail ke elemen P yang baru masuk (Q.tail = P).
       - Pastikan P->next menunjuk ke Nil. */
    if (P == Nil) return;

    P->next = Nil;
    if (isEmptyQueue(Q)) {
        Q.head = P;
        Q.tail = P;
    } else {
        Q.tail->next = P;
        Q.tail = P;
    }
    cout << "[BERHASIL ENQUEUE] Pengembalian alat atas nama \"" << P->info.namaPenyewa 
         << "\" berhasil dimasukkan ke antrean inspeksi & pencucian.\n";
}

/* I.S. Q terdefinisi dan tidak kosong
   F.S. Elemen terdepan (head) antrean dikeluarkan dan disimpan ke P.
        Head bergeser ke elemen berikutnya. Jika antrean menjadi kosong, tail = Nil */
void deQueue(QueueInspeksi &Q, adrQueue &P) {
    /* Logika:
       - Jika antrean kosong: P diset Nil dan tampilkan peringatan.
       - Jika antrean terisi:
         * Simpan alamat node head ke P.
         * Geser pointer head ke elemen berikutnya (Q.head = Q.head->next).
         * Jika setelah bergeser head menjadi Nil (antrean habis), maka tail juga diset Nil.
         * Putuskan tautan pointer next dari node yang dikeluarkan (P->next = Nil).
         * Tampilkan notifikasi bahwa perlengkapan telah selesai diperiksa dan dicuci. */
    if (isEmptyQueue(Q)) {
        P = Nil;
        cout << "[PERINGATAN] Antrean inspeksi kosong! Tidak ada perlengkapan yang perlu diproses.\n";
    } else {
        P = Q.head;
        Q.head = Q.head->next;
        if (Q.head == Nil) {
            Q.tail = Nil;
        }
        P->next = Nil;

        cout << "\n========================================================================\n";
        cout << "                  PROSES INSPEKSI & PENCUCIAN SELESAI                   \n";
        cout << "========================================================================\n";
        cout << " Penyewa      : " << P->info.namaPenyewa << "\n";
        cout << " Rincian Alat : " << P->info.ringkasanAlat << "\n";
        cout << " Status Akhir : Selesai Bersih & Siap Disewakan Kembali\n";
        cout << "========================================================================\n";
    }
}

/* I.S. Q terdefinisi (mungkin kosong)
   F.S. Menampilkan seluruh data antrean inspeksi dan pencucian alat dari posisi head hingga tail */
void printQueue(QueueInspeksi Q) {
    /* Logika:
       - Traversal linear dari Q.head hingga Q.tail (curr != Nil).
       - Tampilkan nomor urut antrean, nama penyewa, ringkasan alat, dan status pemeriksaan. */
    cout << "\n========================================================================\n";
    cout << "             DAFTAR ANTREAN INSPEKSI & PENCUCIAN ALAT (FIFO)            \n";
    cout << "========================================================================\n";

    if (isEmptyQueue(Q)) {
        cout << " [INFO] Antrean inspeksi saat ini KOSONG (Semua alat telah selesai diproses).\n";
        cout << "========================================================================\n";
        return;
    }

    adrQueue curr = Q.head;
    int nomor = 1;
    while (curr != Nil) {
        cout << " Antrean ke-" << nomor << " (Posisi: " << (curr == Q.head ? "HEAD / Terdepan" : (curr == Q.tail ? "TAIL / Terakhir" : "Menunggu")) << ")\n";
        cout << "   - Nama Penyewa   : " << curr->info.namaPenyewa << "\n";
        cout << "   - Perlengkapan   : " << curr->info.ringkasanAlat << "\n";
        cout << "   - Status Saat Ini: " << curr->info.statusCek << "\n";
        cout << "------------------------------------------------------------------------\n";
        nomor++;
        curr = curr->next;
    }
    cout << " Total Antrean Menunggu : " << (nomor - 1) << " Pengembalian\n";
    cout << "========================================================================\n";
}

/* I.S. Q terdefinisi, mungkin kosong
   F.S. Seluruh elemen antrean pada Q didealokasikan sampai Q kosong (head = Nil, tail = Nil) */
void delAllQueue(QueueInspeksi &Q) {
    /* Logika: Dequeue seluruh elemen satu per satu hingga antrean kosong dan bebaskan memorinya */
    adrQueue P = Nil;
    while (!isEmptyQueue(Q)) {
        P = Q.head;
        Q.head = Q.head->next;
        dealokasiQueue(P);
    }
    Q.tail = Nil;
}


/* ========================================================================= */
/*         2. PRIMITIF STACK RIWAYAT TRANSAKSI (LIFO - CHALLENGE 9)          */
/* ========================================================================= */

/* I.S. S sembarang
   F.S. Terbentuk Stack S yang kosong dengan S.Top = Nil */
void createStack(StackRiwayat &S) {
    /* Logika: Inisialisasi pointer puncak stack (Top) menjadi Nil */
    S.Top = Nil;
}

/* Mengirimkan true jika stack S kosong (S.Top == Nil), false jika tidak */
bool isEmptyStack(StackRiwayat S) {
    /* Logika: Stack kosong jika Top menunjuk ke Nil */
    return (S.Top == Nil);
}

/* Mengalokasikan memori dinamis untuk elemen stack baru dengan info x dan next = Nil.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika alokasi gagal */
adrStack alokasiStack(infotypeStack x) {
    /* Logika: Menggunakan new untuk mengalokasikan simpul ElmStack pada heap */
    adrStack P = new ElmStack;
    if (P != Nil) {
        P->info = x;
        P->next = Nil;
    }
    return P;
}

/* I.S. P terdefinisi dan tidak Nil
   F.S. Memori yang ditempati oleh node P dibebaskan, P bernilai Nil */
void dealokasiStack(adrStack &P) {
    /* Logika: Dealokasi memori heap dengan delete dan ubah pointer menjadi Nil */
    if (P != Nil) {
        delete P;
        P = Nil;
    }
}

/* I.S. S terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Elemen P ditambahkan menjadi elemen teratas (Top) pada Stack S */
void push(StackRiwayat &S, adrStack P) {
    /* Logika:
       - Sambungkan pointer next dari elemen baru P ke elemen Top saat ini (P->next = S.Top).
       - Jadikan elemen P sebagai Top yang baru (S.Top = P). */
    if (P == Nil) return;

    P->next = S.Top;
    S.Top = P;
    cout << "[BERHASIL LOG RIWAYAT] Transaksi penyewa \"" << P->info.namaPenyewa 
         << "\" berhasil dicatat ke puncak Stack Riwayat.\n";
}

/* I.S. S terdefinisi dan tidak kosong
   F.S. Elemen teratas Stack S dikeluarkan dan disimpan ke P.
        Top bergeser ke elemen di bawahnya */
void pop(StackRiwayat &S, adrStack &P) {
    /* Logika:
       - Jika stack kosong: P diset Nil dan tampilkan pesan kesalahan.
       - Jika stack terisi:
         * Simpan alamat Top saat ini ke pointer P.
         * Geser pointer Top ke elemen di bawahnya (S.Top = S.Top->next).
         * Putuskan pointer next milik P (P->next = Nil). */
    if (isEmptyStack(S)) {
        P = Nil;
        cout << "[PERINGATAN] Stack riwayat kosong! Tidak ada transaksi yang dapat dikeluarkan.\n";
    } else {
        P = S.Top;
        S.Top = S.Top->next;
        P->next = Nil;
        cout << "[BERHASIL POP] Riwayat transaksi atas nama \"" << P->info.namaPenyewa 
             << "\" berhasil dikeluarkan dari puncak riwayat.\n";
    }
}

/* Mengirimkan address elemen teratas (Top) dari Stack S tanpa menghapusnya.
   Mengembalikan Nil jika Stack S kosong */
adrStack peekTop(StackRiwayat S) {
    /* Logika:
       - Periksa apakah stack kosong.
       - Jika kosong, kembalikan Nil.
       - Jika terisi, cetak informasi transaksi puncak dan kembalikan S.Top tanpa mengubah struktur stack. */
    if (isEmptyStack(S)) {
        cout << "\n[INFO] Stack riwayat transaksi masih kosong, belum ada transaksi selesai.\n";
        return Nil;
    }

    adrStack P = S.Top;
    cout << "\n========================================================================\n";
    cout << "               TRANSAKSI SELESAI PALING TERAKHIR (PEEK TOP)             \n";
    cout << "========================================================================\n";
    cout << " Nama Penyewa    : " << P->info.namaPenyewa << "\n";
    cout << " Kategori Paket  : " << P->info.kategoriPaket << "\n";
    cout << " Durasi Sewa     : " << P->info.durasiSewa << " Hari\n";
    cout << " Total Transaksi : " << formatRupiah(P->info.totalBiaya) << "\n";
    cout << "========================================================================\n";

    return P;
}

/* I.S. S terdefinisi (mungkin kosong)
   F.S. Menampilkan seluruh riwayat transaksi yang tersimpan di Stack S dari Top ke Bottom */
void printStack(StackRiwayat S) {
    /* Logika:
       - Traversal linear dari elemen S.Top menuruni stack melalui pointer next hingga Nil.
       - Akumulasikan nominal totalBiaya untuk menghitung total pendapatan keseluruhan sistem. */
    cout << "\n========================================================================\n";
    cout << "            LOG RIWAYAT TRANSAKSI SELESAI TERBARU (LIFO STACK)          \n";
    cout << "========================================================================\n";

    if (isEmptyStack(S)) {
        cout << " [INFO] Riwayat transaksi masih KOSONG (Belum ada penyewaan yang diselesaikan).\n";
        cout << "========================================================================\n";
        return;
    }

    adrStack curr = S.Top;
    int urutan = 1;
    long long grandTotalPendapatan = 0;

    while (curr != Nil) {
        cout << " Log #" << urutan << " (Posisi: " << (curr == S.Top ? "TOP / Terbaru" : "Riwayat Lampau") << ")\n";
        cout << "   - Nama Penyewa  : " << curr->info.namaPenyewa << "\n";
        cout << "   - Paket Sewa    : " << curr->info.kategoriPaket << "\n";
        cout << "   - Durasi Sewa   : " << curr->info.durasiSewa << " Hari\n";
        cout << "   - Nominal Biaya : " << formatRupiah(curr->info.totalBiaya) << "\n";
        cout << "------------------------------------------------------------------------\n";

        grandTotalPendapatan += curr->info.totalBiaya;
        urutan++;
        curr = curr->next;
    }

    cout << " Total Transaksi Selesai       : " << (urutan - 1) << " Transaksi\n";
    cout << " TOTAL PENDAPATAN KESELURUHAN  : " << formatRupiah(grandTotalPendapatan) << "\n";
    cout << "========================================================================\n";
}

/* I.S. S terdefinisi, mungkin kosong
   F.S. Seluruh elemen stack pada S didealokasikan sampai S kosong (S.Top = Nil) */
void delAllStack(StackRiwayat &S) {
    /* Logika: Iterasi pop dan bebaskan memori seluruh simpul stack hingga S.Top bernilai Nil */
    adrStack P = Nil;
    while (!isEmptyStack(S)) {
        P = S.Top;
        S.Top = S.Top->next;
        dealokasiStack(P);
    }
}
