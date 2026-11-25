#include "bst_mll.h"
#include "queue_stack.h"
#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

/* ========================================================================= */
/*                         FUNGSI UTILITAS & VALIDASI INPUT                  */
/* ========================================================================= */

/* Memotong spasi (whitespace) di awal dan akhir string */
static string trimString(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/* Prosedur untuk menahan layar terminal hingga pengguna menekan tombol Enter */
void pauseTerminal() {
    cout << "\nTekan Enter untuk melanjutkan...";
    string dummy;
    getline(cin, dummy);
}

/* Membaca string yang divalidasi tidak boleh kosong atau hanya berisi spasi */
string inputString(string prompt, string fieldName = "Input") {
    string str;
    while (true) {
        cout << prompt;
        if (!getline(cin, str)) {
            cin.clear();
            continue;
        }
        string trimmed = trimString(str);
        if (trimmed.empty()) {
            cout << "[ERROR] " << fieldName << " tidak boleh kosong! Harap masukkan data yang valid.\n";
            continue;
        }
        return trimmed;
    }
}

/* Membaca input angka bulat positif (misal durasi, jumlah unit, harga nominal) 
   secara ketat dan mendeteksi jika user salah memasukkan titik/koma/huruf */
int inputPositiveInt(string prompt, string fieldName = "Nilai") {
    string input;
    while (true) {
        cout << prompt;
        if (!getline(cin, input)) {
            cin.clear();
            continue;
        }
        string trimmed = trimString(input);
        if (trimmed.empty()) {
            cout << "[ERROR] " << fieldName << " tidak boleh kosong!\n";
            continue;
        }

        /* 1. Deteksi apakah user memasukkan pemisah titik atau koma (misal: 20.000 atau 20.000.00) */
        bool hasDotOrComma = false;
        bool allDigits = true;
        for (char c : trimmed) {
            if (c == '.' || c == ',') {
                hasDotOrComma = true;
            } else if (!isdigit((unsigned char)c)) {
                allDigits = false;
            }
        }

        if (hasDotOrComma) {
            cout << "[ERROR] Harap masukkan angka murni TANPA tanda titik atau koma!\n";
            cout << "        Contoh: ketik 20000 (bukan 20.000 atau 20.000.00)\n";
            continue;
        }

        if (!allDigits) {
            cout << "[ERROR] " << fieldName << " harus berupa angka bulat positif (tanpa huruf/simbol)!\n";
            continue;
        }

        /* 2. Konversi dan pengecekan batas nilai */
        try {
            long long val = stoll(trimmed);
            if (val <= 0) {
                cout << "[ERROR] " << fieldName << " harus bernilai positif (> 0)!\n";
                continue;
            }
            if (val > 2000000000LL) {
                cout << "[ERROR] " << fieldName << " terlalu besar melebihi kapasitas sistem!\n";
                continue;
            }
            return (int)val;
        } catch (...) {
            cout << "[ERROR] Format angka tidak valid!\n";
        }
    }
}

/* Membaca pilihan nomor menu dan memastikan angka berada dalam rentang [minVal, maxVal] */
int inputMenuChoice(string prompt, int minVal, int maxVal) {
    string input;
    while (true) {
        cout << prompt;
        if (!getline(cin, input)) {
            cin.clear();
            continue;
        }
        string trimmed = trimString(input);
        if (trimmed.empty()) {
            cout << "[ERROR] Pilihan menu tidak boleh kosong!\n";
            continue;
        }

        bool allDigits = true;
        for (char c : trimmed) {
            if (!isdigit((unsigned char)c)) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << "[ERROR] Pilihan menu harus berupa angka bulat antara " 
                 << minVal << " sampai " << maxVal << "!\n";
            continue;
        }

        try {
            long long val = stoll(trimmed);
            if (val < minVal || val > maxVal) {
                cout << "[ERROR] Pilihan di luar rentang! Harap pilih angka [" 
                     << minVal << " - " << maxVal << "].\n";
                continue;
            }
            return (int)val;
        } catch (...) {
            cout << "[ERROR] Pilihan tidak valid!\n";
        }
    }
}

/* Membuat ringkasan teks perlengkapan dari child linked list untuk antrean pengembalian */
string buatRingkasanAlat(ListAlat L) {
    if (isEmptyListAlat(L)) {
        return "Tidak ada alat";
    }
    string res = "";
    adrAlat p = L.first;
    while (p != Nil) {
        res += to_string(p->info.jumlah) + "x " + p->info.namaAlat;
        if (p->next != Nil) {
            res += ", ";
        }
        p = p->next;
    }
    return res;
}

/* Memuat data awal (dummy/sample data) untuk kemudahan pengujian praktikum */
void loadDummyData(BinTree &T, QueueInspeksi &Q, StackRiwayat &S) {
    /* 1. Dummy Penyewa 1: Budi Santoso (Family Camp) */
    infotypePenyewa p1 = {"Budi Santoso", 3, "Family Camp"};
    adrPenyewa n1 = alokasiPenyewa(p1);
    insertLastAlat(n1->daftarAlat, alokasiAlat({"Tenda Dome Kapasitas 4", 2, 45000}));
    insertLastAlat(n1->daftarAlat, alokasiAlat({"Matras Alumunium", 4, 10000}));
    insertLastAlat(n1->daftarAlat, alokasiAlat({"Kompor Portable & Nesting", 1, 25000}));
    insertNodePenyewa(T, n1);

    /* 2. Dummy Penyewa 2: Andi Wijaya (Solo Camp) */
    infotypePenyewa p2 = {"Andi Wijaya", 2, "Solo Camp"};
    adrPenyewa n2 = alokasiPenyewa(p2);
    insertLastAlat(n2->daftarAlat, alokasiAlat({"Tenda Single Person", 1, 30000}));
    insertLastAlat(n2->daftarAlat, alokasiAlat({"Sleeping Bag Polar", 1, 15000}));
    insertLastAlat(n2->daftarAlat, alokasiAlat({"Headlamp LED", 1, 10000}));
    insertNodePenyewa(T, n2);

    /* 3. Dummy Penyewa 3: Citra Lestari (Hiking/Pendakian) */
    infotypePenyewa p3 = {"Citra Lestari", 4, "Hiking/Pendakian"};
    adrPenyewa n3 = alokasiPenyewa(p3);
    insertLastAlat(n3->daftarAlat, alokasiAlat({"Carrier 60L Consina", 1, 35000}));
    insertLastAlat(n3->daftarAlat, alokasiAlat({"Trekking Pole", 2, 15000}));
    insertLastAlat(n3->daftarAlat, alokasiAlat({"Sleeping Bag Polar", 1, 15000}));
    insertNodePenyewa(T, n3);

    /* 4. Dummy Penyewa 4: Dimas Pratama (Ekspedisi) */
    infotypePenyewa p4 = {"Dimas Pratama", 5, "Ekspedisi"};
    adrPenyewa n4 = alokasiPenyewa(p4);
    insertLastAlat(n4->daftarAlat, alokasiAlat({"Tenda Dome Kapasitas 4", 1, 45000}));
    insertLastAlat(n4->daftarAlat, alokasiAlat({"Carrier 60L Consina", 2, 35000}));
    insertLastAlat(n4->daftarAlat, alokasiAlat({"Flysheet 3x4 Meter", 1, 20000}));
    insertLastAlat(n4->daftarAlat, alokasiAlat({"Kompor Portable & Nesting", 1, 25000}));
    insertNodePenyewa(T, n4);

    /* 5. Dummy Penyewa 5: Eka Rahmawati (Solo Camp) */
    infotypePenyewa p5 = {"Eka Rahmawati", 1, "Solo Camp"};
    adrPenyewa n5 = alokasiPenyewa(p5);
    insertLastAlat(n5->daftarAlat, alokasiAlat({"Hammock & Webbing", 1, 15000}));
    insertLastAlat(n5->daftarAlat, alokasiAlat({"Lampu Tenda Gantung", 1, 10000}));
    insertNodePenyewa(T, n5);

    /* Dummy Data Queue Inspeksi (Alat yang baru saja dikembalikan dan menunggu pembersihan) */
    infotypeQueue q1 = {"Gilang Ramadhan", "1x Carrier 60L, 2x Matras Alumunium", "Menunggu Cek"};
    enQueue(Q, alokasiQueue(q1));

    /* Dummy Data Stack Riwayat (Transaksi yang telah selesai sebelumnya) */
    infotypeStack s1 = {"Hendra Kusuma", "Solo Camp", 2, 90000};
    push(S, alokasiStack(s1));
}


/* ========================================================================= */
/*                         SUB-MENU HANDLER FUNCTIONS                        */
/* ========================================================================= */

/* Menu 1: Tambah / Update Data Penyewaan */
void menuTambahUpdate(BinTree &T) {
    int subPilih = -1;
    while (subPilih != 0) {
        cout << "\n=======================================================\n";
        cout << "       1. KELOLA DATA PENYEWAAN (TAMBAH & UPDATE)      \n";
        cout << "=======================================================\n";
        cout << " 1. Tambah Penyewa Baru ke BST\n";
        cout << " 2. Tambah Perlengkapan Outdoor ke Penyewa (MLL)\n";
        cout << " 3. Update Durasi Sewa Penyewa\n";
        cout << " 4. Update Kategori Paket Penyewa\n";
        cout << " 0. Kembali ke Menu Utama\n";
        cout << "=======================================================\n";
        subPilih = inputMenuChoice("Pilih sub-menu [0-4]: ", 0, 4);

        if (subPilih == 1) {
            cout << "\n--- Tambah Penyewa Baru ---\n";
            string nama = inputString("Masukkan Nama Penyewa (Key BST unik): ", "Nama penyewa");

            /* Cek apakah nama sudah terdaftar */
            if (findNodePenyewa(T, nama) != Nil) {
                cout << "[PERINGATAN] Penyewa dengan nama \"" << nama << "\" sudah ada dalam sistem!\n";
                pauseTerminal();
                continue;
            }

            cout << "Pilihan Kategori Paket Valid:\n";
            cout << " 1. Solo Camp\n 2. Family Camp\n 3. Hiking/Pendakian\n 4. Ekspedisi\n";
            int pilKategori = inputMenuChoice("Pilih nomor paket [1-4]: ", 1, 4);
            string kategori = "Solo Camp";
            if (pilKategori == 2) kategori = "Family Camp";
            else if (pilKategori == 3) kategori = "Hiking/Pendakian";
            else if (pilKategori == 4) kategori = "Ekspedisi";

            int durasi = inputPositiveInt("Masukkan Durasi Sewa (hari): ", "Durasi sewa");

            infotypePenyewa info;
            info.namaPenyewa = nama;
            info.durasiSewa = durasi;
            info.kategoriPaket = kategori;

            adrPenyewa P = alokasiPenyewa(info);
            insertNodePenyewa(T, P);
            cout << "[BERHASIL] Penyewa \"" << nama << "\" berhasil didaftarkan ke sistem.\n";
            pauseTerminal();

        } else if (subPilih == 2) {
            cout << "\n--- Tambah Perlengkapan ke Penyewa ---\n";
            string nama = inputString("Masukkan Nama Penyewa target: ", "Nama penyewa");
            adrPenyewa P = findNodePenyewa(T, nama);
            if (P == Nil) {
                cout << "[ERROR] Penyewa dengan nama \"" << nama << "\" tidak ditemukan!\n";
            } else {
                cout << "[INFO] Penyewa ditemukan: " << P->info.namaPenyewa 
                     << " (" << P->info.kategoriPaket << ")\n";
                string namaAlat = inputString("Masukkan Nama Perlengkapan: ", "Nama perlengkapan");
                int jumlah = inputPositiveInt("Masukkan Jumlah Unit: ", "Jumlah unit");
                int harga = inputPositiveInt("Masukkan Harga Sewa per Hari (Rp): ", "Harga sewa");

                infotypeAlat infoAlat = {namaAlat, jumlah, harga};
                adrAlat nodeAlat = alokasiAlat(infoAlat);
                insertLastAlat(P->daftarAlat, nodeAlat);
                cout << "[BERHASIL] Perlengkapan berhasil didaftarkan ke daftar sewa \"" 
                     << P->info.namaPenyewa << "\".\n";
            }
            pauseTerminal();

        } else if (subPilih == 3) {
            cout << "\n--- Update Durasi Sewa Penyewa ---\n";
            string nama = inputString("Masukkan Nama Penyewa: ", "Nama penyewa");
            int durasiBaru = inputPositiveInt("Masukkan Durasi Sewa Baru (hari): ", "Durasi sewa");
            updateDurasi(T, nama, durasiBaru);
            pauseTerminal();

        } else if (subPilih == 4) {
            cout << "\n--- Update Kategori Paket Penyewa ---\n";
            string nama = inputString("Masukkan Nama Penyewa: ", "Nama penyewa");
            cout << "Pilihan Kategori Paket Valid:\n";
            cout << " 1. Solo Camp\n 2. Family Camp\n 3. Hiking/Pendakian\n 4. Ekspedisi\n";
            int pilKategori = inputMenuChoice("Pilih nomor paket baru [1-4]: ", 1, 4);
            string kategoriBaru = "Solo Camp";
            if (pilKategori == 2) kategoriBaru = "Family Camp";
            else if (pilKategori == 3) kategoriBaru = "Hiking/Pendakian";
            else if (pilKategori == 4) kategoriBaru = "Ekspedisi";

            updateKategori(T, nama, kategoriBaru);
            pauseTerminal();
        }
    }
}

/* Menu 2: Tampilkan Data Penyewaan (4 Traversal BST) */
void menuTraversal(BinTree T) {
    int subPilih = -1;
    while (subPilih != 0) {
        cout << "\n=======================================================\n";
        cout << "       2. TAMPILKAN DATA PENYEWAAN (TRAVERSAL BST)     \n";
        cout << "=======================================================\n";
        cout << " 1. Traversal In-Order   (Terurut Alfabetis A-Z)\n";
        cout << " 2. Traversal Pre-Order  (Root - Left - Right)\n";
        cout << " 3. Traversal Post-Order (Left - Right - Root)\n";
        cout << " 4. Traversal Level-Order (Breadth First Search - BFS)\n";
        cout << " 0. Kembali ke Menu Utama\n";
        cout << "=======================================================\n";
        subPilih = inputMenuChoice("Pilih sub-menu [0-4]: ", 0, 4);

        if (isEmptyTree(T)) {
            cout << "\n[INFO] Tree penyewaan masih kosong! Belum ada data penyewa.\n";
            pauseTerminal();
            continue;
        }

        if (subPilih == 1) {
            cout << "\n=== HASIL TRAVERSAL IN-ORDER (ALFABETIS A-Z) ===\n";
            printInOrder(T);
            pauseTerminal();
        } else if (subPilih == 2) {
            cout << "\n=== HASIL TRAVERSAL PRE-ORDER ===\n";
            printPreOrder(T);
            pauseTerminal();
        } else if (subPilih == 3) {
            cout << "\n=== HASIL TRAVERSAL POST-ORDER ===\n";
            printPostOrder(T);
            pauseTerminal();
        } else if (subPilih == 4) {
            cout << "\n=== HASIL TRAVERSAL LEVEL-ORDER (BFS - NO STL) ===\n";
            printLevelOrder(T);
            pauseTerminal();
        }
    }
}

/* Menu 3: Pencarian Data Penyewaan */
void menuPencarian(BinTree T) {
    int subPilih = -1;
    while (subPilih != 0) {
        cout << "\n=======================================================\n";
        cout << "               3. PENCARIAN DATA PENYEWAAN             \n";
        cout << "=======================================================\n";
        cout << " 1. Cari Penyewa Berdasarkan Nama (Key BST)\n";
        cout << " 2. Cari Penyewa Berdasarkan Nama Alat Tertentu\n";
        cout << " 3. Cari Penyewa Berdasarkan Durasi Sewa\n";
        cout << " 4. Cari Penyewa Berdasarkan Jumlah Jenis Perlengkapan\n";
        cout << " 0. Kembali ke Menu Utama\n";
        cout << "=======================================================\n";
        subPilih = inputMenuChoice("Pilih sub-menu [0-4]: ", 0, 4);

        if (subPilih == 1) {
            string nama = inputString("Masukkan Nama Penyewa yang dicari: ", "Nama penyewa");
            adrPenyewa P = findNodePenyewa(T, nama);
            if (P != Nil) {
                cout << "\n[DITEMUKAN] Data penyewa berhasil ditemukan:\n";
                printDetailPenyewa(P);
            } else {
                cout << "\n[GAGAL] Penyewa dengan nama \"" << nama << "\" tidak ditemukan di sistem!\n";
            }
            pauseTerminal();

        } else if (subPilih == 2) {
            string namaAlat = inputString("Masukkan Nama Perlengkapan yang dicari: ", "Nama perlengkapan");
            cariByAlat(T, namaAlat);
            pauseTerminal();

        } else if (subPilih == 3) {
            int durasi = inputPositiveInt("Masukkan Durasi Sewa (hari) yang dicari: ", "Durasi sewa");
            cariByDurasi(T, durasi);
            pauseTerminal();

        } else if (subPilih == 4) {
            int targetJenis = inputPositiveInt("Masukkan Target Jumlah Jenis Alat: ", "Target jumlah jenis alat");
            cariByJumlahAlat(T, targetJenis);
            pauseTerminal();
        }
    }
}

/* Menu 4: Hapus Data */
void menuHapus(BinTree &T) {
    int subPilih = -1;
    while (subPilih != 0) {
        cout << "\n=======================================================\n";
        cout << "           4. HAPUS DATA (PENYEWA / DURASI / ALAT)     \n";
        cout << "=======================================================\n";
        cout << " 1. Hapus Node Penyewa dari BST\n";
        cout << " 2. Reset Durasi Sewa Penyewa Tertentu (Menjadi 0)\n";
        cout << " 3. Reset Durasi Sewa Berdasarkan Batas Durasi\n";
        cout << " 4. Hapus Satu Alat dari Daftar Sewa Penyewa (MLL)\n";
        cout << " 0. Kembali ke Menu Utama\n";
        cout << "=======================================================\n";
        subPilih = inputMenuChoice("Pilih sub-menu [0-4]: ", 0, 4);

        if (subPilih == 1) {
            string nama = inputString("Masukkan Nama Penyewa yang akan dihapus: ", "Nama penyewa");
            deleteNodePenyewa(T, nama);
            pauseTerminal();

        } else if (subPilih == 2) {
            string nama = inputString("Masukkan Nama Penyewa yang durasinya akan direset: ", "Nama penyewa");
            hapusDurasiPenyewa(T, nama);
            pauseTerminal();

        } else if (subPilih == 3) {
            int batas = inputPositiveInt("Masukkan Batas Durasi yang akan direset ke 0 hari: ", "Batas durasi");
            hapusDurasiPenyewa(T, batas);
            pauseTerminal();

        } else if (subPilih == 4) {
            string nama = inputString("Masukkan Nama Penyewa: ", "Nama penyewa");
            string alat = inputString("Masukkan Nama Alat yang ingin dihapus dari penyewa: ", "Nama alat");
            hapusAlatDariPenyewa(T, nama, alat);
            pauseTerminal();
        }
    }
}

/* Menu 5: Proses Pengembalian Barang (Integrasi BST -> Queue Inspeksi & Stack Riwayat) */
void menuPengembalian(BinTree &T, QueueInspeksi &Q, StackRiwayat &S) {
    cout << "\n========================================================================\n";
    cout << "     PROSES PENGEMBALIAN BARANG (INTEGRASI BST -> QUEUE & STACK)        \n";
    cout << "========================================================================\n";
    cout << "Alur Kerja Sistem:\n";
    cout << " 1. Data penyewa dicari di sistem aktif (BST).\n";
    cout << " 2. Rincian alat dimasukkan ke Queue Inspeksi & Pencucian (FIFO).\n";
    cout << " 3. Riwayat transaksi dihitung dan dicatat ke Stack Riwayat (LIFO).\n";
    cout << " 4. Data penyewa aktif dikeluarkan dari BST.\n";
    cout << "------------------------------------------------------------------------\n";

    string nama = inputString("Masukkan Nama Penyewa yang mengembalikan barang: ", "Nama penyewa");
    adrPenyewa P = findNodePenyewa(T, nama);

    if (P == Nil) {
        cout << "[GAGAL] Data penyewa dengan nama \"" << nama << "\" tidak ditemukan di daftar sewa aktif!\n";
        pauseTerminal();
        return;
    }

    cout << "\n--- Detail Penyewaan Ditemukan ---\n";
    printDetailPenyewa(P);

    long long totalBiaya = hitungTotalBiaya(P);
    string ringkasan = buatRingkasanAlat(P->daftarAlat);

    string konfirmasi = inputString("Konfirmasi proses pengembalian perlengkapan? (y/n): ", "Konfirmasi");
    if (konfirmasi != "y" && konfirmasi != "Y") {
        cout << "[DIBATALKAN] Proses pengembalian dibatalkan oleh pengguna.\n";
        pauseTerminal();
        return;
    }

    /* 1. Masukkan ke Queue Inspeksi (FIFO) */
    infotypeQueue infoQ;
    infoQ.namaPenyewa = P->info.namaPenyewa;
    infoQ.ringkasanAlat = ringkasan;
    infoQ.statusCek = "Menunggu Cek & Cuci";
    enQueue(Q, alokasiQueue(infoQ));

    /* 2. Masukkan ke Stack Riwayat Transaksi (LIFO) */
    infotypeStack infoS;
    infoS.namaPenyewa = P->info.namaPenyewa;
    infoS.kategoriPaket = P->info.kategoriPaket;
    infoS.durasiSewa = P->info.durasiSewa;
    infoS.totalBiaya = totalBiaya;
    push(S, alokasiStack(infoS));

    /* 3. Hapus penyewa dari BST */
    string namaHapus = P->info.namaPenyewa;
    deleteNodePenyewa(T, namaHapus);

    cout << "\n========================================================================\n";
    cout << "         PENGEMBALIAN & PENYELESAIAN TRANSAKSI BERHASIL DIPROSES        \n";
    cout << "========================================================================\n";
    cout << " [1] Perlengkapan terdaftar di Queue Inspeksi untuk dibersihkan teknisi.\n";
    cout << " [2] Transaksi sebesar Rp " << totalBiaya << " telah diarsipkan ke Stack Riwayat.\n";
    cout << " [3] Node penyewa aktif telah dihapus dari BST.\n";
    cout << "========================================================================\n";
    pauseTerminal();
}

/* Menu 6: Kelola Antrean Inspeksi (Queue FIFO) */
void menuQueue(QueueInspeksi &Q) {
    int subPilih = -1;
    while (subPilih != 0) {
        cout << "\n=======================================================\n";
        cout << "       6. KELOLA ANTREAN INSPEKSI & PENCUCIAN (FIFO)   \n";
        cout << "=======================================================\n";
        cout << " 1. Lihat Seluruh Antrean Inspeksi & Pencucian\n";
        cout << " 2. Proses Perlengkapan Terdepan (DeQueue)\n";
        cout << " 0. Kembali ke Menu Utama\n";
        cout << "=======================================================\n";
        subPilih = inputMenuChoice("Pilih sub-menu [0-2]: ", 0, 2);

        if (subPilih == 1) {
            printQueue(Q);
            pauseTerminal();
        } else if (subPilih == 2) {
            if (isEmptyQueue(Q)) {
                cout << "\n[INFO] Antrean inspeksi kosong! Tidak ada alat yang perlu diproses.\n";
            } else {
                adrQueue P = Nil;
                deQueue(Q, P);
                if (P != Nil) {
                    dealokasiQueue(P);
                }
            }
            pauseTerminal();
        }
    }
}

/* Menu 7: Riwayat Transaksi Selesai (Stack LIFO) */
void menuStack(StackRiwayat &S) {
    int subPilih = -1;
    while (subPilih != 0) {
        cout << "\n=======================================================\n";
        cout << "         7. RIWAYAT TRANSAKSI SELESAI (STACK LIFO)     \n";
        cout << "=======================================================\n";
        cout << " 1. Tampilkan Seluruh Riwayat & Total Pendapatan\n";
        cout << " 2. Lihat Transaksi Paling Terakhir Selesai (Peek Top)\n";
        cout << " 3. Batalkan / Keluarkan Transaksi Teratas (Pop)\n";
        cout << " 0. Kembali ke Menu Utama\n";
        cout << "=======================================================\n";
        subPilih = inputMenuChoice("Pilih sub-menu [0-3]: ", 0, 3);

        if (subPilih == 1) {
            printStack(S);
            pauseTerminal();
        } else if (subPilih == 2) {
            peekTop(S);
            pauseTerminal();
        } else if (subPilih == 3) {
            if (isEmptyStack(S)) {
                cout << "\n[INFO] Stack riwayat masih kosong!\n";
            } else {
                adrStack P = Nil;
                pop(S, P);
                if (P != Nil) {
                    cout << "[INFO] Transaksi atas nama \"" << P->info.namaPenyewa << "\" didealokasikan.\n";
                    dealokasiStack(P);
                }
            }
            pauseTerminal();
        }
    }
}

/* Menu 8: Laporan & Statistika */
void menuStatistik(BinTree T) {
    int subPilih = -1;
    while (subPilih != 0) {
        cout << "\n=======================================================\n";
        cout << "                8. LAPORAN & STATISTIKA                \n";
        cout << "=======================================================\n";
        cout << " 1. Total Penyewa Aktif di BST\n";
        cout << " 2. Tampilkan & Hitung Penyewa Berdasarkan Kategori\n";
        cout << " 3. Statistik Durasi Sewa (Terlama, Tercepat, Rata-rata)\n";
        cout << " 0. Kembali ke Menu Utama\n";
        cout << "=======================================================\n";
        subPilih = inputMenuChoice("Pilih sub-menu [0-3]: ", 0, 3);

        if (subPilih == 1) {
            cout << "\n=======================================================\n";
            cout << " Total Penyewa Aktif di Sistem : " << hitungTotalPenyewa(T) << " Orang\n";
            cout << "=======================================================\n";
            pauseTerminal();

        } else if (subPilih == 2) {
            cout << "\nPilihan Kategori Paket:\n";
            cout << " 1. Solo Camp\n 2. Family Camp\n 3. Hiking/Pendakian\n 4. Ekspedisi\n";
            int pilKategori = inputMenuChoice("Pilih kategori paket [1-4]: ", 1, 4);
            string kategori = "Solo Camp";
            if (pilKategori == 2) kategori = "Family Camp";
            else if (pilKategori == 3) kategori = "Hiking/Pendakian";
            else if (pilKategori == 4) kategori = "Ekspedisi";

            viewDanHitungByKategori(T, kategori);
            pauseTerminal();

        } else if (subPilih == 3) {
            tampilkanStatistikDurasi(T);
            pauseTerminal();
        }
    }
}


/* ========================================================================= */
/*                               MAIN FUNCTION                               */
/* ========================================================================= */

int main() {
    /* 1. Inisialisasi ADT Struktur Data */
    BinTree T;
    createTree(T);

    QueueInspeksi Q;
    createQueue(Q);

    StackRiwayat S;
    createStack(S);

    /* 2. Muat data awal pengujian (demo/dummy data) */
    loadDummyData(T, Q, S);

    int pilihan = -1;
    while (pilihan != 0) {
        cout << "\n=======================================================\n";
        cout << "      SISTEM PENYEWAAN PERLENGKAPAN OUTDOOR/CAMPING    \n";
        cout << "=======================================================\n";
        cout << " 1. Tambah / Update Data Penyewaan\n";
        cout << " 2. Tampilkan Data Penyewaan (Traversal BST)\n";
        cout << " 3. Pencarian Data Penyewaan\n";
        cout << " 4. Hapus Data (Penyewa / Durasi / Alat)\n";
        cout << " 5. Proses Pengembalian Barang (Ke Queue & Stack)\n";
        cout << " 6. Kelola Antrean Inspeksi (Queue FIFO)\n";
        cout << " 7. Riwayat Transaksi Selesai (Stack LIFO)\n";
        cout << " 8. Laporan & Statistika\n";
        cout << " 0. Keluar Program\n";
        cout << "=======================================================\n";
        pilihan = inputMenuChoice("Pilih menu [0-8]: ", 0, 8);

        switch (pilihan) {
            case 1:
                menuTambahUpdate(T);
                break;
            case 2:
                menuTraversal(T);
                break;
            case 3:
                menuPencarian(T);
                break;
            case 4:
                menuHapus(T);
                break;
            case 5:
                menuPengembalian(T, Q, S);
                break;
            case 6:
                menuQueue(Q);
                break;
            case 7:
                menuStack(S);
                break;
            case 8:
                menuStatistik(T);
                break;
            case 0:
                cout << "\nMembersihkan seluruh alokasi memori heap...\n";
                delAllTree(T);
                delAllQueue(Q);
                delAllStack(S);
                cout << "=======================================================\n";
                cout << " Terima kasih telah menggunakan Sistem Penyewaan Outdoor!\n";
                cout << "=======================================================\n";
                break;
            default:
                break;
        }
    }

    return 0;
}
