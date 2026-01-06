#include "fractal.ispc.h"
#include <complex>
#include <iostream>
#include <ostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Color {
    unsigned char r, g, b;
};

Color hsvToRgb(double h, double s, double v)
{
    double r = 0, g = 0, b = 0;
    if (s == 0.0) {
        r = g = b = v;
    } else {
        h /= 60.0;
        int i = floor(h);
        double f = h - i;
        double p = v * (1.0 - s);
        double q = v * (1.0 - s * f);
        double t = v * (1.0 - s * (1.0 - f));
        switch (i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:
            r = v;
            g = p;
            b = q;
            break;
        }
    }
    return {
        static_cast<unsigned char>(r * 255),
        static_cast<unsigned char>(g * 255),
        static_cast<unsigned char>(b * 255)
    };
}

int main()
{
    const double TOLERANCE = 0.000001;
    const int MAX_ITERATIONS = 100;

    int size;
    std::cout << "Enter the size of image (e.g., 512, 1024, 4096): ";
    std::cin >> size;

    if (size < 256) {
        std::cerr << "Error: Size must be 256 or greater." << std::endl;
        return 1;
    }

    double range;
    std::cout << "Enter the coordinate range [-R, R] for the grid (e.g., 5.0): ";
    std::cin >> range;

    if (range <= 0) {
        std::cerr << "Error: Range must be a positive number." << std::endl;
        return 1;
    }

    int n;
    std::cout << "Enter the power N for z^N = 1 (e.g., 3, 4, 5...): ";
    std::cin >> n;

    if (n < 2) {
        std::cerr << "Error: N must be 2 or greater." << std::endl;
        return 1;
    }

    std::vector<double> root_hues;
    std::vector<double> roots_real;
    std::vector<double> roots_imag;

    for (int i = 0; i < n; ++i) {
        double angle = 2.0 * M_PI / n * i;
        double hue = 360.0 / n * i;

        std::complex<double> root = std::polar(1.0, angle);

        root_hues.push_back(hue);
        roots_real.push_back(root.real());
        roots_imag.push_back(root.imag());
    }

    std::vector<int> pixel_roots(size * size, -1);
    std::vector<int> pixel_iterations(size * size, 0);

    ispc::calculate_fractal(
        size, range, n, TOLERANCE, MAX_ITERATIONS,
        roots_real.data(), roots_imag.data(),
        pixel_roots.data(), pixel_iterations.data());

    int most_iterations = 0;
    for (int i = 0; i < size * size; ++i) {
        if (pixel_iterations[i] > most_iterations) {
            most_iterations = pixel_iterations[i];
        }
    }

    std::vector<unsigned char> pixels(size * size * 3);

    for (int i = 0; i < size * size; ++i) {
        Color color = { 0, 0, 0 };
        if (pixel_roots[i] != -1) {
            double hue = root_hues[pixel_roots[i]];
            double brightness = 1 - (std::log(pixel_iterations[i]) / std::log(most_iterations));
            color = hsvToRgb(hue, 1.0, brightness);
        }
        int pixel_index = i * 3;
        pixels[pixel_index] = color.r;
        pixels[pixel_index + 1] = color.g;
        pixels[pixel_index + 2] = color.b;
    }

    const char* filename = "out/fractal.png";
    if (stbi_write_png(filename, size, size, 3, pixels.data(), size * 3)) {
        std::cout << "Image '" << filename << "' generated successfully." << std::endl;
    } else {
        std::cerr << "Failed to write image." << std::endl;
        return 1;
    }

    return 0;
}
