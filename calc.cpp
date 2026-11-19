#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <cctype>
using namespace std;

static double ans = 0;
static string out; 

static string fmt(double v) {
    ostringstream ss;
    ss << fixed << setprecision(2) << v;
    return ss.str();
}

class Parser {
    string s;
    size_t pos = 0;
    void skip() { while (pos < s.size() && isspace((unsigned char)s[pos])) pos++; }
    bool match(char c) {
        skip();
        if (pos < s.size() && s[pos] == c) { pos++; return true; }
        return false;
    }
    double number() {
        skip();
        if (s.compare(pos, 3, "Ans") == 0 || s.compare(pos, 3, "ans") == 0) { pos += 3; return ans; }
        if (s.compare(pos, 3, "√") == 0) {
            pos += 3;
            double x = factor();
            if (x < 0) throw runtime_error("Akar negatif tidak valid!");
            return sqrt(x);
        }
        if (match('(')) {
            double x = expression();
            if (!match(')')) throw runtime_error("Kurung tidak lengkap!");
            return x;
        }
        size_t start = pos;
        while (pos < s.size() && (isdigit((unsigned char)s[pos]) || s[pos] == '.')) pos++;
        if (start == pos) throw runtime_error("Input tidak valid!");
        try { return stod(s.substr(start, pos - start)); }
        catch (...) { throw runtime_error("Angka tidak valid!"); }
    }
    double factor() {
        if (match('+')) return factor();
        if (match('-')) return -factor();
        double x = number();
        if (match('^')) x = pow(x, factor());
        return x;
    }
    double term() {
        double x = factor();
        while (true) {
            if (match('*')) x *= factor();
            else if (match('/')) {
                double y = factor();
                if (y == 0) throw runtime_error("Tidak bisa dibagi nol!");
                x /= y;
            } else break;
        }
        return x;
    }
    double expression() {
        double x = term();
        while (true) {
            if (match('+')) x += term();
            else if (match('-')) x -= term();
            else break;
        }
        return x;
    }
public:
    Parser(string input) {
        s = input;
        size_t p;
        while ((p = s.find("×")) != string::npos) s.replace(p, 2, "*");
        while ((p = s.find("÷")) != string::npos) s.replace(p, 2, "/");
        while ((p = s.find("−")) != string::npos) s.replace(p, 3, "-");
    }
    double calculate() {
        double hasil = expression();
        skip();
        if (pos != s.size()) throw runtime_error("Format perhitungan salah!");
        if (!isfinite(hasil)) throw runtime_error("Hasil tidak valid!");
        return hasil;
    }
};

static const char* SATUAN[3][7] = {
    {"km", "hm", "dam", "m", "dm", "cm", "mm"},
    {"ton", "kuintal", "kg", "hg", "dag", "g", "mg"},
    {"hari", "jam", "menit", "detik", "ms", "", ""}
};
static const double FAKTOR[3][7] = {
    {1000000, 100000, 10000, 1000, 100, 10, 1},
    {1000000, 100000, 1000, 100, 10, 1, 0.001},
    {86400, 3600, 60, 1, 0.001, 0, 0}
};

extern "C" {

const char* hitung(const char* expr) {
    try {
        Parser p(expr);
        double h = p.calculate();
        ans = h;
        out = "OK:" + fmt(h);
    } catch (exception& e) {
        out = string("ERR:") + e.what();
    }
    return out.c_str();
}

double get_ans() { return ans; }
void reset_ans() { ans = 0; }

const char* konversi(int kategori, int asal, int tujuan, double nilai, int mode) {
    double hasil = 0;
    string unit;
    if (!isfinite(nilai)) { out = "ERR:Nilai tidak valid!"; return out.c_str(); }
    if (kategori == 3) {
        if (mode < 1 || mode > 6) { out = "ERR:Pilihan tidak tersedia!"; return out.c_str(); }
        if (((mode == 1 || mode == 3) && nilai < -273.15) ||
            ((mode == 2 || mode == 5) && nilai < -459.67) ||
            ((mode == 4 || mode == 6) && nilai < 0)) {
            out = "ERR:Suhu tidak valid!"; return out.c_str();
        }
        switch (mode) {
            case 1: hasil = nilai * 9 / 5 + 32; unit = "°F"; break;
            case 2: hasil = (nilai - 32) * 5 / 9; unit = "°C"; break;
            case 3: hasil = nilai + 273.15; unit = "K"; break;
            case 4: hasil = nilai - 273.15; unit = "°C"; break;
            case 5: hasil = (nilai - 32) * 5 / 9 + 273.15; unit = "K"; break;
            case 6: hasil = (nilai - 273.15) * 9 / 5 + 32; unit = "°F"; break;
        }
    } else if (kategori >= 0 && kategori < 3) {
        if (asal < 0 || asal > 6 || tujuan < 0 || tujuan > 6 ||
            SATUAN[kategori][asal][0] == 0 || SATUAN[kategori][tujuan][0] == 0) {
            out = "ERR:Satuan tidak tersedia!"; return out.c_str();
        }
        hasil = nilai * FAKTOR[kategori][asal] / FAKTOR[kategori][tujuan];
        unit = SATUAN[kategori][tujuan];
    } else {
        out = "ERR:Kategori tidak tersedia!"; return out.c_str();
    }
    ans = hasil;
    out = "OK:" + fmt(hasil) + " " + unit;
    return out.c_str();
}

const char* satuan_nama(int kategori, int i) {
    if (kategori < 0 || kategori > 2 || i < 0 || i > 6) return "";
    return SATUAN[kategori][i];
}

}
