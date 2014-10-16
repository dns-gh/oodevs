// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __FrameCounter_h_
#define __FrameCounter_h_

#include <boost/noncopyable.hpp>

class QGLWidget;

namespace gui
{

class FrameCounter : boost::noncopyable
{
public:
    FrameCounter( QGLWidget& widget, int x, int y );

    void Update();

private:
    void Compute();
    void Draw() const;

private:
    QGLWidget& widget_;
    int x_;
    int y_;
    QTime time_;
    int frames_;
    int fps_;
};

}

#endif // __FrameCounter_h_
