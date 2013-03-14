// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ImageWrapper_h_
#define __ImageWrapper_h_

#include <QtGui/qpixmap.h>
#include <QtGui/qimage.h>

namespace tools
{
    class Path;
}

namespace gui
{
    // -----------------------------------------------------------------------------
    // QPixmap
    // -----------------------------------------------------------------------------
    class Pixmap : public QPixmap
    {
    public:
                 Pixmap( const tools::Path& file );
        virtual ~Pixmap();
    };

    // -----------------------------------------------------------------------------
    // QImage
    // -----------------------------------------------------------------------------
    class Image : public QImage
    {
    public:
                 Image( const tools::Path& file );
        virtual ~Image();
    };

    // -----------------------------------------------------------------------------
    // QIcon
    // -----------------------------------------------------------------------------
    class Icon : public QIcon
    {
    public:
                 Icon( const tools::Path& file );
        virtual ~Icon();
    };
}

#endif // __ImageWrapper_h_
