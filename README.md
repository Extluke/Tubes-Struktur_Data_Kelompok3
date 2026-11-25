# Sistem Manajemen Penyewaan Perlengkapan Outdoor / Camping
**Tugas Besar Algoritma & Struktur Data (C++ Pure Pointer ADT - Zero STL)**

---

## 📋 Deskripsi Proyek
Proyek ini adalah aplikasi konsol (CLI) sistem informasi penyewaan perlengkapan *outdoor* dan *camping* yang dikembangkan menggunakan bahasa C++. Seluruh struktur data dibangun secara mandiri (*from scratch*) berbasis alokasi memori dinamis dan pointer murni (`new` / `delete`) tanpa menggunakan Standard Template Library (STL) seperti `std::vector`, `std::list`, `std::queue`, `std::stack`, maupun `std::map`.

Dokumentasi kode mengikuti standar modul praktikum perkuliahan dengan komentar spesifikasi formal:
```cpp
/* I.S. ...
   F.S. ... */
```
serta deskripsi nilai balik untuk fungsi.

---

## 🏗️ Arsitektur 4 Struktur Data (Pointer-Based)

```
                    [ BINARY SEARCH TREE (BST) ]
                     Data Penyewa (Key: Nama)
                           /         \
                         /             \
                   [Node Kiri]     [Node Kanan]
                       |
            +----------+----------+
            |  Child Singly List  | (MLL)
            |   Daftar Alat Sewa  |
            +----------+----------+
                       |
               [Alat 1] -> [Alat 2] -> Nil
```
```
   [ QUEUE FIFO (Inspeksi & Cuci) ]      [ STACK LIFO (Riwayat Selesai) ]
     Head                       Tail       Top (Terbaru)
   +------+   +------+   +------+        +-------------+
   | Alat |-->| Alat |-->| Alat |        | Transaksi 3 |
   +------+   +------+   +------+        +-------------+
     v                                          |
   (Keluar Pertama untuk Dicuci)         +-------------+
                                         | Transaksi 2 |
                                         +-------------+
                                                |
                                         +-------------+
                                         | Transaksi 1 |
                                         +-------------+
```

1. **Binary Search Tree (BST) - Data Penyewa**:
   - Menyimpan seluruh data penyewa aktif yang terurut secara alfabetis berdasarkan kunci unik `namaPenyewa` (case-insensitive).
   - Memiliki kompleksitas pencarian rata-rata $O(\log N)$.
2. **Multiple Linked List (MLL) - Daftar Perlengkapan**:
   - Sub-list bertipe *Singly Linked List* yang berada di dalam setiap node penyewa BST.
   - Menyimpan rincian alat yang disewa (`namaAlat`, `jumlah` unit, dan `hargaPerHari`).
3. **Queue (Pointer / FIFO) - Antrean Inspeksi & Pencucian**:
   - Menampung perlengkapan yang baru saja dikembalikan oleh penyewa untuk diperiksa kelengkapannya dan dicuci oleh staf sebelum disewakan kembali.
   - Elemen terdepan (`head`) diproses terlebih dahulu (*First In First Out*).
4. **Stack (Pointer / LIFO) - Riwayat Transaksi Selesai**:
   - Mencatat arsip transaksi sewa yang telah selesai dan lunas.
   - Transaksi yang paling baru selesai berada di puncak (`Top`) tumpukan (*Last In First Out*), serta mengkalkulasi akumulasi total omzet/pendapatan sistem.

---

## 🎯 Pemetaan 13 Challenges Praktikum

| No | Challenge | Modul & Fungsi Terkait | Deskripsi Implementasi |
|:---:|:---|:---|:---|
| **C1** | Insert Penyewa ke BST | `bst_mll.cpp` -> `insertNodePenyewa` | Rekursif BST alfabetis. Jika nama duplikat, alokasi dibatalkan & dicegah *memory leak*. |
| **C2** | Insert & Update Alat (MLL) | `bst_mll.cpp` -> `insertLastAlat`, `updateDurasi`, `updateKategori` | Insert last pada child list; jika alat sudah ada, kuantitas unit otomatis diakumulasi. |
| **C3** | 4 Traversal BST | `bst_mll.cpp` -> `printInOrder`, `printPreOrder`, `printPostOrder`, `printLevelOrder` | In-Order (A-Z), Pre-Order, Post-Order, dan **Level-Order BFS menggunakan Queue pointer manual (No STL)**. |
| **C4** | Pencarian Multi-Kriteria | `bst_mll.cpp` -> `findNodePenyewa`, `cariByAlat`, `cariByDurasi` | Pencarian rekursif by nama ($O(\log N)$), by nama alat yang disewa, dan by durasi sewa. |
| **C5** | Delete Node BST (3 Kasus) | `bst_mll.cpp` -> `deleteNodePenyewa`, `mostLeft` | Menangani kasus node *Leaf*, *1 Child*, dan *2 Children* menggunakan *successor* (`mostLeft(T->right)`). |
| **C6** | Reset Durasi Sewa | `bst_mll.cpp` -> `hapusDurasiPenyewa` | Mereset `durasiSewa = 0` (mendukung reset per nama spesifik atau reset massal by batas durasi). |
| **C7** | Hapus Alat dari Penyewa | `bst_mll.cpp` -> `hapusAlatDariPenyewa` | Menghapus 1 node alat dari sub-list MLL (delete first, tengah, atau last) lalu `dealokasiAlat`. |
| **C8** | Queue Antrean Inspeksi | `queue_stack.cpp` -> `enQueue`, `deQueue`, `printQueue`, `delAllQueue` | Antrean FIFO berbasis pointer (`head` & `tail`) untuk pengecekan & pencucian perlengkapan. |
| **C9** | Stack Riwayat Transaksi | `queue_stack.cpp` -> `push`, `pop`, `peekTop`, `printStack`, `delAllStack` | Tumpukan LIFO berbasis pointer (`Top`) untuk log transaksi selesai dan total omzet pendapatan. |
| **C10** | Total Penyewa Aktif | `bst_mll.cpp` -> `hitungTotalPenyewa` | Menghitung total node pada BST secara rekursif ($1 + L + R$). |
| **C11** | Filter & Hitung by Kategori | `bst_mll.cpp` -> `viewDanHitungByKategori` | Traversal dan agregasi penyewa berdasarkan 4 kategori resmi (*Solo Camp*, *Family Camp*, *Hiking*, *Ekspedisi*). |
| **C12** | Statistik Durasi Sewa | `bst_mll.cpp` -> `tampilkanStatistikDurasi` | Menganalisis durasi Terlama (*Max*), Tercepat (*Min*, mengabaikan durasi 0), dan Rata-rata (*Average*). |
| **C13** | Filter by Jumlah Jenis Alat | `bst_mll.cpp` -> `cariByJumlahAlat`, `hitungJumlahJenisAlat` | Menghitung banyaknya simpul (node) pada child list MLL dan menampilkan penyewa yang tepat sesuai target. |

---

## 🔄 Alur Integrasi Antar Struktur Data

Salah satu fitur utama sistem ini adalah **Menu 5: Proses Pengembalian Barang**, yang mengintegrasikan seluruh struktur data dalam satu siklus transaksi:

```
[ 1. Cari Penyewa di BST ]
             |
             v
[ 2. Hitung Total Biaya & Buat Ringkasan Alat ]
             |
             +------------------------------+
             |                              |
             v                              v
[ 3. Masukkan ke Queue Inspeksi ]   [ 4. Arsipkan ke Stack Riwayat ]
     Status: "Menunggu Cek & Cuci"       Tercatat di Puncak (Top)
             |                              |
             +--------------+---------------+
                            |
                            v
[ 5. Hapus Node Penyewa Aktif dari BST ]
     (Penyewaan Selesai, Memori BST Dibersihkan)
```

1. **Queue Inspeksi (Menu 6)**: Staf toko dapat menjalankan `deQueue` untuk memproses alat dari antrean terdepan setelah alat selesai diperiksa kelengkapannya dan dicuci bersih.
2. **Stack Riwayat (Menu 7)**: Manajer/kasir dapat melihat seluruh riwayat sewa dari yang paling baru (`Top`), memantau transaksi terakhir (`peekTop`), atau menghitung total pendapatan keseluruhan.

---

## 📂 Struktur Berkas Proyek

```text
Tugas Besar/
│
├── bst_mll.h           # Header ADT BST Penyewa & Child MLL Daftar Alat (+ Queue Bantu BFS)
├── bst_mll.cpp         # Implementasi seluruh logika pointer BST & MLL (Zero STL)
│
├── queue_stack.h       # Header ADT Queue Inspeksi (FIFO) & Stack Riwayat (LIFO)
├── queue_stack.cpp     # Implementasi seluruh logika pointer Queue & Stack (Zero STL)
│
├── main.cpp            # Menu utama CLI terstruktur, input handler, & dummy data loader
├── README.md           # Dokumentasi teknis lengkap dan panduan pengujian
└── rental_outdoor.exe  # Binary executable hasil kompilasi g++
```

---

## ⚙️ Panduan Kompilasi & Menjalankan Program

### 1. Prasyarat Lingkungan
- Kompiler C++: `g++` (MinGW / GCC versi 4.8 ke atas dengan dukungan C++11).
- Sistem Operasi: Windows 10/11, Linux, atau macOS.

### 2. Kompilasi Program
Buka terminal (PowerShell / Command Prompt / Terminal) di direktori proyek, lalu jalankan perintah kompilasi berikut:

```bash
g++ -Wall -Wextra -std=c++11 -O2 main.cpp bst_mll.cpp queue_stack.cpp -o rental_outdoor.exe
```

> **Catatan**: Flag `-Wall -Wextra` memastikan program bebas dari segala jenis *warning* dan *syntax error*.

### 3. Menjalankan Program
Eksekusi file binary yang telah dibuat:

**Windows (PowerShell / CMD):**
```powershell
.\rental_outdoor.exe
```

**Linux / macOS:**
```bash
./rental_outdoor.exe
```

---

## 🧪 Data Demo / Dummy Pengujian Awal

Saat program pertama kali dijalankan, sistem secara otomatis memuat sejumlah data sampel realistis untuk mempermudah penilaian seluruh skenario pengujian tanpa harus menginput manual dari awal:

### 1. Data Penyewa Aktif di BST & MLL:
1. **Andi Wijaya** (*Solo Camp*, 2 Hari)
   - Tenda Single Person (1 unit @ Rp 30.000/hari)
   - Sleeping Bag Polar (1 unit @ Rp 15.000/hari)
   - Headlamp LED (1 unit @ Rp 10.000/hari)
   - *Total: 3 unit (3 jenis) | Estimasi Biaya: Rp 110.000*
2. **Budi Santoso** (*Family Camp*, 3 Hari)
   - Tenda Dome Kapasitas 4 (2 unit @ Rp 45.000/hari)
   - Matras Alumunium (4 unit @ Rp 10.000/hari)
   - Kompor Portable & Nesting (1 unit @ Rp 25.000/hari)
   - *Total: 7 unit (3 jenis) | Estimasi Biaya: Rp 465.000*
3. **Citra Lestari** (*Hiking/Pendakian*, 4 Hari)
   - Carrier 60L Consina (1 unit @ Rp 35.000/hari)
   - Trekking Pole (2 unit @ Rp 15.000/hari)
   - Sleeping Bag Polar (1 unit @ Rp 15.000/hari)
   - *Total: 4 unit (3 jenis) | Estimasi Biaya: Rp 320.000*
4. **Dimas Pratama** (*Ekspedisi*, 5 Hari)
   - Tenda Dome Kapasitas 4 (1 unit @ Rp 45.000/hari)
   - Carrier 60L Consina (2 unit @ Rp 35.000/hari)
   - Flysheet 3x4 Meter (1 unit @ Rp 20.000/hari)
   - Kompor Portable & Nesting (1 unit @ Rp 25.000/hari)
   - *Total: 5 unit (4 jenis) | Estimasi Biaya: Rp 800.000*
5. **Eka Rahmawati** (*Solo Camp*, 1 Hari)
   - Hammock & Webbing (1 unit @ Rp 15.000/hari)
   - Lampu Tenda Gantung (1 unit @ Rp 10.000/hari)
   - *Total: 2 unit (2 jenis) | Estimasi Biaya: Rp 25.000*

### 2. Data Awal Queue Inspeksi (FIFO):
- **Gilang Ramadhan**: 1x Carrier 60L, 2x Matras Alumunium (Status: `Menunggu Cek`)

### 3. Data Awal Stack Riwayat (LIFO):
- **Hendra Kusuma**: Paket Solo Camp, 2 Hari (Total Transaksi: `Rp 90.000`)

---

## 🛡️ Manajemen Memori & Clean-Up
Ketika pengguna memilih menu `0. Keluar Program`, fungsi destruksi rekursif dipanggil untuk membersihkan seluruh alokasi memori dinamis di heap:
- `delAllTree(T)`: Menghapus seluruh node BST beserta child list MLL secara *post-order*.
- `delAllQueue(Q)`: Mendealokasikan seluruh elemen antrean inspeksi.
- `delAllStack(S)`: Mendealokasikan seluruh elemen riwayat transaksi.

Hal ini menjamin **0% memory leak** saat aplikasi ditutup.
#   T u b e s - S t r u k t u r _ D a t a _ K e l o m p o k 3  
 