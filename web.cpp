#include "calc.cpp"
#include <emscripten.h>

const string SATUAN[3][7] = {
    {"km", "hm", "dam", "m", "dm", "cm", "mm"},
    {"ton", "kuintal", "kg", "hg", "dag", "g", "mg"},
    {"hari", "jam", "menit", "detik", "ms", "", ""}
};

const double FAKTOR[3][7] = {
    {1000000, 100000, 10000, 1000, 100, 10, 1},
    {1000000, 100000, 1000, 100, 10, 1, 0.001},
    {86400, 3600, 60, 1, 0.001, 0, 0}
};

static string buffer;

static const char* kirim(const string &s) {
    buffer = s;
    return buffer.c_str();
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
const char* hitung(const char* input) {
    string ekspresi = input;

    try {
        Parser parser(ekspresi);
        double hasil = parser.calculate();

        ans = hasil;
        history.push_back(ekspresi + " = " + formatAngka(hasil));

        return kirim("OK:" + formatAngka(hasil));
    }
    catch (exception &e) {
        return kirim(string("ERR:") + e.what());
    }
}
EMSCRIPTEN_KEEPALIVE
const char* konversi(int kategori, int asal, int tujuan,
                     double nilai, int mode) {
    double hasil;

    if (!isfinite(nilai))
        return kirim("ERR:Nilai tidak valid!");

    if (kategori == 3) {
        if (mode < 1 || mode > 6)
            return kirim("ERR:Pilihan tidak tersedia!");

        if (((mode == 1 || mode == 3) && nilai < -273.15) ||
            ((mode == 2 || mode == 5) && nilai < -459.67) ||
            ((mode == 4 || mode == 6) && nilai < 0))
            return kirim("ERR:Suhu tidak valid!");

        switch (mode) {
            case 1: hasil = nilai * 9 / 5 + 32; break;
            case 2: hasil = (nilai - 32) * 5 / 9; break;
            case 3: hasil = nilai + 273.15; break;
            case 4: hasil = nilai - 273.15; break;
            case 5: hasil = (nilai - 32) * 5 / 9 + 273.15; break;
            case 6: hasil = (nilai - 273.15) * 9 / 5 + 32; break;
        }

        ans = hasil;
        history.push_back("Konversi satuan = " + formatAngka(hasil));

        return kirim("OK:" + formatAngka(hasil));
    }

    if (kategori < 0 || kategori > 2)
        return kirim("ERR:Kategori tidak tersedia!");

    if (asal < 0 || asal > 6 || tujuan < 0 || tujuan > 6 ||
        SATUAN[kategori][asal] == "" ||
        SATUAN[kategori][tujuan] == "")
        return kirim("ERR:Satuan tidak tersedia!");

    hasil = nilai * FAKTOR[kategori][asal]
            / FAKTOR[kategori][tujuan];

    ans = hasil;
    history.push_back("Konversi satuan = " + formatAngka(hasil));

    return kirim("OK:" + formatAngka(hasil) + " " + SATUAN[kategori][tujuan]);
}

EMSCRIPTEN_KEEPALIVE
double get_ans() { return ans; }

EMSCRIPTEN_KEEPALIVE
void reset_ans() { ans = 0; }

EMSCRIPTEN_KEEPALIVE
const char* satuan_nama(int kategori, int i) {
    if (kategori < 0 || kategori > 2 || i < 0 || i > 6)
        return "";
    return SATUAN[kategori][i].c_str();
}

EMSCRIPTEN_KEEPALIVE
int riwayat_jumlah() { return (int)history.size(); }

EMSCRIPTEN_KEEPALIVE
const char* riwayat_ambil(int i) {
    if (i < 0 || i >= (int)history.size())
        return "";
    return kirim(history[i]);
}

EMSCRIPTEN_KEEPALIVE
void hapus_riwayat() { history.clear(); }

}
