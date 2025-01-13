#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <conio.h>
#include <sstream>
#include <limits>
#include <iomanip>
#include <cmath>

#ifdef _WIN32
#include <windows.h> // Required for system("cls") in Windows
#else
#include <cstdlib> // Required for system("clear") in Unix-based systems
#endif

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix-based systems
#endif
}
// Struktur data untuk informasi rekening
struct Account {
    string username;
    string password;
    string alamat;
    long long int saldo;
};

// Fungsi untuk mendaftarkan rekening baru
void daftarAkun(vector<Account>& accounts) {
    if (accounts.size() >= 10) {
        cout << "\n Batas jumlah rekening telah tercapai.";
        return;
    }

    Account akunBaru;
    cout << " __________________________________________________\n";
    cout << "|                                                  |\n";
    cout << "|                 Form Pendaftaran                 |\n";
    cout << "|__________________________________________________|\n";

    bool validasiUsername = false;
    while (!validasiUsername) {
        cout << "\n Masukkan Username : ";
        cin.ignore();
        getline(cin, akunBaru.username);

        // Cek apakah username kosong
        if (akunBaru.username.empty()) {
            cout << "\n Username tidak boleh kosong. Harap masukkan username.\n";
            cout << "\n Tekan tombol apa saja untuk kembali ke menu >> ";
            _getch();
            clearScreen();
            return;
        }

        // Cek panjang username
        if (akunBaru.username.length() < 4 || akunBaru.username.length() > 32) {
            cout << "\n Panjang username mengurangi / melebihi batas.\n Harap masukkan username di antara 4 - 32 karakter.\n";
            cout << "\n Tekan tombol apa saja untuk kembali ke menu >> ";
            _getch();
            clearScreen();
            return;
        }

        // Cek apakah username mengandung spasi
        if (akunBaru.username.find(' ') != string::npos) {
            cout << "\n Username tidak boleh mengandung spasi.\n Harap masukkan username tanpa spasi.\n";
            cout << "\n Tekan tombol apa saja untuk kembali ke menu >> ";
            _getch();
            clearScreen();
            return;
        }

        // Cek apakah username sudah ada dalam daftar akun
        bool adaUsername = false;
        for (const auto& account : accounts) {
            if (account.username == akunBaru.username) {
                adaUsername = true;
                break;
            }
        }

        if (adaUsername) {
            cout << "\n Username sudah digunakan. Harap masukkan username lain.\n";
            continue;
        }

        validasiUsername = true;
    }

    // Membuat PIN harus 6 digit angka
    bool validasiPIN = false;
    char ch;
    while (!validasiPIN) {
        cout << "\n Masukkan PIN      : ";
        ch = _getch();
        while (ch != 13){
            akunBaru.password.push_back(ch);
            cout << "*";
            ch = _getch();
        }
        // Cek panjang password
        if (akunBaru.password.length() != 6) {
            cout << "\n PIN harus terdiri dari 6 angka. Coba lagi.\n";
            continue;
        }

        // Cek apakah password hanya terdiri dari angka
        bool isNumeric = true;
        for (char c : akunBaru.password) {
            if (!isdigit(c)) {
                isNumeric = false;
                break;
            }
        }

        if (!isNumeric) {
            cout << "\n PIN hanya boleh terdiri dari angka. Coba lagi.\n";
            continue;
        }

        validasiPIN = true;
    }

    cout << "\n Masukkan Alamat   : ";
    cin.ignore();
    getline(cin, akunBaru.alamat);
    akunBaru.saldo = 0.0;

    accounts.push_back(akunBaru);

    cout << "\n Rekening berhasil didaftarkan.\n";
    cout << "\n Tekan tombol apa saja untuk kembali ke menu >> ";
    _getch();
    clearScreen();
}

// Menyimpan data ke dalam file
void simpanData(const vector<Account>& accounts, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        for (const Account& account : accounts) {
            file << account.username
                << " " << account.password
                << " " << account.saldo
                << " " << account.alamat << endl;
        }
        file.close();
    } else {
        cout << "Gagal menyimpan data." << endl;
    }
}

// Memuat data untuk dibaca program
void loadData(vector<Account>& accounts, const string& filename) {
    ifstream file(filename);

    if (file.is_open()) {
        Account account;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> account.username >> account.password >> account.saldo;
            getline(ss, account.alamat);
            accounts.push_back(account);
        }
        file.close();
    } else {
        cout << "Gagal membuka file data akun." << endl;
    }
}

// Fungsi untuk melakukan login ke rekening
int login(const vector<Account>& accounts, string username, string password) {
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].username == username && accounts[i].password == password) {
            return i; // Mengembalikan indeks rekening yang sesuai
        }
    }

    return -1; // Jika rekening tidak ditemukan
}

// Fungsi untuk melakukan setor ke rekening
void setor(Account& account, double amount) {
    if (amount < 10000) {
        cout << "\n Jumlah setoran harus lebih dari atau sama dengan 10000." << endl;
        cout << "\n Tekan tombol apapun untuk melanjutkan...";
        cin.ignore();
        cin.get();
        system("cls"); // Membersihkan layar jika syarat tidak terpenuhi
        return;
    }

    account.saldo += amount;
    cout << "\n Deposit berhasil dilakukan. Saldo saat ini: " << account.saldo << endl;
    cout << "\n Tekan tombol apapun untuk melanjutkan...";
    cin.ignore();
    cin.get();
    system("cls"); // Membersihkan layar setelah setoran berhasil
}

// Fungsi untuk melakukan penarikan dari rekening
void tarikTunai(Account& account, double amount) {
    if (amount > account.saldo) {
        cout << "\n Saldo tidak mencukupi." << endl;
        cout << "\n Tekan tombol apapun untuk melanjutkan...";
        cin.ignore();
        cin.get();
        clearScreen();
        return;
    }

    if (amount < 50000) {
        cout << "\n Saldo tidak mencapai minimum (Rp. 50.000)." << endl;
        cout << "\n Tekan tombol apapun untuk melanjutkan...";
        cin.ignore();
        cin.get();
        clearScreen();
        return;
    }

    account.saldo -= amount;
    cout << "\n Penarikan berhasil dilakukan. Saldo saat ini: " << account.saldo << endl;
    cout << "\n Tekan tombol apapun untuk melanjutkan...";
    cin.ignore();
    cin.get();
    clearScreen();
}


// Fungsi hapus akun
void hapusAkun(vector<Account>& accounts, const string& username) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->username == username) {
            accounts.erase(it);
            cout << "\n Akun anda berhasil dihapus.";
            cout << "\n Tekan tombol apapun untuk melanjutkan...";
            _getch();
            clearScreen();
            return;
        }
    }
    cout << "\n Gagal menghapus akun.";
}

// Fungsi transfer
void transfer(vector<Account>& accounts, int loggedInAccount) {
    string senderUsername, receiverUsername;
    double amount;
    cout << " __________________________________________________\n";
    cout << "|                                                  |\n";
    cout << "|                     Transfer                     |\n";
    cout << "|                  Min. Rp. 5.000                  |\n";
    cout << "|__________________________________________________|\n";
    cout << "\n Masukkan username penerima : ";
    cin >> receiverUsername;

    int indexPengirim = loggedInAccount;
    int indexPenerima = -1;

    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].username == receiverUsername) {
            indexPenerima = i;
            break;
        }
    }

    if (indexPenerima == -1) {
        cout << "\n Akun penerima dengan username '" << receiverUsername << "' tidak ditemukan." << endl;
        cout << " Tekan tombol apapun untuk melanjutkan...";
        _getch();
        clearScreen();
    } else {
        cout << " Masukkan jumlah transfer   : ";
        cin >> amount;

        if (amount > accounts[indexPengirim].saldo) {
            cout << " Saldo tidak cukup." << endl;
            cout << " Tekan tombol apapun untuk melanjutkan...";
            _getch();
            clearScreen();
        } else if (amount < 5000){
            cout << " Minimal transfer Rp. 5.000" << endl;
            cout << " Tekan tombol apapun untuk melanjutkan...";
            _getch();
            clearScreen();
        } else {
            accounts[indexPengirim].saldo -= amount;
            accounts[indexPenerima].saldo += amount;
            cout << " Transfer berhasil dilakukan." << endl;
            cout << "\n Sisa saldo anda : " << accounts[indexPengirim].saldo << endl;
            cout << "\n Tekan tombol apapun untuk melanjutkan...";
            _getch();
            clearScreen();
        }
    }
}

int cariAkun(const vector<Account>& accounts, const string& username) {
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].username == username) {
            return i; // Mengembalikan indeks rekening yang sesuai
        }
    }
    return -1; // Jika rekening tidak ditemukan
}

void tampilkanPengguna(const string& filename) {
    ifstream file(filename);
    cout << endl;
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            // Membagi baris menjadi token menggunakan spasi sebagai pemisah
            vector<string> tokens;
            size_t pos = 0;
            while ((pos = line.find(' ')) != string::npos) {
                string token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);  // Menambahkan token terakhir

            if (!tokens.empty()) {
                string username = tokens[0];  // Mengambil token pertama sebagai username
                cout << " " << username << endl;
            }
        }

        file.close();
    } else {
        cout << "Gagal membuka file " << filename << endl;
    }
}

int main() {
    vector<Account> accounts;
    int choice, choice1, foundAccount;
    string username, password, cariUsername, filename = "accounts.txt";
    double amount;
    loadData(accounts, filename);
    do {
        username = "";
        password = "";
        cout << " _________________________________________________" << endl;
        cout << "|                    |=======|                    |" << endl;
        cout << "|             ========  ATM  ========             |" << endl;
        cout << "|____________________|=======|____________________|" << endl;
        cout << "|                                                 |" << endl;
        cout << "|[1]Daftar Rekening                               |" << endl;
        cout << "|[2]Login                                         |" << endl;
        cout << "|[3]Keluar                                        |" << endl;
        cout << "|_________________________________________________|\n" << endl;
        cout << " Pilihan Anda >> ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();  // Clear the error state of cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore remaining characters in the input buffer
            cout << "\n Pilihan tidak valid. Mohon masukkan angka.\n" << endl;
            cout << " Tekan tombol apapun untuk melanjutkan...";
            _getch();
            clearScreen();
            continue;  // Restart the loop
        }
        switch (choice) {
            case 1:
                daftarAkun(accounts);
                simpanData(accounts, filename);
                break;
            case 2:
                cout << "\n Masukkan Username : ";
                cin >> username;
                char ch;
                cout << "\n Masukkan PIN      : ";
                ch = _getch();
                while (ch != 13){
                    password.push_back(ch);
                    cout << "*";
                    ch = _getch();
                }
                int loggedInAccount = login(accounts, username, password);
                if (loggedInAccount != -1) {
                    clearScreen();
                    do {
                        cout << " _________________________________________________" << endl;
                        cout << "|                    |=======|                    |" << endl;
                        cout << "|             ========  ATM  ========             |" << endl;
                        cout << "|____________________|=======|____________________|" << endl;
                        cout << "|                                                 |" << endl;
                        cout << "|Selamat Datang, " << left << setw(32) << accounts[loggedInAccount].username << " |" << endl;
                        cout << "|                                                 |" << endl;
                        cout << "|[1]Deposit                                       |" << endl;
                        cout << "|[2]Penarikan                                     |" << endl;
                        cout << "|[3]Cek Saldo                                     |" << endl;
                        cout << "|[4]Ganti Password                                |" << endl;
                        cout << "|[5]Hapus Akun                                    |" << endl;
                        cout << "|[6]Transfer                                      |" << endl;
                        cout << "|[7]Cari Akun Berdasarkan Username                |" << endl;
                        cout << "|[8]Tampilkan Akun Lain                           |" << endl;
                        cout << "|[9]Keluar                                        |" << endl;
                        cout << "|_________________________________________________|\n" << endl;
                        cout << " Pilihan Anda >> ";
                        cin >> choice1;
                        if (cin.fail()) {
                            cin.clear();  // Clear the error state of cin
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore remaining characters in the input buffer
                            cout << "\n Pilihan tidak valid. Mohon masukkan angka.\n" << endl;
                            cout << " Tekan tombol apapun untuk melanjutkan...";
                            _getch();
                            clearScreen();
                            continue;  // Restart the loop
                        }
                        switch (choice1) {
                            case 1:
                                if (accounts.empty()) {
                                    cout << "\n Tidak ada rekening terdaftar.";
                                } else {
                                    cout << " __________________________________________________\n";
                                    cout << "|                                                  |\n";
                                    cout << "|                      Deposit                     |\n";
                                    cout << "|__________________________________________________|\n";
                                    cout << "\n Masukkan jumlah yang ingin setor: ";
                                    cin >> amount;
                                    setor(accounts[loggedInAccount], amount);
                                }
                                simpanData(accounts, filename);
                                break;
                            case 2:
                                if (accounts.empty()) {
                                    cout << "\n Tidak ada rekening terdaftar.";
                                } else {
                                    cout << " __________________________________________________\n";
                                    cout << "|                                                  |\n";
                                    cout << "|                    Penarikan                     |\n";
                                    cout << "|__________________________________________________|\n";
                                    do {
                                        do {
                                            cout << "\n Masukkan jumlah penarikan (Rp. 50.000): ";
                                            cin >> amount;
                                        } while (amount < 0);
                                    } while (fmod(amount, 50000) != 0);
                                    tarikTunai(accounts[loggedInAccount], amount);
                                }
                                simpanData(accounts, filename);
                                break;
                            case 3:
                                // Tampilkan informasi rekening
                                    cout << " __________________________________________________\n";
                                    cout << "|                                                  |\n";
                                    cout << "|                Saldo Rekening                    |\n";
                                    cout << "|__________________________________________________|\n";
                                    cout << "\n Saldo  : " << accounts[loggedInAccount].saldo << endl;
                                    cout << "\n_________________________________________________\n";
                                    cout << "\n Tekan tombol apapun untuk melanjutkan...";
                                    cin.ignore();
                                    cin.get();
                                    clearScreen();
                                break;
                            case 4:
                                // Ganti password
                                cin.ignore();
                                bool validasiPIN;
                                validasiPIN = false;
                                while (!validasiPIN) {
                                    cout << " __________________________________________________\n";
                                    cout << "|                                                  |\n";
                                    cout << "|                  Ganti Password                  |\n";
                                    cout << "|__________________________________________________|\n";
                                    cout << "\n Masukkan PIN baru: ";
                                    getline(cin, accounts[loggedInAccount].password);

                                    if (accounts[loggedInAccount].password.length() == 6) {
                                        validasiPIN = true;
                                    } else {
                                        cout << " PIN harus terdiri dari 6 angka. Coba lagi." << endl;
                                    }
                                }
                                cout << "\n PIN berhasil diubah." << endl;
                                simpanData(accounts, filename);
                                cout << "\n Tekan tombol apapun untuk melanjutkan...";
                                _getch();
                                clearScreen();
                                break;
                            case 5:
                                // Hapus akun
                                cout << " __________________________________________________\n";
                                cout << "|                                                  |\n";
                                cout << "|                    Hapus Akun                    |\n";
                                cout << "|__________________________________________________|\n";
                                cout << "\n Untuk mengkonfirmasi ulang harap:";
                                cout << "\n Masukkan PIN: ";
                                cin >> password;
                                if (password == accounts[loggedInAccount].password){
                                    hapusAkun(accounts, username);
                                } else {
                                    cout << "\n Gagal menghapus akun karena PIN salah!" << endl;
                                    cout << "\n Tekan tombol apapun untuk melanjutkan...";
                                    _getch();
                                    clearScreen();
                                    break;
                                }
                                choice1 = 9;
                                simpanData(accounts, filename);
                                break;
                            case 6:
                                transfer(accounts, loggedInAccount);
                                simpanData(accounts, filename);
                                break;
                            case 7:
                                cout << " __________________________________________________\n";
                                cout << "|                                                  |\n";
                                cout << "|                    Cari Akun                     |\n";
                                cout << "|__________________________________________________|\n";
                                cout << "\n Masukkan username  : ";
                                cin >> cariUsername;
                                int foundAccount;
                                foundAccount = cariAkun(accounts, cariUsername); // Memanggil fungsi mencari rekening berdasarkan username
                                if (foundAccount != -1) {
                                    cout << " Rekening dengan username " << cariUsername << " ditemukan." << endl;
                                    cout << "\n Tekan tombol apapun untuk melanjutkan...";
                                    _getch();
                                    clearScreen();
                                } else {
                                    cout << " Rekening dengan username " << cariUsername << " tidak ditemukan." << endl;
                                    cout << "\n Tekan tombol apapun untuk melanjutkan...";
                                    _getch();
                                    clearScreen();
                                }
                                simpanData(accounts, filename);
                                break;
                            case 8:
                                cout << " __________________________________________________\n";
                                cout << "|                                                  |\n";
                                cout << "|                    Daftar Akun                   |\n";
                                cout << "|__________________________________________________|\n";
                                tampilkanPengguna(filename);
                                cout << " ________________________________________ \n";
                                cout << "\n Tekan tombol apapun untuk melanjutkan...";
                                _getch();
                                clearScreen();
                                break;
                            case 9:
                                clearScreen();
                                break;
                            default:
                                cout << "\n Pilihan tidak valid." << endl;
                                break;
                        }
                    } while (choice1 != 9);
                } else {
                    cout << "\n Login gagal. Username atau password salah."  << endl;
                    cout << " Tekan tombol apapun untuk melanjutkan...";
                    cin.ignore();
                    cin.get();
                    clearScreen();
                }
                break;
                simpanData(accounts, filename);
        }
        cout << endl;
    } while (choice != 3);
    cout << "\n Terima kasih telah menggunakan layanan ATM.\n";
    simpanData(accounts, filename);
    return 0;
}
