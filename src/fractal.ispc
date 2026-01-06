struct Complex {
    double real;
    double imag;
};

static inline Complex c_mul(Complex a, Complex b)
{
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

static inline Complex c_pow(Complex base, int power)
{
    Complex result = { 1.0, 0.0 };
    for (int i = 0; i < power; ++i) {
        result = c_mul(result, base);
    }
    return result;
}

export void calculate_fractal(
    uniform const int size,
    uniform const double range,
    uniform const int n,
    uniform const double tolerance,
    uniform const int max_iterations,
    uniform const double roots_real[],
    uniform const double roots_imag[],
    uniform int pixel_roots[],
    uniform int pixel_iterations[])
{
    foreach (y = 0 ... size, x = 0 ... size) {
        double real = (x - (size / 2.0)) / (size / 2.0 / range);
        double imag = ((size / 2.0) - y) / (size / 2.0 / range);

        Complex z = { real, imag };

        for (int iteration = 0; iteration < max_iterations; ++iteration) {
            Complex z_pow_n_minus_1 = c_pow(z, n - 1);
            Complex z_pow_n = c_mul(z_pow_n_minus_1, z);

            Complex f_z = { z_pow_n.real - 1.0, z_pow_n.imag };
            Complex df_z = { n * z_pow_n_minus_1.real, n * z_pow_n_minus_1.imag };

            double df_z_norm_sq = df_z.real * df_z.real + df_z.imag * df_z.imag;

            if (df_z_norm_sq > 1e-12) {
                Complex quotient;
                quotient.real = (f_z.real * df_z.real + f_z.imag * df_z.imag) / df_z_norm_sq;
                quotient.imag = (f_z.imag * df_z.real - f_z.real * df_z.imag) / df_z_norm_sq;
                z.real -= quotient.real;
                z.imag -= quotient.imag;
            }

            bool found = false;
            for (int i = 0; i < n; ++i) {
                double dr = z.real - roots_real[i];
                double di = z.imag - roots_imag[i];
                if (dr * dr + di * di < tolerance * tolerance) {
                    int index = y * size + x;
                    pixel_roots[index] = i;
                    pixel_iterations[index] = iteration + 1;
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
}
