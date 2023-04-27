
//  FilterCalc.h
//  Created by lsd5k on 2/21/15.

#ifndef __filters__
#define __filters__

#include <stdio.h>

class FilterCalc
{
public:
    
    constexpr static float myPI = 3.1415926535897932384626433832795; //NASA only needs 15 decimal places of Pi for their calculations
    
    // Calculates the filter coefficients for a 1-pole low-pass filter.
    // coeffs = [b0 a1]
    // fc = -3dB freq
    // fs = sampling rate
    static void calcCoeffs1PoleLPF(float* coeffs, float fc, float fs);
    
    // Calculates the filter coefficients for a 2nd-order peak filter.
    // coeffs = [b0, b1, b2, a1, a2]
    // fc     = center frequency in Hz
    // gainDB = the amount of peak gain or cut, in dB
    // Q      = filter Q, proportional to fc/bandwidth
    // fs     = sampling rate in Hz;
    static void calcCoeffsPeak(float* coeffs, float fc, float gainDb, float Q, float fs);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Calculates the filter coefficients for a 2nd-order low-shelf filter.
    // coeffs = [b0, b1, b2, a1, a2]
    // fc     = transition frequency in Hz
    // gainDb = the amount of peak gain or cut, in dB
    // fs     = sampling rate in Hz
    static void calcCoeffsLowShelf(float* coeffs, float fc, float gainDb, float fs);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Calculates the filter coefficients for a 2nd-order high-shelf filter.
    // coeffs = [b0, b1, b2, a1, a2]
    // fc     = transition frequency in Hz
    // gainDb = the amount of peak gain or cut, in dB
    // fs     = sampling rate in Hz
    static void calcCoeffsHighShelf(float* coeffs, float fc, float gainDb, float fs);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Calculates the filter coefficients for a 2nd-order low-pass filter.
    // coeffs = [b0, b1, b2, a1, a2]
    // fc     = transition frequency in Hz
    // Q      = filter Q, proportional to fc/bandwidth
    // fs     = sampling rate in Hz
    static void calcCoeffsLPF(float* coeffs, float fc, float Q, float fs);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Calculates the filter coefficients for a 2nd-order high-pass filter.
    // coeffs = [b0, b1, b2, a1, a2]
    // fc     = transition frequency in Hz
    // Q      = filter Q, proportional to fc/bandwidth
    // fs     = sampling rate in Hz
    static void calcCoeffsHPF(float* coeffs, float fc, float Q, float fs);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Calculates the filter coefficients for a 2nd-order band-pass filter.
    // coeffs = [b0, b1, b2, a1, a2]
    // fc     = transition frequency in Hz
    // Q      = filter Q, proportional to fc/bandwidth
    // fs     = sampling rate in Hz
    static void calcCoeffsBPF(float* coeffs, float fc, float Q, float fs);
    
};

#endif /* defined(__filters__) */
