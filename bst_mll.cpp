#include "bst_mll.h"
#include <iomanip>

/* ========================================================================= */
/*                         FUNGSI UTILITAS BANTU INTERNAL                    */
/* ========================================================================= */

/* Mengubah seluruh karakter string menjadi huruf kecil (case-insensitive) */
static string toLowerStr(string str) {
    string res = str;
    for (size_t i = 0; i < res.length(); i++) {
        if (res[i] >= 'A' && res[i] <= 'Z') {
            res[i] = res[i] + 32;
        }
    }
    return res;
}

/* Memvalidasi apakah kategori paket termasuk ke dalam 4 kategori yang diizinkan */
static bool isValidKategori(string kat) {
    string k = toLowerStr(kat);
    return (k == "solo camp" || 
            k == "family camp" || 
            k == "hiking/pendakian" || 
            k == "ekspedisi");
}

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
/*             1. MANAJEMEN LIST & TREE (INISIALISASI & ALOKASI)             */
/* ========================================================================= */

/* I.S. T sembarang
   F.S. Terbentuk Binary Search Tree T yang kosong (T = Nil) */
void createTree(BinTree &T) {
    /* Logika: Inisialisasi pointer akar (root) pohon menjadi Nil */
    T = Nil;
}

/* Mengirimkan true jika Binary Search Tree T kosong (T == Nil), false jika tidak */
bool isEmptyTree(BinTree T) {
    return (T == Nil);
}

/* I.S. L sembarang
   F.S. Terbentuk ListAlat L yang kosong (L.first = Nil) */
void createListAlat(ListAlat &L) {
    /* Logika: Inisialisasi pointer first dari child singly linked list menjadi Nil */
    L.first = Nil;
}

/* Mengirimkan true jika ListAlat L kosong (L.first == Nil), false jika tidak */
bool isEmptyListAlat(ListAlat L) {
    return (L.first == Nil);
}

/* Mengalokasikan memori dinamis untuk elemen alat baru.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika gagal */
adrAlat alokasiAlat(infotypeAlat x) {
    /* Logika: Menggunakan operator new untuk alokasi memori heap ElmAlat */
    adrAlat P = new ElmAlat;
    if (P != Nil) {
        P->info = x;
        P->next = Nil;
    }
    return P;
}

/* I.S. P terdefinisi dan tidak Nil
   F.S. Memori yang dialokasikan untuk node P dibebaskan, P bernilai Nil */
void dealokasiAlat(adrAlat &P) {
    /* Logika: Membebaskan memori heap dengan delete dan mengatur pointer ke Nil untuk menghindari dangling pointer */
    if (P != Nil) {
        delete P;
        P = Nil;
    }
}

/* Mengalokasikan memori dinamis untuk node penyewa baru dengan daftarAlat kosong, left = Nil, right = Nil.
   Mengirimkan address hasil alokasi jika berhasil, atau Nil jika gagal */
adrPenyewa alokasiPenyewa(infotypePenyewa x) {
    /* Logika: Alokasi memori heap untuk NodePenyewa dan inisialisasi child list serta kedua pointer anak */
    adrPenyewa P = new NodePenyewa;
    if (P != Nil) {
        P->info = x;
        createListAlat(P->daftarAlat);
        P->left = Nil;
        P->right = Nil;
    }
    return P;
}

/* I.S. P terdefinisi dan tidak Nil
   F.S. Seluruh daftarAlat milik P dihapus, memori node P dibebaskan, P bernilai Nil */
void dealokasiPenyewa(adrPenyewa &P) {
    /* Logika: Hapus child linked list terlebih dahulu, baru dealokasi node BST utama */
    if (P != Nil) {
        delAllAlat(P->daftarAlat);
        delete P;
        P = Nil;
    }
}

/* I.S. L terdefinisi, mungkin kosong
   F.S. Semua elemen pada ListAlat L didealokasikan hingga L kosong (L.first = Nil) */
void delAllAlat(ListAlat &L) {
    /* Logika: Iterasi traversal list dari first, simpan pointer next, hapus node saat ini */
    adrAlat curr = L.first;
    while (curr != Nil) {
        adrAlat temp = curr;
        curr = curr->next;
        dealokasiAlat(temp);
    }
    L.first = Nil;
}

/* I.S. T terdefinisi, mungkin kosong
   F.S. Seluruh node pada Tree T beserta child list alat didealokasikan secara post-order hingga T = Nil */
void delAllTree(BinTree &T) {
    /* Logika: Rekursif post-order (Left -> Right -> Root) agar anak-anak dihapus sebelum induk */
    if (T != Nil) {
        delAllTree(T->left);
        delAllTree(T->right);
        dealokasiPenyewa(T);
    }
}


/* ========================================================================= */
/*                     2. INSERT & UPDATE (CHALLENGE 1 & 2)                  */
/* ========================================================================= */

/* I.S. T terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Node P disisipkan ke dalam Binary Search Tree T sesuai aturan BST berdasarkan info.namaPenyewa (case-insensitive).
        Jika namaPenyewa sudah ada (duplikat), tampilkan pesan kesalahan dan batalkan alokasi */
void insertNodePenyewa(BinTree &T, adrPenyewa P) {
    /* Logika: 
       - Basis 1: Tree kosong (T == Nil), tempatkan node P pada T.
       - Rekursi: Bandingkan namaPenyewa secara alfabetis (case-insensitive).
         * Jika nama P < nama T, belok ke subtree kiri (T->left).
         * Jika nama P > nama T, belok ke subtree kanan (T->right).
         * Jika sama (duplikat), tampilkan peringatan dan dealokasi P agar tidak memory leak. */
    if (T == Nil) {
        T = P;
    } else {
        string namaBaru = toLowerStr(P->info.namaPenyewa);
        string namaCurrent = toLowerStr(T->info.namaPenyewa);

        if (namaBaru < namaCurrent) {
            insertNodePenyewa(T->left, P);
        } else if (namaBaru > namaCurrent) {
            insertNodePenyewa(T->right, P);
        } else {
            cout << "\n[PERINGATAN] Penyewa dengan nama \"" << P->info.namaPenyewa 
                 << "\" sudah terdaftar (Duplikat key BST tidak diperbolehkan)!\n";
            dealokasiPenyewa(P);
        }
    }
}

/* I.S. L terdefinisi (mungkin kosong), P terdefinisi dan sudah dialokasi
   F.S. Node alat P ditambahkan pada posisi terakhir (insert last) pada ListAlat L.
        Jika nama alat sudah ada pada penyewa tersebut, tambahkan jumlahnya dan dealokasi P */
void insertLastAlat(ListAlat &L, adrAlat P) {
    /* Logika:
       - Cek duplikasi alat di dalam list (case-insensitive).
       - Jika alat sudah ada, perbarui kuantitas (jumlah += P->info.jumlah), dealokasi P baru.
       - Jika belum ada dan list kosong, L.first = P.
       - Jika belum ada dan list terisi, telusuri hingga elemen terakhir (last->next == Nil), lalu sambungkan last->next = P. */
    if (P == Nil) return;

    string namaAlatBaru = toLowerStr(P->info.namaAlat);
    adrAlat curr = L.first;
    bool found = false;

    while (curr != Nil && !found) {
        if (toLowerStr(curr->info.namaAlat) == namaAlatBaru) {
            curr->info.jumlah += P->info.jumlah;
            cout << "[INFO] Alat \"" << P->info.namaAlat << "\" sudah ada. Kuantitas diperbarui menjadi " 
                 << curr->info.jumlah << " unit.\n";
            dealokasiAlat(P);
            found = true;
        }
        curr = curr->next;
    }

    if (!found) {
        if (isEmptyListAlat(L)) {
            L.first = P;
        } else {
            adrAlat last = L.first;
            while (last->next != Nil) {
                last = last->next;
            }
            last->next = P;
        }
        P->next = Nil;
    }
}

/* I.S. T terdefinisi, nama dan durasiBaru terdefinisi
   F.S. Jika penyewa dengan namaPenyewa == nama ditemukan, update durasiSewa menjadi durasiBaru.
        Jika tidak ditemukan, tampilkan pesan error bahwa penyewa tidak ditemukan */
void updateDurasi(BinTree &T, string nama, int durasiBaru) {
    /* Logika: Cari node menggunakan findNodePenyewa, jika address != Nil, modifikasi info.durasiSewa */
    adrPenyewa P = findNodePenyewa(T, nama);
    if (P != Nil) {
        P->info.durasiSewa = durasiBaru;
        cout << "[BERHASIL] Durasi sewa atas nama \"" << P->info.namaPenyewa 
             << "\" berhasil diperbarui menjadi " << durasiBaru << " hari.\n";
    } else {
        cout << "[GAGAL] Data penyewa dengan nama \"" << nama << "\" tidak ditemukan di sistem!\n";
    }
}

/* I.S. T terdefinisi, nama dan kategoriBaru terdefinisi
   F.S. Jika penyewa ditemukan dan kategoriBaru valid ("Solo Camp", "Family Camp", "Hiking/Pendakian", "Ekspedisi"),
        update kategoriPaket penyewa. Jika tidak valid/ditemukan, tampilkan pesan peringatan */
void updateKategori(BinTree &T, string nama, string kategoriBaru) {
    /* Logika: Validasi 4 pilihan kategori paket resmi terlebih dahulu, lalu cari node dan ubah nilainya */
    if (!isValidKategori(kategoriBaru)) {
        cout << "[PERINGATAN] Kategori paket \"" << kategoriBaru << "\" tidak valid!\n";
        cout << "Pilihan yang valid: \"Solo Camp\", \"Family Camp\", \"Hiking/Pendakian\", \"Ekspedisi\".\n";
        return;
    }

    adrPenyewa P = findNodePenyewa(T, nama);
    if (P != Nil) {
        P->info.kategoriPaket = kategoriBaru;
        cout << "[BERHASIL] Kategori paket penyewa \"" << P->info.namaPenyewa 
             << "\" berhasil diperbarui menjadi \"" << kategoriBaru << "\".\n";
    } else {
        cout << "[GAGAL] Data penyewa dengan nama \"" << nama << "\" tidak ditemukan di sistem!\n";
    }
}


/* ========================================================================= */
/*                3. TRAVERSAL BST & DISPLAY (CHALLENGE 3)                   */
/* ========================================================================= */

/* I.S. P terdefinisi (tidak Nil)
   F.S. Menampilkan informasi detail penyewa secara rapi dan seragam berbingkai garis pemisah,
        meliputi Nama, Kategori Paket, Durasi Sewa, Rincian Daftar Alat (MLL), dan Estimasi Total Biaya */
void printDetailPenyewa(adrPenyewa P) {
    /* Logika: Cetak data node BST dan lakukan transversal iteratif pada child list (MLL) */
    if (P == Nil) return;

    cout << "------------------------------------------------------------------------\n";
    cout << " Nama Penyewa    : " << P->info.namaPenyewa << "\n";
    cout << " Kategori Paket  : " << P->info.kategoriPaket << "\n";
    cout << " Durasi Sewa     : " << P->info.durasiSewa << " Hari\n";
    cout << " Daftar Perlengkapan (MLL):\n";

    if (isEmptyListAlat(P->daftarAlat)) {
        cout << "   (Belum ada perlengkapan yang disewa)\n";
    } else {
        adrAlat alat = P->daftarAlat.first;
        int no = 1;
        while (alat != Nil) {
            cout << "   " << no << ". " << left << setw(25) << alat->info.namaAlat 
                 << " | Jumlah: " << setw(3) << alat->info.jumlah << " unit"
                 << " | Harga: " << formatRupiah(alat->info.hargaPerHari) << "/hari\n";
            no++;
            alat = alat->next;
        }
    }

    cout << " Total Kuantitas : " << hitungJumlahAlat(P->daftarAlat) << " Unit Perlengkapan (" 
         << hitungJumlahJenisAlat(P->daftarAlat) << " Jenis)\n";
    cout << " Estimasi Total  : " << formatRupiah(hitungTotalBiaya(P)) << "\n";
    cout << "------------------------------------------------------------------------\n";
}

/* I.S. P terdefinisi (tidak Nil)
   F.S. Menampilkan informasi penyewa (memanggil printDetailPenyewa) */
void printPenyewa(adrPenyewa P) {
    printDetailPenyewa(P);
}

/* I.S. L terdefinisi (mungkin kosong)
   F.S. Menampilkan seluruh data alat yang ada di ListAlat L ke layar */
void printListAlat(ListAlat L) {
    if (isEmptyListAlat(L)) {
        cout << "  [Daftar alat kosong]\n";
        return;
    }
    adrAlat p = L.first;
    int no = 1;
    while (p != Nil) {
        cout << "  " << no++ << ". " << p->info.namaAlat 
             << " - " << p->info.jumlah << " unit (@ " << formatRupiah(p->info.hargaPerHari) << "/hari)\n";
        p = p->next;
    }
}

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data seluruh penyewa secara In-Order (Left - Root - Right), terurut alfabetis A-Z */
void printInOrder(BinTree T) {
    /* Logika: Rekursi traversal In-Order: Kunjungi subtree kiri, cetak node saat ini, kunjungi subtree kanan */
    if (T != Nil) {
        printInOrder(T->left);
        printDetailPenyewa(T);
        printInOrder(T->right);
    }
}

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data seluruh penyewa secara Pre-Order (Root - Left - Right) */
void printPreOrder(BinTree T) {
    /* Logika: Rekursi traversal Pre-Order: Cetak node saat ini, kunjungi subtree kiri, kunjungi subtree kanan */
    if (T != Nil) {
        printDetailPenyewa(T);
        printPreOrder(T->left);
        printPreOrder(T->right);
    }
}

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data seluruh penyewa secara Post-Order (Left - Right - Root) */
void printPostOrder(BinTree T) {
    /* Logika: Rekursi traversal Post-Order: Kunjungi subtree kiri, kunjungi subtree kanan, cetak node saat ini */
    if (T != Nil) {
        printPostOrder(T->left);
        printPostOrder(T->right);
        printDetailPenyewa(T);
    }
}

/* I.S. T terdefinisi (mungkin kosong)
   F.S. Menampilkan data penyewa level demi level dari atas ke bawah, kiri ke kanan (BFS)
        menggunakan antrean bantu berbasis pointer manual QueueBST (Strictly No-STL) */
void printLevelOrder(BinTree T) {
    /* Logika: 
       - Gunakan QueueBST bantu berbasis pointer murni (FIFO).
       - Enqueue root T.
       - Selama antrean tidak kosong: dequeue node terdepan, cetak infonya, lalu enqueue anak kiri dan anak kanannya jika ada. */
    if (isEmptyTree(T)) {
        cout << "[INFO] Tree kosong, tidak ada data penyewa untuk ditampilkan.\n";
        return;
    }

    QueueBST Q;
    createQueueBST(Q);
    enqueueBST(Q, T);

    while (!isEmptyQueueBST(Q)) {
        adrPenyewa curr = Nil;
        dequeueBST(Q, curr);

        printDetailPenyewa(curr);

        if (curr->left != Nil) {
            enqueueBST(Q, curr->left);
        }
        if (curr->right != Nil) {
            enqueueBST(Q, curr->right);
        }
    }
}


/* ========================================================================= */
/*                     4. SEARCH / PENCARIAN (CHALLENGE 4)                   */
/* ========================================================================= */

/* Mengirimkan address node penyewa dengan info.namaPenyewa == nama (case-insensitive).
   Mengembalikan Nil jika data penyewa tidak ditemukan */
adrPenyewa findNodePenyewa(BinTree T, string nama) {
    /* Logika:
       - Basis: T == Nil (tidak ditemukan) -> return Nil.
       - Cocok: toLowerStr(nama) == toLowerStr(T->info.namaPenyewa) -> return T.
       - Lebih kecil: rekursif cari di subtree kiri (T->left).
       - Lebih besar: rekursif cari di subtree kanan (T->right). */
    if (T == Nil) {
        return Nil;
    }

    string target = toLowerStr(nama);
    string current = toLowerStr(T->info.namaPenyewa);

    if (target == current) {
        return T;
    } else if (target < current) {
        return findNodePenyewa(T->left, nama);
    } else {
        return findNodePenyewa(T->right, nama);
    }
}

/* Helper rekursif untuk mencari penyewa yang menyewa alat tertentu */
static void helperCariAlat(BinTree T, string namaAlat, int &foundCount) {
    if (T != Nil) {
        helperCariAlat(T->left, namaAlat, foundCount);

        /* Periksa apakah alat ada di child list milik node T */
        adrAlat p = T->daftarAlat.first;
        bool hasAlat = false;
        int jumlahPinjam = 0;
        while (p != Nil) {
            if (toLowerStr(p->info.namaAlat) == toLowerStr(namaAlat)) {
                hasAlat = true;
                jumlahPinjam += p->info.jumlah;
            }
            p = p->next;
        }

        if (hasAlat) {
            foundCount++;
            cout << " [" << foundCount << "] Penyewa: " << left << setw(20) << T->info.namaPenyewa 
                 << " | Paket: " << setw(18) << T->info.kategoriPaket 
                 << " | Jumlah Disewa: " << jumlahPinjam << " unit\n";
        }

        helperCariAlat(T->right, namaAlat, foundCount);
    }
}

/* I.S. T dan namaAlat terdefinisi
   F.S. Menampilkan daftar nama penyewa dan jumlah unit yang menyewa perlengkapan namaAlat.
        Jika tidak ada penyewa yang meminjam alat tersebut, tampilkan pesan informasi */
void cariByAlat(BinTree T, string namaAlat) {
    /* Logika: Melakukan traversal In-Order untuk mengecek seluruh node BST dan child MLL masing-masing */
    cout << "\n=== HASIL PENCARIAN PENYEWA DENGAN ALAT: \"" << namaAlat << "\" ===\n";
    int count = 0;
    helperCariAlat(T, namaAlat, count);
    if (count == 0) {
        cout << "[INFO] Tidak ditemukan penyewa yang menyewa perlengkapan \"" << namaAlat << "\".\n";
    } else {
        cout << "Total ditemukan: " << count << " penyewa.\n";
    }
}

/* Helper rekursif untuk mencari penyewa dengan durasi sewa tertentu */
static void helperCariDurasi(BinTree T, int durasi, int &foundCount) {
    if (T != Nil) {
        helperCariDurasi(T->left, durasi, foundCount);
        if (T->info.durasiSewa == durasi) {
            foundCount++;
            printDetailPenyewa(T);
        }
        helperCariDurasi(T->right, durasi, foundCount);
    }
}

/* I.S. T dan durasi terdefinisi
   F.S. Menampilkan seluruh penyewa yang memiliki durasiSewa sama dengan durasi */
void cariByDurasi(BinTree T, int durasi) {
    /* Logika: Traversal In-Order seluruh BST dan cetak detail setiap node yang durasiSewa == durasi */
    cout << "\n=== HASIL PENCARIAN PENYEWA DENGAN DURASI: " << durasi << " HARI ===\n";
    int count = 0;
    helperCariDurasi(T, durasi, count);
    if (count == 0) {
        cout << "[INFO] Tidak ditemukan penyewa dengan durasi sewa " << durasi << " hari.\n";
    } else {
        cout << "Total penyewa dengan durasi " << durasi << " hari: " << count << " orang.\n";
    }
}


/* ========================================================================= */
/*                    5. DELETE / PENGHAPUSAN (CHALLENGE 5, 6, 7)            */
/* ========================================================================= */

/* Mengirimkan address node dengan nilai nama terkecil (paling kiri) pada subtree dengan akar root.
   Digunakan sebagai successor saat menghapus node dengan 2 anak pada BST */
adrPenyewa mostLeft(adrPenyewa root) {
    /* Logika: Telusuri pointer left sejauh mungkin hingga curr->left bernilai Nil */
    if (root == Nil) return Nil;
    adrPenyewa curr = root;
    while (curr->left != Nil) {
        curr = curr->left;
    }
    return curr;
}

/* I.S. T terdefinisi, nama terdefinisi
   F.S. Jika node dengan namaPenyewa == nama ditemukan, node tersebut dihapus dari BST T
        (menangani kasus: node daun, node 1 anak, atau node 2 anak menggunakan mostLeft successor).
        Memori node beserta daftar alatnya didealokasikan */
void deleteNodePenyewa(BinTree &T, string nama) {
    /* Logika:
       1. Jika T == Nil: node tidak ditemukan.
       2. Jika nama < T->nama: rekursif ke subtree kiri.
       3. Jika nama > T->nama: rekursif ke subtree kanan.
       4. Jika cocok (node ditemukan):
          - Kasus 1: Daun (left == Nil && right == Nil) -> dealokasiPenyewa(T), T = Nil.
          - Kasus 2a: Hanya punya anak kanan (left == Nil) -> temp = T; T = T->right; temp->right = Nil; dealokasiPenyewa(temp).
          - Kasus 2b: Hanya punya anak kiri (right == Nil) -> temp = T; T = T->left; temp->left = Nil; dealokasiPenyewa(temp).
          - Kasus 3: Memiliki 2 anak:
            * Cari successor = mostLeft(T->right).
            * Hapus daftarAlat lama milik T (delAllAlat(T->daftarAlat)).
            * Salin data info successor ke T.
            * Pindahkan daftarAlat milik successor ke T (shallow copy pointer), lalu set successor->daftarAlat.first = Nil.
            * Rekursif hapus successor dari subtree kanan: deleteNodePenyewa(T->right, succ->info.namaPenyewa). */
    if (T == Nil) {
        cout << "[GAGAL] Data penyewa dengan nama \"" << nama << "\" tidak ditemukan dalam pohon!\n";
        return;
    }

    string target = toLowerStr(nama);
    string current = toLowerStr(T->info.namaPenyewa);

    if (target < current) {
        deleteNodePenyewa(T->left, nama);
    } else if (target > current) {
        deleteNodePenyewa(T->right, nama);
    } else {
        /* Node ditemukan */
        if (T->left == Nil && T->right == Nil) {
            /* Kasus 1: Node Daun */
            dealokasiPenyewa(T);
            T = Nil;
            cout << "[BERHASIL] Node penyewa \"" << nama << "\" (daun) berhasil dihapus.\n";
        } else if (T->left == Nil) {
            /* Kasus 2a: 1 Anak (kanan) */
            adrPenyewa temp = T;
            T = T->right;
            temp->right = Nil;
            dealokasiPenyewa(temp);
            cout << "[BERHASIL] Node penyewa \"" << nama << "\" (1 anak kanan) berhasil dihapus.\n";
        } else if (T->right == Nil) {
            /* Kasus 2b: 1 Anak (kiri) */
            adrPenyewa temp = T;
            T = T->left;
            temp->left = Nil;
            dealokasiPenyewa(temp);
            cout << "[BERHASIL] Node penyewa \"" << nama << "\" (1 anak kiri) berhasil dihapus.\n";
        } else {
            /* Kasus 3: 2 Anak (menggunakan mostLeft dari subtree kanan) */
            adrPenyewa succ = mostLeft(T->right);

            /* Hapus child list T lama agar memori terbebas */
            delAllAlat(T->daftarAlat);

            /* Salin data info dari successor */
            T->info = succ->info;

            /* Pindahkan child list successor ke T */
            T->daftarAlat = succ->daftarAlat;
            succ->daftarAlat.first = Nil; // Lepas kepemilikan alat dari successor

            /* Hapus node successor secara rekursif dari subtree kanan */
            deleteNodePenyewa(T->right, succ->info.namaPenyewa);
            cout << "[BERHASIL] Node penyewa \"" << nama << "\" (2 anak via successor) berhasil dihapus.\n";
        }
    }
}

/* I.S. T dan namaPenyewa terdefinisi
   F.S. Jika penyewa ditemukan, durasiSewa miliknya direset menjadi 0.
        Jika tidak ditemukan, tampilkan pesan peringatan */
void hapusDurasiPenyewa(BinTree &T, string namaPenyewa) {
    /* Logika: Cari node penyewa secara spesifik dan atur info.durasiSewa = 0 */
    adrPenyewa P = findNodePenyewa(T, namaPenyewa);
    if (P != Nil) {
        P->info.durasiSewa = 0;
        cout << "[BERHASIL] Durasi sewa atas nama \"" << P->info.namaPenyewa 
             << "\" telah di-reset menjadi 0 hari.\n";
    } else {
        cout << "[GAGAL] Penyewa dengan nama \"" << namaPenyewa << "\" tidak ditemukan.\n";
    }
}

/* Helper rekursif untuk mereset durasi berdasarkan batas durasi tertentu */
static void helperResetDurasiBatas(BinTree T, int batasDurasi, int &countReset) {
    if (T != Nil) {
        helperResetDurasiBatas(T->left, batasDurasi, countReset);
        if (T->info.durasiSewa == batasDurasi) {
            T->info.durasiSewa = 0;
            countReset++;
            cout << " -> Durasi penyewa \"" << T->info.namaPenyewa << "\" direset menjadi 0 hari.\n";
        }
        helperResetDurasiBatas(T->right, batasDurasi, countReset);
    }
}

/* I.S. T dan batasDurasi terdefinisi
   F.S. Seluruh penyewa yang memiliki durasiSewa == batasDurasi direset durasinya menjadi 0 */
void hapusDurasiPenyewa(BinTree &T, int batasDurasi) {
    /* Logika: Traversal In-Order pohon untuk mereset semua penyewa dengan durasiSewa == batasDurasi */
    cout << "\n=== RESET DURASI SEWA UNTUK PENYEWA DENGAN DURASI: " << batasDurasi << " HARI ===\n";
    int count = 0;
    helperResetDurasiBatas(T, batasDurasi, count);
    if (count == 0) {
        cout << "[INFO] Tidak ada penyewa dengan durasi sewa " << batasDurasi << " hari.\n";
    } else {
        cout << "[SELESAI] Total " << count << " penyewa telah direset durasinya menjadi 0 hari.\n";
    }
}

/* I.S. T, namaPenyewa, dan namaAlat terdefinisi
   F.S. Jika penyewa ditemukan, alat dengan namaAlat dihapus dari sub-list daftarAlat milik penyewa tersebut.
        Jika alat tidak ditemukan dalam list penyewa, tampilkan pesan pemberitahuan */
void hapusAlatDariPenyewa(BinTree T, string namaPenyewa, string namaAlat) {
    /* Logika:
       1. Cari node penyewa dengan findNodePenyewa.
       2. Jika ditemukan, lakukan operasi penghapusan node pada singly linked list daftarAlat:
          - Kasus delete first: alat berada di first.
          - Kasus delete after: alat berada di tengah atau di akhir list.
       3. Dealokasikan memori node alat yang dihapus. */
    adrPenyewa P = findNodePenyewa(T, namaPenyewa);
    if (P == Nil) {
        cout << "[GAGAL] Penyewa dengan nama \"" << namaPenyewa << "\" tidak ditemukan!\n";
        return;
    }

    if (isEmptyListAlat(P->daftarAlat)) {
        cout << "[INFO] Daftar alat milik \"" << P->info.namaPenyewa << "\" sudah kosong!\n";
        return;
    }

    string targetAlat = toLowerStr(namaAlat);
    adrAlat curr = P->daftarAlat.first;
    adrAlat prev = Nil;
    bool found = false;

    while (curr != Nil && !found) {
        if (toLowerStr(curr->info.namaAlat) == targetAlat) {
            found = true;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    if (!found) {
        cout << "[INFO] Alat \"" << namaAlat << "\" tidak ditemukan di dalam daftar sewa \"" 
             << P->info.namaPenyewa << "\"!\n";
    } else {
        if (curr == P->daftarAlat.first) {
            /* Kasus Delete First */
            P->daftarAlat.first = curr->next;
        } else {
            /* Kasus Delete Tengah / Akhir */
            prev->next = curr->next;
        }
        dealokasiAlat(curr);
        cout << "[BERHASIL] Perlengkapan \"" << namaAlat << "\" berhasil dihapus dari daftar sewa \"" 
             << P->info.namaPenyewa << "\".\n";
    }
}


/* ========================================================================= */
/*             6. STATISTIKA & ANALISIS (CHALLENGE 10, 11, 12, 13)          */
/* ========================================================================= */

/* Menghitung dan mengembalikan total banyaknya penyewa yang tercatat di dalam BST T */
int hitungTotalPenyewa(BinTree T) {
    /* Logika: Rekursif menghitung banyaknya node: 1 + kiri + kanan. Basis T == Nil -> 0 */
    if (T == Nil) {
        return 0;
    }
    return 1 + hitungTotalPenyewa(T->left) + hitungTotalPenyewa(T->right);
}

/* Menghitung dan mengembalikan total kuantitas unit perlengkapan dalam ListAlat L */
int hitungJumlahAlat(ListAlat L) {
    /* Logika: Iterasi seluruh node di singly linked list dan menjumlahkan info.jumlah */
    int total = 0;
    adrAlat p = L.first;
    while (p != Nil) {
        total += p->info.jumlah;
        p = p->next;
    }
    return total;
}

/* Menghitung dan mengembalikan jumlah jenis perlengkapan (banyaknya node dalam ListAlat L) */
int hitungJumlahJenisAlat(ListAlat L) {
    /* Logika: Menghitung banyaknya simpul (node) pada singly linked list L */
    int count = 0;
    adrAlat p = L.first;
    while (p != Nil) {
        count++;
        p = p->next;
    }
    return count;
}

/* Menghitung total estimasi biaya sewa milik seorang penyewa P:
   Total = sum(jumlah * hargaPerHari) * durasiSewa.
   Mengembalikan 0 jika P == Nil atau list alat kosong */
long long hitungTotalBiaya(adrPenyewa P) {
    /* Logika:
       - Iterasi seluruh elemen di child list daftarAlat.
       - Akumulasi subtotal = sum(jumlah * hargaPerHari).
       - Total biaya = subtotal * durasiSewa. */
    if (P == Nil || isEmptyListAlat(P->daftarAlat) || P->info.durasiSewa <= 0) {
        return 0;
    }

    long long subtotalHarian = 0;
    adrAlat p = P->daftarAlat.first;
    while (p != Nil) {
        subtotalHarian += ((long long)p->info.jumlah * p->info.hargaPerHari);
        p = p->next;
    }

    return subtotalHarian * P->info.durasiSewa;
}

/* Helper rekursif untuk menampilkan penyewa berdasarkan kategori */
static void helperViewKategori(BinTree T, string kategori, int &count) {
    if (T != Nil) {
        helperViewKategori(T->left, kategori, count);
        if (toLowerStr(T->info.kategoriPaket) == toLowerStr(kategori)) {
            count++;
            printDetailPenyewa(T);
        }
        helperViewKategori(T->right, kategori, count);
    }
}

/* I.S. T dan kategori terdefinisi
   F.S. Menampilkan daftar penyewa dalam kategoriPaket tertentu dan mencetak total jumlah penyewanya */
void viewDanHitungByKategori(BinTree T, string kategori) {
    /* Logika: Traversal In-Order BST, filter node yang info.kategoriPaket sesuai, akumulasikan counter */
    cout << "\n=== DAFTAR PENYEWA KATEGORI PAKET: \"" << kategori << "\" ===\n";
    int total = 0;
    helperViewKategori(T, kategori, total);
    cout << "------------------------------------------------------------------------\n";
    cout << " Total Penyewa Kategori \"" << kategori << "\" : " << total << " Orang\n";
    cout << "------------------------------------------------------------------------\n";
}

/* Helper rekursif untuk mengumpulkan data statistik durasi */
static void helperKumpulStatistik(BinTree T, int &maxDurasi, int &minDurasi, 
                                  long long &sumDurasi, int &countValid, 
                                  string &namaMax, string &namaMin) {
    if (T != Nil) {
        helperKumpulStatistik(T->left, maxDurasi, minDurasi, sumDurasi, countValid, namaMax, namaMin);

        /* Abaikan durasi 0 (karena dianggap sewa selesai / di-reset) */
        if (T->info.durasiSewa > 0) {
            countValid++;
            sumDurasi += T->info.durasiSewa;

            if (T->info.durasiSewa > maxDurasi) {
                maxDurasi = T->info.durasiSewa;
                namaMax = T->info.namaPenyewa;
            }
            if (T->info.durasiSewa < minDurasi) {
                minDurasi = T->info.durasiSewa;
                namaMin = T->info.namaPenyewa;
            }
        }

        helperKumpulStatistik(T->right, maxDurasi, minDurasi, sumDurasi, countValid, namaMax, namaMin);
    }
}

/* I.S. T terdefinisi
   F.S. Menampilkan ringkasan statistik durasi sewa dari seluruh penyewa pada BST T:
        durasi terlama (maksimum), durasi tercepat (minimum), dan rata-rata durasi sewa */
void tampilkanStatistikDurasi(BinTree T) {
    /* Logika:
       - Telusuri seluruh node BST.
       - Catat durasi maksimum, durasi minimum (mengabaikan durasi 0), dan akumulasi total durasi.
       - Hitung rata-rata = total durasi / total penyewa aktif. */
    if (isEmptyTree(T)) {
        cout << "[INFO] Tree kosong, belum ada data untuk menghitung statistik durasi.\n";
        return;
    }

    int maxDurasi = -1;
    int minDurasi = 999999;
    long long sumDurasi = 0;
    int countValid = 0;
    string namaMax = "-";
    string namaMin = "-";

    helperKumpulStatistik(T, maxDurasi, minDurasi, sumDurasi, countValid, namaMax, namaMin);

    cout << "\n========================================================================\n";
    cout << "                   RINGKASAN STATISTIK DURASI SEWA                      \n";
    cout << "========================================================================\n";
    if (countValid == 0) {
        cout << " [INFO] Tidak ada penyewa dengan durasi sewa aktif (> 0 hari).\n";
    } else {
        double rataRata = (double)sumDurasi / countValid;
        cout << " Total Penyewa Aktif     : " << countValid << " Orang\n";
        cout << " Durasi Sewa Terlama (Max): " << maxDurasi << " Hari (Atas Nama: " << namaMax << ")\n";
        cout << " Durasi Sewa Tercepat(Min): " << minDurasi << " Hari (Atas Nama: " << namaMin << ")\n";
        cout << fixed << setprecision(2);
        cout << " Rata-rata Durasi Sewa   : " << rataRata << " Hari\n";
    }
    cout << "========================================================================\n";
}

/* Helper rekursif untuk mencari penyewa berdasarkan jumlah jenis alat */
static void helperCariJumlahAlat(BinTree T, int targetJumlah, int &foundCount) {
    if (T != Nil) {
        helperCariJumlahAlat(T->left, targetJumlah, foundCount);
        if (hitungJumlahJenisAlat(T->daftarAlat) == targetJumlah) {
            foundCount++;
            printDetailPenyewa(T);
        }
        helperCariJumlahAlat(T->right, targetJumlah, foundCount);
    }
}

/* I.S. T dan targetJumlah terdefinisi
   F.S. Menghitung jumlah node di child MLL tiap penyewa, lalu menampilkan penyewa
        yang memiliki jumlah jenis perlengkapan tepat sesuai targetJumlah */
void cariByJumlahAlat(BinTree T, int targetJumlah) {
    /* Logika: Traversal BST, hitung node pada sub-list daftarAlat tiap penyewa menggunakan hitungJumlahJenisAlat.
               Tampilkan penyewa yang jumlah jenis perlengkapannya tepat sama dengan targetJumlah */
    cout << "\n=== HASIL PENCARIAN PENYEWA DENGAN " << targetJumlah << " JENIS PERLENGKAPAN ===\n";
    int count = 0;
    helperCariJumlahAlat(T, targetJumlah, count);
    if (count == 0) {
        cout << "[INFO] Tidak ditemukan penyewa yang memiliki tepat " << targetJumlah << " jenis perlengkapan.\n";
    } else {
        cout << "Total ditemukan: " << count << " penyewa.\n";
    }
}


/* ========================================================================= */
/*           7. PRIMITIF QUEUE BANTU LEVEL ORDER BST (NO-STL)                */
/* ========================================================================= */

/* I.S. Q sembarang
   F.S. Terbentuk QueueBST Q yang kosong (Q.head = Nil, Q.tail = Nil) */
void createQueueBST(QueueBST &Q) {
    /* Logika: Set pointer head dan tail antrean menjadi Nil */
    Q.head = Nil;
    Q.tail = Nil;
}

/* Mengirimkan true jika QueueBST Q kosong, false jika tidak */
bool isEmptyQueueBST(QueueBST Q) {
    return (Q.head == Nil);
}

/* I.S. Q terdefinisi, nodeTree terdefinisi
   F.S. Node tree nodeTree disisipkan ke bagian belakang (tail) QueueBST Q */
void enqueueBST(QueueBST &Q, adrPenyewa nodeTree) {
    /* Logika:
       - Alokasikan elemen antrean baru ElmQueueBST.
       - Sambungkan ke tail saat ini (tail->next = baru), lalu geser tail ke node baru.
       - Jika antrean semula kosong, jadikan node baru sebagai head sekaligus tail. */
    adrQueueBST P = new ElmQueueBST;
    P->nodeTree = nodeTree;
    P->next = Nil;

    if (isEmptyQueueBST(Q)) {
        Q.head = P;
        Q.tail = P;
    } else {
        Q.tail->next = P;
        Q.tail = P;
    }
}

/* I.S. Q terdefinisi dan tidak kosong
   F.S. Node tree terdepan (head) diambil dan disimpan pada nodeTree, elemen antrean didealokasi */
void dequeueBST(QueueBST &Q, adrPenyewa &nodeTree) {
    /* Logika:
       - Ambil info nodeTree dari Q.head.
       - Simpan pointer head lama ke temp, geser head ke head->next.
       - Jika head menjadi Nil, set tail = Nil.
       - Dealokasikan memori node antrean temp dengan delete. */
    if (!isEmptyQueueBST(Q)) {
        adrQueueBST temp = Q.head;
        nodeTree = temp->nodeTree;
        Q.head = Q.head->next;
        if (Q.head == Nil) {
            Q.tail = Nil;
        }
        delete temp;
    } else {
        nodeTree = Nil;
    }
}

/* I.S. Q terdefinisi, mungkin kosong
   F.S. Seluruh elemen antrean pada QueueBST didealokasi hingga antrean kosong */
void delAllQueueBST(QueueBST &Q) {
    /* Logika: Dequeue semua elemen yang tersisa di dalam antrean hingga kosong */
    adrPenyewa dummy = Nil;
    while (!isEmptyQueueBST(Q)) {
        dequeueBST(Q, dummy);
    }
}
