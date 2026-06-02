#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

using namespace std;

class TokoElektronik {
private:
    // Array dengan kapasitas 3 elemen (prinsip enkapsulasi)
    string etalase[3];
    
public:
    // Constructor untuk mengisi data awal ke array
    TokoElektronik() {
        etalase[0] = "TV Samsung 43 Inch";
        etalase[1] = "Laptop Asus ROG";
        etalase[2] = "Smartphone iPhone 14";
    }
    
    // Method untuk mengambil produk dengan parameter nomorRak
    string ambilProduk(size_t nomorRak) {
        try {
            // Menggunakan .at() untuk akses array dengan pengecekan batas
            return etalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // Menangkap error out_of_range dan melempar pesan kustom
            throw runtime_error("Gagal Mengambil Barang : Rak nomor " + 
                               to_string(nomorRak) + " kosong atau tidak tersedia!");
        }
    }
    
    // Method untuk menampilkan isi etalase
    void tampilkanEtalase() {
        cout << "\n=== ISI ETALASE TOKO ELEKTRONIK 'GIBRAN JAYA' ===" << endl;
        for (int i = 0; i < 3; i++) {
            cout << "Rak " << i << ": " << etalase[i] << endl;
        }
        cout << "=================================================" << endl;
    }
};

class ManajemenGudang {
private:
    string namaFile;
    
public:
    ManajemenGudang(string filename = "gudang.txt") : namaFile(filename) {}
    
    // CREATE: Menambahkan data barang baru
    void tambahBarang() {
        ofstream file(namaFile, ios::app); // Mode append
        if (!file.is_open()) {
            cout << "Error: Gagal membuka file!" << endl;
            return;
        }
        
        string namaBarang;
        int harga;
        int stok;
        
        cout << "\n--- TAMBAH BARANG BARU ---" << endl;
        cout << "Nama Barang: ";
        cin.ignore();
        getline(cin, namaBarang);
        cout << "Harga: Rp ";
        cin >> harga;
        cout << "Stok: ";
        cin >> stok;
        
        // Format penyimpanan: NamaBarang|Harga|Stok
        file << namaBarang << "|" << harga << "|" << stok << endl;
        
        file.close();
        cout << "✓ Barang berhasil ditambahkan!" << endl;
    }
    
    // READ: Membaca dan menampilkan daftar barang
    void bacaBarang() {
        ifstream file(namaFile);
        if (!file.is_open()) {
            cout << "\n=== DAFTAR BARANG GUDANG ===" << endl;
            cout << "Gudang masih kosong!" << endl;
            cout << "=============================" << endl;
            return;
        }
        
        cout << "\n=== DAFTAR BARANG GUDANG ===" << endl;
        cout << left << setw(5) << "No" 
             << setw(25) << "Nama Barang" 
             << setw(15) << "Harga" 
             << setw(10) << "Stok" << endl;
        cout << "-----------------------------------------------" << endl;
        
        string line;
        int nomor = 1;
        while (getline(file, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            
            if (pos1 != string::npos && pos2 != string::npos) {
                string nama = line.substr(0, pos1);
                string harga = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string stok = line.substr(pos2 + 1);
                
                cout << left << setw(5) << nomor++
                     << setw(25) << nama
                     << setw(15) << ("Rp " + harga)
                     << setw(10) << stok << endl;
            }
        }
        
        file.close();
        cout << "=============================" << endl;
    }
    
    // UPDATE: Memperbarui data barang tertentu
    void updateBarang() {
        vector<string> dataBarang;
        ifstream fileBaca(namaFile);
        
        if (!fileBaca.is_open()) {
            cout << "Error: File tidak ditemukan!" << endl;
            return;
        }
        
        string line;
        while (getline(fileBaca, line)) {
            dataBarang.push_back(line);
        }
        fileBaca.close();
        
        if (dataBarang.empty()) {
            cout << "Tidak ada data untuk diupdate!" << endl;
            return;
        }
        
        // Tampilkan data
        cout << "\n=== UPDATE DATA BARANG ===" << endl;
        for (size_t i = 0; i < dataBarang.size(); i++) {
            size_t pos1 = dataBarang[i].find('|');
            string nama = dataBarang[i].substr(0, pos1);
            cout << i + 1 << ". " << nama << endl;
        }
        
        int pilihan;
        cout << "Pilih nomor barang yang akan diupdate: ";
        cin >> pilihan;
        
        if (pilihan < 1 || pilihan > (int)dataBarang.size()) {
            cout << "Pilihan tidak valid!" << endl;
            return;
        }
        
        string namaBaru;
        int hargaBaru;
        int stokBaru;
        
        cout << "\nMasukkan data baru:" << endl;
        cout << "Nama Barang: ";
        cin.ignore();
        getline(cin, namaBaru);
        cout << "Harga: Rp ";
        cin >> hargaBaru;
        cout << "Stok: ";
        cin >> stokBaru;
        
        // Update data
        dataBarang[pilihan - 1] = namaBaru + "|" + to_string(hargaBaru) + "|" + to_string(stokBaru);
        
        // Tulis kembali ke file
        ofstream fileTulis(namaFile);
        for (const auto& item : dataBarang) {
            fileTulis << item << endl;
        }
        fileTulis.close();
        
        cout << "✓ Data barang berhasil diupdate!" << endl;
    }
    
    // DELETE: Menghapus data barang
    void hapusBarang() {
        vector<string> dataBarang;
        ifstream fileBaca(namaFile);
        
        if (!fileBaca.is_open()) {
            cout << "Error: File tidak ditemukan!" << endl;
            return;
        }
        
        string line;
        while (getline(fileBaca, line)) {
            dataBarang.push_back(line);
        }
        fileBaca.close();
        
        if (dataBarang.empty()) {
            cout << "Tidak ada data untuk dihapus!" << endl;
            return;
        }
        
        // Tampilkan data
        cout << "\n=== HAPUS DATA BARANG ===" << endl;
        for (size_t i = 0; i < dataBarang.size(); i++) {
            size_t pos1 = dataBarang[i].find('|');
            string nama = dataBarang[i].substr(0, pos1);
            cout << i + 1 << ". " << nama << endl;
        }
        
        int pilihan;
        cout << "Pilih nomor barang yang akan dihapus: ";
        cin >> pilihan;
        
        if (pilihan < 1 || pilihan > (int)dataBarang.size()) {
            cout << "Pilihan tidak valid!" << endl;
            return;
        }
        
        string namaHapus;
        size_t pos1 = dataBarang[pilihan - 1].find('|');
        namaHapus = dataBarang[pilihan - 1].substr(0, pos1);
        
        dataBarang.erase(dataBarang.begin() + (pilihan - 1));
        
        // Tulis kembali ke file
        ofstream fileTulis(namaFile);
        for (const auto& item : dataBarang) {
            fileTulis << item << endl;
        }
        fileTulis.close();
        
        cout << "✓ Barang '" << namaHapus << "' berhasil dihapus!" << endl;
    }
};

void simulasiEtalase(TokoElektronik& toko) {
    cout << "\n========== SIMULASI ETALASE ==========" << endl;
    
    // Tampilkan isi etalase
    toko.tampilkanEtalase();
    
    // Skenario 1: Pengambilan barang di rak indeks ke-1
    cout << "\n--- SKENARIO 1: Rak indeks ke-1 ---" << endl;
    try {
        string produk = toko.ambilProduk(1);
        cout << "✓ Berhasil mengambil barang dari rak 1: " << produk << endl;
    } catch (const runtime_error& e) {
        cout << "✗ Error: " << e.what() << endl;
    }
    
    // Skenario 2: Pengambilan barang di rak indeks ke-5
    cout << "\n--- SKENARIO 2: Rak indeks ke-5 ---" << endl;
    try {
        string produk = toko.ambilProduk(5);
        cout << "✓ Berhasil mengambil barang dari rak 5: " << produk << endl;
    } catch (const runtime_error& e) {
        cout << "✗ Error: " << e.what() << endl;
    }
    
    cout << "=====================================" << endl;
}

void tampilkanMenu() {
    cout << "\n╔══════════════════════════════════════╗" << endl;
    cout << "║   SISTEM MANAJEMEN GUDANG            ║" << endl;
    cout << "║   TOKO ELEKTRONIK 'GIBRAN JAYA'      ║" << endl;
    cout << "╠══════════════════════════════════════╣" << endl;
    cout << "║ 1. Tambah Barang (Create)            ║" << endl;
    cout << "║ 2. Lihat Barang (Read)               ║" << endl;
    cout << "║ 3. Update Barang                     ║" << endl;
    cout << "║ 4. Hapus Barang                      ║" << endl;
    cout << "║ 5. Simulasi Etalase                  ║" << endl;
    cout << "║ 6. Keluar                            ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    cout << "Pilihan Anda: ";
}

int main() {
    TokoElektronik toko;
    ManajemenGudang gudang;
    int pilihan;
    
    cout << "\n=========================================" << endl;
    cout << "   SELAMAT DATANG DI GIBRAN JAYA ELEKTRONIK" << endl;
    cout << "=========================================" << endl;
    
    do {
        tampilkanMenu();
        cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                gudang.tambahBarang();
                break;
            case 2:
                gudang.bacaBarang();
                break;
            case 3:
                gudang.updateBarang();
                break;
            case 4:
                gudang.hapusBarang();
                break;
            case 5:
                simulasiEtalase(toko);
                break;
            case 6:
                cout << "\nTerima kasih telah menggunakan sistem ini!" << endl;
                cout << "Sampai jumpa kembali di Gibran Jaya Elektronik!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid! Silakan coba lagi." << endl;
        }
        
    } while (pilihan != 6);
    
    return 0;
}