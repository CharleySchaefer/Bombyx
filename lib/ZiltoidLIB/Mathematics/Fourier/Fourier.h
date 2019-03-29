#ifndef FOURIER_H
  #define FOURIER_H  1

  #include <complex.h>
  #include <math.h>
  #ifndef M_PI
    #define M_PI 3.14159265358979323846264338327950288419716939937510582097494459230781
  #endif

  // Discrete Fourier transform
  void  dft(    complex *, complex *, int);
  void  dft2( complex **, complex *, int,  int);

  // Fast Fourier transform (1D) Adapted from RosettaCodeData https://github.com/acmeism/RosettaCodeData/tree/948b86eafab0e034330a3b6c31617370c6cca2fc/Task/Fast-Fourier-transform/C
  void  fft(    complex *, complex *, int);
  void  fft_emb(complex *, complex *, int, int);
  void ifft(    complex *, complex *, int);
  void ifft_emb(complex *, complex *, int, int);

  // Fast Fourier transform (2D) - uses 1D version
  void  fft2( complex **, complex *, int, int);
  void ifft2( complex **, complex *, int, int);

  // Fast Fourier transform (3D) - uses 1D version
  void  fft3(complex ***, complex *, int, int, int);
  void ifft3(complex ***, complex *, int, int, int);
#endif

