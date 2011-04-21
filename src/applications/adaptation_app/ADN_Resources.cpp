// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources.h"

#include <qiconset.h>
#include <qpixmap.h>

#include <qimage.h>
#include <qdict.h>
#include <qmime.h>
#include <qdragobject.h>

// images\undo
static const unsigned char image_4_data[] = {
    0x00,0x00,0x07,0x90,0x78,0x9c,0x3b,0x7a,0xec,0x28,0xc3,0xd1,0x51,0x3c,
    0xac,0x71,0x33,0x03,0xc3,0x7f,0xac,0xb8,0xb1,0xf9,0x3f,0x3e,0x3d,0x24,
    0x9b,0x87,0x86,0xf1,0xe9,0xc5,0x67,0x1e,0x41,0x7b,0xb1,0xb8,0x1b,0x5d,
    0x3f,0x31,0xe6,0x11,0x72,0x17,0x3e,0xbf,0x92,0x1a,0xf6,0xc4,0x98,0x4b,
    0x4e,0x9c,0x0e,0xa4,0x7b,0x69,0x19,0xbe,0xc4,0x98,0x4d,0x4c,0x3a,0x26,
    0x29,0x4c,0x81,0xe6,0x90,0x1b,0x1f,0x44,0xb9,0x93,0x8a,0x66,0x8e,0xe2,
    0xe1,0x89,0x01,0xe0,0xa1,0x1f,0x27
};

// images\redo
static const unsigned char image_5_data[] = {
    0x00,0x00,0x07,0x90,0x78,0x9c,0x3b,0x7a,0xec,0x28,0xc3,0xd1,0x51,0x3c,
    0xe4,0x71,0x73,0x63,0xf3,0xff,0x66,0x06,0x06,0xac,0x98,0x6c,0x33,0x71,
    0x98,0x47,0xac,0xf9,0xd8,0xe4,0x90,0xdd,0x49,0xac,0xbd,0xd8,0xe4,0x70,
    0xe9,0x21,0xd5,0x5f,0xd8,0xf8,0xe4,0x9a,0x8b,0x2f,0xcc,0x28,0x35,0x17,
    0x97,0xd9,0x83,0xd1,0xbd,0xa4,0xa4,0x15,0x62,0xd2,0x03,0x31,0x69,0x91,
    0xa0,0x1e,0xa0,0x3d,0xe4,0xe6,0x03,0x62,0xd2,0xd0,0x28,0x1e,0xc5,0xd4,
    0xc0,0x00,0x7b,0xb7,0x1f,0x27
};

static struct EmbedImage {
    int width, height, depth;
    const unsigned char *data;
    ulong compressed;
    int numColors;
    const QRgb *colorTable;
    bool alpha;
    const char *name;
} embed_image_vec[] = {
    { 22, 22, 32, (const unsigned char*)image_4_data, 91, 0, 0, TRUE, "undo" },
    { 22, 22, 32, (const unsigned char*)image_5_data, 90, 0, 0, TRUE, "redo" },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

static QImage uic_findImage( const QString& name )
{
    for ( int i=0; embed_image_vec[i].data; i++ ) {
    if( QString::fromUtf8(embed_image_vec[i].name) == name ) {
        QByteArray baunzip;
        baunzip = qUncompress( embed_image_vec[i].data,
        embed_image_vec[i].compressed );
        QImage img((uchar*)baunzip.data(),
            embed_image_vec[i].width,
            embed_image_vec[i].height,
            embed_image_vec[i].depth,
            (QRgb*)embed_image_vec[i].colorTable,
            embed_image_vec[i].numColors,
            QImage::BigEndian
        );
        img = img.copy();
        if( embed_image_vec[i].alpha )
        img.setAlphaBuffer(TRUE);
        return img;
        }
    }
    return QImage();
}

class MimeSourceFactory_example : public QMimeSourceFactory
{
public:
    MimeSourceFactory_example() {}
    ~MimeSourceFactory_example() {}
    const QMimeSource* data( const QString& abs_name ) const {
    const QMimeSource* d = QMimeSourceFactory::data( abs_name );
    if( d || abs_name.isNull() ) return d;
    QImage img = uic_findImage( abs_name );
    if( !img.isNull() )
        ((QMimeSourceFactory*)this)->setImage( abs_name, img );
    return QMimeSourceFactory::data( abs_name );
    };
};

static QMimeSourceFactory* factory = 0;

void qInitImages_example()
{
    if( !factory ) {
    factory = new MimeSourceFactory_example;
    QMimeSourceFactory::defaultFactory()->addFactory( factory );
    }
}

void qCleanupImages_example()
{
    if( factory ) {
    QMimeSourceFactory::defaultFactory()->removeFactory( factory );
    delete factory;
    factory = 0;
    }
}

class StaticInitImages_example
{
public:
    StaticInitImages_example() { qInitImages_example(); }
#if defined(Q_OS_SCO) || defined(Q_OS_UNIXWARE)
    ~StaticInitImages_example() { }
#else
    ~StaticInitImages_example() { qCleanupImages_example(); }
#endif
};

static StaticInitImages_example staticImages;

// $$$$ CBX 01-06-26: Hack to encapsulate the xpm resources in a resources.cpp
// $$$$ CBX 01-06-26: instead of resources.h
#define static

#include "res/fileopen.xpm"
#include "res/filesave.xpm"
#include "res/filenew.xpm"
#include "res/testdata.xpm"
#include "res/application.xpm"
