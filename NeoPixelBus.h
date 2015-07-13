/*--------------------------------------------------------------------
This file is a modification of the Adafruit NeoPixel library.

NeoPixel is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixel is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/
#pragma once

#include <Arduino.h>

enum ColorType
{
    ColorType_Rgb,
    ColorType_Hsl
};

#include "RgbColor.h"
#include "HslColor.h"
#include "NeoPixelAnimator.h"
#include "NeoPixelMultiBus.h"

extern "C"
{
#include "NeoPixelEsp8266.h"
}

// '_flagsPixels' flags for LED _pixels (third parameter to constructor):
#define NEO_RGB     0x01 // Wired for RGB data order
#define NEO_GRB     0x02 // Wired for GRB data order
#define NEO_BRG     0x04
#define NEO_COLMASK 0x07

#define NEO_KHZ400  0x01 // 400 KHz datastream
#define NEO_KHZ800  0x02 // 800 KHz datastream
#define NEO_SPDMASK 0x03

#define NEO_DIRTY   0x80 // a change was made it _pixels that requires a show

// v1 NeoPixels aren't handled by default, include the following define before the 
// NeoPixelBus library include to support the slower bus speeds
//#define INCLUDE_NEO_KHZ400_SUPPORT 

class NeoPixelBus
{
public:
    // Constructor: number of LEDs, pin number, LED type
    NeoPixelBus();
    NeoPixelBus(uint16_t n, uint8_t p, uint8_t t = NEO_GRB | NEO_KHZ800);
    ~NeoPixelBus();

    inline uint16_t getPixelCount()
    {
        return _countPixels;
    }

    void SetPin(uint8_t p);

    uint8_t Pin() const
    {
        return _pin;
    }

    void Begin(uint16_t n, uint8_t p, uint8_t t = NEO_GRB | NEO_KHZ800);
    void Begin();

    void Show();

    inline bool CanShow(void) const
    { 
        return (micros() - _endTime) >= 50L; 
    }

    void ClearTo(uint8_t r, uint8_t g, uint8_t b);

    void ClearTo(RgbColor c)
    {
        ClearTo(c.R, c.G, c.B);
    }

    bool IsDirty() const
    {
        return  (_flagsPixels & NEO_DIRTY);
    };

    void Dirty()
    {
        _flagsPixels |= NEO_DIRTY;
    };

    void ResetDirty()
    {
        _flagsPixels &= ~NEO_DIRTY;
    }

    uint8_t* Pixels() const
    {
        return _pixels;
    };

    uint16_t PixelCount() const
    {
        return _countPixels;
    };

    uint16_t PixelSize() const
    {
        return _sizePixels;
    }

    void SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

    void SetPixelColor(uint16_t n, RgbColor c)
    {
        SetPixelColor(n, c.R, c.G, c.B);
    };

    RgbColor GetPixelColor(uint16_t n) const;

private:
    friend NeoPixelAnimator;

    
    void updatePixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void updatePixelColor(uint16_t n, RgbColor c)
    {
        updatePixelColor(n, c.R, c.G, c.B);
    };

    void initMemory();
    void deinitMemory();

    uint32_t _endTime;       // Latch timing reference
    uint16_t    _countPixels;       // Number of RGB LEDs in strip
    uint16_t    _sizePixels;      // Size of '_pixels' buffer below
    
    uint8_t _flagsPixels;          // Pixel flags (400 vs 800 KHz, RGB vs GRB color)
    uint8_t _pin;           // Output pin number
    uint8_t* _pixels;        // Holds LED color values (3 bytes each)
    

#ifdef __AVR__
    const volatile uint8_t* _port;         // Output PORT register
    uint8_t _pinMask;       // Output PORT bitmask
#endif


};

