#include <unordered_map>
#include <emscripten/emscripten.h>
// #define EMSCRIPTEN_KEEPALIVE 
// #include <iostream>

const int GRADIENT_1 = 0xFFF33B;
const int GRADIENT_0 = 0xEB4034;

class QuadraticAttractor {
private:
    double x = 0., y = 0., z = 1;
    double xn = 0., yn = 0.;
    double ax, ay;
    double a[12];
    int w, h;
    std::unordered_map<int, int> map;

public:
    QuadraticAttractor() { map = { }; }

    void set_consts(
        int width, int height, double zoom,
        float consts[], double adj_x, double adj_y
    ) {
        w = width;
        h = height;
        z = zoom;
        ax = adj_x;
        ay = adj_y;

        for (auto i = 0; i < 12; i++)
            a[i] = static_cast<double>(consts[i]);
    }

    int scr_x() const { 
        auto coord = static_cast<int> (z * (x + ax) + w / 2.);
        return coord;
    }

    int scr_y() const {
        auto coord = static_cast<int> (h / 2. - z * (y + ay));
        return coord;
    }

    void next() {
        x = a[0] + (a[1] * xn) + (a[2] * (xn * xn)) +
            (a[3] * xn * yn) + (a[4] * yn) + (a[5] * (yn * yn));

        y = a[6] + (a[7] * xn) + (a[8] * (xn * xn)) +
            (a[9] * xn * yn) + (a[10] * yn) + (a[11] * (yn * yn));

        xn = x;
        yn = y;

        auto index = scr_y() * h + scr_x();

        #if __cplusplus >= 201709
        if (map.contains(index))
        #elif
        if (map.find(index) != map.end())
        #endif
            map[index]++;
        else map[index] = 1;
    }

    int color(int a, int b) const {
        auto color = 0, max = -1;
        double alpha;

        #if __cplusplus >= 201709
        if (!map.contains(b * h + a))
        #elif
        if (map.find(b * h + a) == map.end())
        #endif
            return 0;

        for (const auto &[i, p] : map) if (p > max) max = p;

        if (max <= 0) return 0;

        alpha = static_cast<double>(map.at(b * h + a)) / max;

        color |= static_cast<int>(
            alpha * (GRADIENT_1 >> 16) + (1 - alpha) * (GRADIENT_0 >> 16)
        ) << 16;

        color |= static_cast<int>(
            alpha * ((GRADIENT_1 >> 8) & 0xFF) + (1 - alpha) * ((GRADIENT_0 >> 8) & 0xFF)
        ) << 8;

        color |= static_cast<int>(
            alpha * (GRADIENT_1 & 0xFF) + (1 - alpha) * (GRADIENT_0 & 0xFF)
        );

        return color;
    }
};

auto qa = QuadraticAttractor();

extern "C" {
    EMSCRIPTEN_KEEPALIVE \
    void init(int w, int h, double z, float a[], double c, double d) {
        qa.set_consts(w, h, z, a, c, d);
    }

    EMSCRIPTEN_KEEPALIVE \
    void next() { qa.next(); }

    EMSCRIPTEN_KEEPALIVE \
    int x() { return qa.scr_x(); }

    EMSCRIPTEN_KEEPALIVE \
    int y() { return qa.scr_y(); }

    EMSCRIPTEN_KEEPALIVE \
    int color(int x, int y) { return qa.color(x, y); }
}

int main() {
    // float a[12] = {
    //     -1,0.9,0.4,-0.2,-0.6,-0.5,0.4,0.7,0.3,-0.5,0.7,-0.8
    // };
    // init(3440,1440,600,a,.65,.45);

    // for (auto i = 0; i < 20; i++)
    //     next();

    // std::cout << x() << ' ' << y() << std::endl;

    // std::cout << "HI!" << '\n';
    return 0;
}