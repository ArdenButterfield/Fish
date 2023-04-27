//
//  FilterCalc.cpp
//
//  Created by lsd5k on 2/21/15.
//
//

#include "FilterCalc.h"
#include <cmath>
#include <iostream>

// Calculates the filter coefficients for a 1-pole low-pass filter.
// fc = -3dB freq
// fs = sampling rate
// coeffs = [b0 a1]

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void FilterCalc::calcCoeffs1PoleLPF(float* coeffs, float fc, float fs)
{
    double wc, b, b0, a1;
    
    // fc cannot be zero. let's limit it to 10Hz to fs/2
    if (fc < 10){
        fc = 10;
    }
    else if (fc > fs/2) {
        fc = fs/2;
    }
    
    wc = 2*myPI*fc/fs;    // the -3dB freq in rad/sec
    b = 4 -2*cos(wc);
    a1 =0.5*(sqrt(b*b - 4) - b);
    b0 = 1 + a1;
    
    coeffs[0] = b0;
    coeffs[1] = a1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Calculates the filter coefficients for a 2nd-order peak filter.
// coeffs = [b0, b1, b2, a1, a2]
// fc     = center frequency in Hz
// gainDB = the amount of peak gain or cut, in dB
// Q      = filter Q, proportional to fc/bandwidth
// fs     = sampling rate in Hz

void FilterCalc::calcCoeffsPeak(float* coeffs, float fc, float gainDb, float Q, float fs)
{
    // Based on Zolzer, DASP, Chapter 5
    double K, V, D;
    float b0, b1, b2, a1, a2;
    
    // let's limit fc to 10Hz to fs/2
    if (fc < 10){
        fc = 10;
    }
    else if (fc > fs/2) {
        fc = fs/2;
    }
    
    K = tan(myPI*fc/fs);
    
    if (gainDb >= 0) {
        // boost or peak
        V = powf(10, gainDb/20);
        b0 = 1 + V*K/Q + K*K;
        b1 = 2*(K*K - 1);
        b2 = 1 - V*K/Q + K*K;
        D = 1 + K/Q + K*K;
        a1 = 2*(K*K - 1);
        a2 = 1 - K/Q + K*K;
    }
    else {
        // cut or notch
        V = pow(10, -gainDb/20);
        b0 = 1 + K/Q + K*K;
        b1 = 2*(K*K - 1);
        b2 = 1 - K/Q + K*K;
        D = 1 + V*K/Q + K*K;
        a1 = 2*(K*K - 1);
        a2 = 1 - V*K/Q + K*K;
    }
    // normalize by D
    b0 = b0 / D;
    b1 = b1 / D;
    b2 = b2 / D;
    a1 = a1 / D;
    a2 = a2 / D;
    
    coeffs[0] = b0;
    coeffs[1] = b1;
    coeffs[2] = b2;
    coeffs[3] = a1;
    coeffs[4] = a2;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Calculates the filter coefficients for a 2nd-order low-shelf filter.
// coeffs = [b0, b1, b2, a1, a2]
// fc     = transition frequency in Hz
// gainDb = the amount of peak gain or cut, in dB
// fs     = sampling rate in Hz

void FilterCalc::calcCoeffsLowShelf(float* coeffs, float fc, float gainDb, float fs)
{
    // Based on Zolzer, DASP, Chapter 5
    double K, V, D;
    float b0, b1, b2, a1, a2;
    
    // let's limit fc to 10Hz to fs/2
    if (fc < 10){
        fc = 10;
    }
    else if (fc > fs/2) {
        fc = fs/2;
    }
    
    K = tan(myPI*fc/fs);
    
    if (gainDb >= 0) {
        // boost low freqs
        V = pow(10, gainDb/20);
        b0 = 1 + sqrt(2*V)*K + V*K*K;
        b1 = 2*(V*K*K - 1);
        b2 = 1 - sqrt(2*V)*K + V*K*K;
        D = 1 + sqrt(2)*K + K*K;
        a1 = 2*(K*K - 1);
        a2 = 1 - sqrt(2)*K + K*K;
    }
    else {
        // cut low freqs
        V = pow(10, -gainDb/20);
        b0 = 1 + sqrt(2)*K + K*K;
        b1 = 2*(K*K - 1);
        b2 = 1 - sqrt(2)*K + K*K;
        D = 1 + sqrt(2*V)*K + V*K*K;
        a1 = 2*(V*K*K - 1);
        a2 = 1 - sqrt(2*V)*K + V*K*K;
    }
    // normalize by D
    b0 = b0 / D;
    b1 = b1 / D;
    b2 = b2 / D;
    a1 = a1 / D;
    a2 = a2 / D;
    
    coeffs[0] = b0;
    coeffs[1] = b1;
    coeffs[2] = b2;
    coeffs[3] = a1;
    coeffs[4] = a2;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Calculates the filter coefficients for a 2nd-order high-shelf filter.
// coeffs = [b0, b1, b2, a1, a2]
// fc     = transition frequency in Hz
// gainDb = the amount of peak gain or cut, in dB
// fs     = sampling rate in Hz

void FilterCalc::calcCoeffsHighShelf(float* coeffs, float fc, float gainDb, float fs)
{
    // Based on Zolzer, DASP, Chapter 5
    double K, V, D1, D2;
    float b0, b1, b2, a1, a2;
    
    // let's limit fc to 10Hz to fs/2
    if (fc < 10){
        fc = 10;
    }
    else if (fc > fs/2) {
        fc = fs/2;
    }
    
    K = tan(myPI*fc/fs);
    
    if (gainDb >= 0) {
        // boost high freqs
        V = pow(10, gainDb/20);
        b0 = V + sqrt(2*V)*K + K*K;
        b1 = 2*(K*K - V);
        b2 = V - sqrt(2*V)*K + K*K;
        D1 = 1 + sqrt(2)*K + K*K;
        a1 = 2*(K*K - 1);
        a2 = 1 - sqrt(2)*K + K*K;
        
        //normalize
        b0 = b0 / D1;
        b1 = b1 / D1;
        b2 = b2 / D1;
        a1 = a1 / D1;
        a2 = a2 / D1;
    }
    else {
        // cut high freqs
        V = pow(10, -gainDb/20);
        b0 = 1 + sqrt(2)*K + K*K;
        b1 = 2*(K*K - 1);
        b2 = 1 - sqrt(2)*K + K*K;
        D1 = V + sqrt(2*V)*K + K*K;
        D2 = 1 + sqrt(2/V)*K + K*K/V;
        a1 = 2*(K*K/V - 1);
        a2 = 1 - sqrt(2/V)*K + K*K/V;
        
        // normalize
        b0 = b0 / D1;
        b1 = b1 / D1;
        b2 = b2 / D1;
        a1 = a1 / D2;
        a2 = a2 / D2;
    }
    
    coeffs[0] = b0;
    coeffs[1] = b1;
    coeffs[2] = b2;
    coeffs[3] = a1;
    coeffs[4] = a2;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Calculates the filter coefficients for a 2nd-order low-pass filter.
// coeffs = [b0, b1, b2, a1, a2]
// fc     = transition frequency in Hz
// Q      = filter Q, proportional to fc/bandwidth
// fs     = sampling rate in Hz

void FilterCalc::calcCoeffsLPF(float* coeffs, float fc, float Q, float fs)
{
    float K, Ksq, D, b0, b1, b2, a1, a2;
    
    K = tan(myPI*fc/fs);
    Ksq = K*K;
    
    D = Ksq*Q + K +Q;
    b0 = Ksq*Q;
    b1 = 2*b0;
    a1 = 2*Q*(Ksq-1);
    a2 = Ksq*Q - K + Q;
    
    b0 = b0 / D;
    b1 = b1 / D;
    b2 = b0;
    a1 = a1 / D;
    a2 = a2 / D;
    
    coeffs[0] = b0;
    coeffs[1] = b1;
    coeffs[2] = b2;
    coeffs[3] = a1;
    coeffs[4] = a2;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Calculates the filter coefficients for a 2nd-order high-pass filter.
// coeffs = [b0, b1, b2, a1, a2]
// fc     = transition frequency in Hz
// Q      = filter Q, proportional to fc/bandwidth
// fs     = sampling rate in Hz

void FilterCalc::calcCoeffsHPF(float* coeffs, float fc, float Q, float fs)
{
    float K, Ksq, D, b0, b1, b2, a1, a2;
    
    K = tan(myPI*fc/fs);
    Ksq = K*K;
    
    D = Ksq*Q + K +Q;
    b0 = Q;
    b1 = -2*b0;
    a1 = 2*Q*(Ksq-1);
    a2 = Ksq*Q - K + Q;
    
    b0 = b0 / D;
    b1 = b1 / D;
    b2 = b0;
    a1 = a1 / D;
    a2 = a2 / D;
    
    coeffs[0] = b0;
    coeffs[1] = b1;
    coeffs[2] = b2;
    coeffs[3] = a1;
    coeffs[4] = a2;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Calculates the filter coefficients for a 2nd-order band-pass filter.
// coeffs = [b0, b1, b2, a1, a2]
// fc     = transition frequency in Hz
// Q      = filter Q, proportional to fc/bandwidth
// fs     = sampling rate in Hz

void FilterCalc::calcCoeffsBPF(float* coeffs, float fc, float Q, float fs)
{
    float K, Ksq, D, b0, b1, b2, a1, a2;
    
    K = tan(myPI*fc/fs);
    Ksq = K*K;
    
    D = Ksq*Q + K +Q;
    b0 = K;
    a1 = 2*Q*(Ksq-1);
    a2 = Ksq*Q - K + Q;
    
    b0 = b0 / D;
    b1 = 0;
    b2 = -b0;
    a1 = a1 / D;
    a2 = a2 / D;
    
    coeffs[0] = b0;
    coeffs[1] = b1;
    coeffs[2] = b2;
    coeffs[3] = a1;
    coeffs[4] = a2;
}
















