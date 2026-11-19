# Kalkulator-C-
DEPLOY KE GITHUB PAGES (otomatis, tanpa emcc di perangkat sendiri)

1. Buat repo baru di GitHub, upload isi folder ini ke root repo:
   index.html, css/, js/, kalk.cpp, web.cpp, .github/workflows/deploy.yml
2. Repo > Settings > Pages > Build and deployment > Source: GitHub Actions
3. Push ke branch main. GitHub akan compile web.cpp (yang meng-include kalk.cpp) jadi calc.js lalu publish.
   Cek progres di tab Actions. Alamat: https://USERNAME.github.io/NAMA-REPO/

BUILD MANUAL (butuh Emscripten):
emcc web.cpp -O2 -Dmain=main_terminal -o calc.js -sEXPORTED_RUNTIME_METHODS=cwrap -sEXPORTED_FUNCTIONS=_hitung,_konversi,_get_ans,_reset_ans,_satuan_nama,_riwayat_jumlah,_riwayat_ambil,_hapus_riwayat -sSINGLE_FILE=1
Lalu taruh calc.js sejajar index.html.

VERSI TERMINAL:
g++ -std=c++17 kalk.cpp -o kalk && ./kalk
