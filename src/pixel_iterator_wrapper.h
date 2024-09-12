#pragma once

#include "FastLED.h"
#include "pixel_controller.h"
#include "pixel_iterator.h"

FASTLED_NAMESPACE_BEGIN

template <typename PixelControllerT>
class PixelIteratorT : protected PixelIterator {
    // New chipsets/drivers should use as_iterator() to process led output.
    // Accessing PixelController directly from user code deprecated, and should
    // be minimized.
    //
    // Create an adapter class to allow PixelController to be used as a
    // PixelIterator. Caller must save the fully templatized object to the
    // stack, then call base() in order to get the non-templatized base class
    // reference, which is then used to call the get the ordering of pixels.
    // PixelIterator should be used for more powerful micro controllers which
    // can easily handle the virtual function overhead.

  public:
    static const EOrder RGB_ORDER = PixelControllerT::RGB_ORDER_VALUE;

    PixelIteratorT(PixelControllerT &pc, Rgbw rgbw) : mPixelController(pc) {
        set_rgbw(rgbw);
    }
    virtual bool has(int n) { return mPixelController.has(n); }
    virtual void loadAndScaleRGBW(uint8_t *b0_out, uint8_t *b1_out,
                                  uint8_t *b2_out, uint8_t *b3_out) {
        Rgbw rgbw = get_rgbw();
        mPixelController.loadAndScaleRGBW(rgbw, b0_out, b1_out, b2_out, b3_out);
    }
    virtual void loadAndScaleRGB(uint8_t *b0, uint8_t *b1, uint8_t *b2) {
        *b0 = mPixelController.loadAndScale0();
        *b1 = mPixelController.loadAndScale1();
        *b2 = mPixelController.loadAndScale2();
    }
    virtual void loadAndScale_APA102_HD(uint8_t *b0_out, uint8_t *b1_out,
                                        uint8_t *b2_out,
                                        uint8_t *brightness_out) {
        mPixelController.loadAndScale_APA102_HD(b0_out, b1_out, b2_out,
                                                brightness_out);
    }
    virtual void stepDithering() { mPixelController.stepDithering(); }
    virtual void advanceData() { mPixelController.advanceData(); }
    virtual int size() { return mPixelController.size(); }

    // Get the base class reference for the PixelIterator, which is used by the
    // driver system.
    PixelIterator &base() { return *this; }

    PixelControllerT &mPixelController;
};

FASTLED_NAMESPACE_END