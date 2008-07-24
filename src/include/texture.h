/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2008 Larry Gritz.
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// (This is the MIT open source license.)
/////////////////////////////////////////////////////////////////////////////


#ifndef TEXTURE_H
#define TEXTURE_H

#include "varyingref.h"
#include "ustring.h"


typedef unsigned char Runflag;



class TextureOptions {
public:
    enum Wrap {
        WrapDefault,        ///< Use the default found in the file
        WrapBlack,          ///< Black outside [0..1]
        WrapClamp,          ///< Clamp to [0..1]
        WrapPeriodic,       ///< Periodic mod 1
        WrapMirror,         ///< Mirror the image
        WrapLast            ///< Mark the end
    };

    TextureOptions () { }

    // Options that must be the same for all points we're texturing at once
    int firstchannel;
    int nchannels;
    Wrap swrap;
    Wrap twrap;

    // Options that may be different for each point we're texturing
    VaryingRef<ustring> filename;
    VaryingRef<float>   sblur, tblur;
    VaryingRef<float>   swidth, twidth;
    VaryingRef<float>   bias;
    VaryingRef<float>   fill;

    // Storage for results
    VaryingRef<float>   alpha;

private:
};



class TextureSystem {
public:
    static TextureSystem *create ();
    static void destroy (TextureSystem * &x);

    TextureSystem (void) { }
    virtual ~TextureSystem () { }

    // Set options
    virtual void max_open_files (int nfiles) = 0;
    virtual void max_memory_MB (float size) = 0;
    virtual void searchpath (const ustring &path) = 0;
    
    // Retrieve options
    virtual int max_open_files () const = 0;
    virtual float max_memory_MB () const = 0;
    virtual ustring searchpath () const = 0;

    /// Retrieve filtered texture lookups for several points.
    virtual void texture (ustring filename, TextureOptions &options,
                          Runflag *runflags, int firstactive, int lastactive,
                          VaryingRef<float> s, VaryingRef<float> t,
                          VaryingRef<float> dsdx, VaryingRef<float> dtdx,
                          VaryingRef<float> dsdy, VaryingRef<float> dtdy,
                          float *result, int threadid=-1) = 0;
};


#endif // TEXTURE_H