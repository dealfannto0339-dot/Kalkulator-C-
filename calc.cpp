
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <stdexcept>
#include <cctype>

using namespace std;

vector<string> history;

double ans = 0;
const int presisi = 2;

string formatAngka(double nilai) {
    ostringstream ss;
    ss << fixed << setprecision(presisi) << nilai;
    return ss.str();
}

void clearScreen() {
    system("clear");
}

void garis() {
    cout << "============================================\n";
}

void judul() {
    cout << "\n             K A L K U L A T O R\n";
}

void anggota() {
    cout << "                KELAS XI-D\n";

    garis();

    cout << "             ANGGOTA KELOMPOK\n";

    cout << "  01. Aurel Fitria Rahmadhani\n";
    cout << "  15. Auri Surya Alam\n";
    cout << "  19. Chika A'yunin Rahmawati\n";
    cout << "  21. David Arya Wijaya\n";
    cout << "  22. Davinka Ganza Gifany\n";
    cout << "  30. M. Zidny Alfan Syammil Haqqi\n";

    garis();
}

class Parser {
    string s;
    size_t pos = 0;

    void skip() {
        while (pos < s.size() &&
               isspace((unsigned char)s[pos]))
            pos++;
    }

    bool match(char c) {
        skip();

        if (pos < s.size() && s[pos] == c) {
            pos++;
            return true;
        }

        return false;
    }

    double number() {
        skip();

        if (s.compare(pos, 3, "Ans") == 0 ||
            s.compare(pos, 3, "ans") == 0) {
            pos += 3;
            return ans;
        }

        if (s.compare(pos, 3, "√") == 0) {
            pos += 3;

            double x = factor();

            if (x < 0)
                throw runtime_error("Akar negatif tidak valid!");

            return sqrt(x);
        }

        if (match('(')) {
            double x = expression();

            if (!match(')'))
                throw runtime_error("Kurung tidak lengkap!");

            return x;
        }

        size_t start = pos;

        while (pos < s.size() &&
               (isdigit((unsigned char)s[pos]) || s[pos] == '.'))
            pos++;

        if (start == pos)
            throw runtime_error("Input tidak valid!");

        return stod(s.substr(start, pos - start));
    }

    double factor() {
        if (match('+'))
            return factor();

        if (match('-'))
            return -factor();

        double x = number();

        if (match('^'))
            x = pow(x, factor());

        return x;
    }

    double term() {
        double x = factor();

        while (true) {
            if (match('*')) {
                x *= factor();
            }
            else if (match('/')) {
                double y = factor();

                if (y == 0)
                    throw runtime_error("Tidak bisa dibagi nol!");

                x /= y;
            }
            else {
                break;
            }
        }

        return x;
    }

    double expression() {
        double x = term();

        while (true) {
            if (match('+'))
                x += term();
            else if (match('-'))
                x -= term();
            else
                break;
        }

        return x;
    }

public:
    Parser(string input) {
        s = input;

        size_t p;

        while ((p = s.find("×")) != string::npos)
            s.replace(p, 3, "*");

        while ((p = s.find("÷")) != string::npos)
            s.replace(p, 3, "/");

        while ((p = s.find("−")) != string::npos)
            s.replace(p, 3, "-");
    }

    double calculate() {
        double hasil = expression();

        skip();

        if (pos != s.size())
            throw runtime_error("Format perhitungan salah!");

        if (!isfinite(hasil))
            throw runtime_error("Hasil tidak valid!");

        return hasil;
    }
};

void menuUtama() {
    clearScreen();

    judul();
    anggota();

    cout << "                MAIN MENU\n";

    cout << "  [1] Kalkulator\n";
    cout << "  [2] Konversi Satuan\n";
    cout << "  [3] Riwayat Perhitungan\n";
    cout << "  [4] Hapus Riwayat\n";
    cout << "  [0] Keluar\n";

    garis();

    cout << "  Ans : " << formatAngka(ans) << "\n";
    cout << "  Pilih Menu : ";
}

void kalkulator() {
    string input;

    while (true) {
        clearScreen();
        judul();

        cout << "              KALKULATOR\n";

        garis();

        cout << "  PANDUAN SIMBOL\n";

        cout << "  +   Penjumlahan\n";
        cout << "  -   Pengurangan\n";
        cout << "  *   Perkalian (x)\n";
        cout << "  /   Pembagian (:)\n";
        cout << "  ^   Pangkat\n";
        cout << "  √   Akar kuadrat\n";
        cout << "  ( ) Kurung\n";
        cout << "  Ans Hasil sebelumnya\n";

        garis();

        cout << "  Ans : " << formatAngka(ans) << "\n";

        cout << "\n  Masukkan Perhitungan\n";

        cout << "  Ketik 0 untuk kembali\n";

        cout << "\n  Input : ";
        getline(cin, input);

        if (input == "0")
            break;

        if (input.empty())
            continue;

        try {
            Parser parser(input);
            double hasil = parser.calculate();

            ans = hasil;

            cout << "\n";
            garis();

            cout << "  HASIL : " << formatAngka(hasil) << "\n";

            garis();

            history.push_back(
                input + " = " + formatAngka(hasil)
            );

        }
        catch (exception &e) {
            cout << "\n  ERROR : " << e.what() << "\n";
        }

        cout << "\n  Tekan Enter untuk menghitung lagi...";
        cin.get();
    }
}

void konversi() {
    int kategori, asal, tujuan;
    double nilai, hasil;

    string satuan[3][7] = {
        {"km", "hm", "dam", "m", "dm", "cm", "mm"},
        {"ton", "kuintal", "kg", "hg", "dag", "g", "mg"},
        {"hari", "jam", "menit", "detik", "ms", "", ""}
    };

    double faktor[3][7] = {
        {1000000, 100000, 10000, 1000, 100, 10, 1},
        {1000000, 100000, 1000, 100, 10, 1, 0.001},
        {86400, 3600, 60, 1, 0.001, 0, 0}
    };

    while (true) {
        clearScreen();
        judul();

        cout << "              KONVERSI SATUAN\n";

        garis();

        cout << "  [1] Panjang\n";
        cout << "  [2] Berat\n";
        cout << "  [3] Waktu\n";
        cout << "  [4] Suhu\n";
        cout << "  [0] Kembali\n";

        cout << "\n  Pilih Kategori : ";
        cin >> kategori;

        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\n  Input tidak valid!\n";
            cout << "\n  Tekan Enter...";
            cin.get();
            continue;
        }

        if (kategori == 0)
            break;

        if (kategori < 1 || kategori > 4) {
            cout << "\n  Kategori tidak tersedia!\n";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\n  Tekan Enter...";
            cin.get();
            continue;
        }

        cout << "\n  Masukkan Nilai : ";
        cin >> nilai;

        if (!cin || !isfinite(nilai)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\n  Nilai tidak valid!\n";
            cout << "\n  Tekan Enter...";
            cin.get();
            continue;
        }

        if (kategori == 4) {
            int mode;

            cout << "\n  [1] Celsius ke Fahrenheit\n";
            cout << "  [2] Fahrenheit ke Celsius\n";
            cout << "  [3] Celsius ke Kelvin\n";
            cout << "  [4] Kelvin ke Celsius\n";
            cout << "  [5] Fahrenheit ke Kelvin\n";
            cout << "  [6] Kelvin ke Fahrenheit\n";

            cout << "\n  Pilih Konversi : ";
            cin >> mode;

            if (!cin || mode < 1 || mode > 6) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\n  Pilihan tidak tersedia!\n";
                cout << "\n  Tekan Enter...";
                cin.get();
                continue;
            }

            if (((mode == 1 || mode == 3) && nilai < -273.15) ||
                ((mode == 2 || mode == 5) && nilai < -459.67) ||
                ((mode == 4 || mode == 6) && nilai < 0)) {

                cout << "\n  Suhu tidak valid!\n";

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\n  Tekan Enter...";
                cin.get();
                continue;
            }

            switch (mode) {
                case 1:
                    hasil = nilai * 9 / 5 + 32;
                    break;

                case 2:
                    hasil = (nilai - 32) * 5 / 9;
                    break;

                case 3:
                    hasil = nilai + 273.15;
                    break;

                case 4:
                    hasil = nilai - 273.15;
                    break;

                case 5:
                    hasil = (nilai - 32) * 5 / 9 + 273.15;
                    break;

                case 6:
                    hasil = (nilai - 273.15) * 9 / 5 + 32;
                    break;
            }

            cout << "\n";
            garis();

            cout << "  HASIL KONVERSI : "
                 << formatAngka(hasil) << "\n";

            garis();
        }
        else {
            int k = kategori - 1;

            cout << "\n  DAFTAR SATUAN\n";

            for (int i = 0; i < 7; i++) {
                if (satuan[k][i] != "")
                    cout << "  [" << i + 1 << "] "
                         << satuan[k][i] << "\n";
            }

            cout << "\n  Satuan Asal : ";
            cin >> asal;

            cout << "  Satuan Tujuan : ";
            cin >> tujuan;

            if (!cin) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\n  Input tidak valid!\n";
                cout << "\n  Tekan Enter...";
                cin.get();
                continue;
            }

            if (asal < 1 || asal > 7 ||
                tujuan < 1 || tujuan > 7 ||
                satuan[k][asal - 1] == "" ||
                satuan[k][tujuan - 1] == "") {

                cout << "\n  Satuan tidak tersedia!\n";

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\n  Tekan Enter...";
                cin.get();
                continue;
            }

            hasil = nilai * faktor[k][asal - 1]
                    / faktor[k][tujuan - 1];

            cout << "\n";
            garis();

            cout << "  HASIL KONVERSI : "
                 << formatAngka(hasil) << " "
                 << satuan[k][tujuan - 1] << "\n";

            garis();
        }

        history.push_back(
            "Konversi satuan = " + formatAngka(hasil)
        );

        ans = hasil;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\n  Tekan Enter untuk konversi lagi...";
        cin.get();
    }
}

void tampilHistory() {
    clearScreen();
    judul();

    cout << "           RIWAYAT PERHITUNGAN\n";

    garis();

    if (history.empty()) {
        cout << "  Belum ada riwayat perhitungan.\n";
    }
    else {
        for (int i = 0; i < history.size(); i++) {
            cout << "  " << i + 1 << ". "
                 << history[i] << "\n";
        }
    }

    garis();
}

void hapusHistory() {
    history.clear();

    cout << "\n  Riwayat berhasil dihapus!\n";
}

int main() {
    int menu;

    do {
        menuUtama();

        cin >> menu;

        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\n  Input tidak valid!\n";

            cout << "\n  Tekan Enter untuk kembali...";
            cin.get();
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (menu) {
            case 1:
                kalkulator();
                break;

            case 2:
                konversi();
                break;

            case 3:
                tampilHistory();
                break;

            case 4:
                hapusHistory();
                break;

            case 0:
                clearScreen();
                judul();
                garis();

                cout << "  Terima kasih telah menggunakan\n"
                     << "  kalkulator kami!\n";

                garis();
                break;

            default:
                cout << "\n  Menu tidak tersedia!\n";
        }

        if (menu != 0) {
            cout << "\n  Tekan Enter untuk kembali...";
            cin.get();
        }

    } while (menu != 0);

    return 0;
}
