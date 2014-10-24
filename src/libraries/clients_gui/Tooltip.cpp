// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Tooltip.h"
#include "TooltipsLayer_ABC.h"

using namespace gui;

Tooltip::Tooltip( const T_Messages& messages )
    : messages_( messages )
{
    // NOTHING
}

namespace
{
    bool IsMagenta( unsigned int i )
    {
        int delta = 0x000000FF - ( i & 0x000000FF );
        delta    += ( i >> 8 ) & 0x000000FF;
        delta    += 0x000000FF - ( ( i >> 16 ) & 0x000000FF );
        return delta < 256;
    }

    void RestoreAlpha( QImage& image )
    {
        if( image.isNull() )
            return;
        unsigned int* data = (unsigned int*)image.bits();
        for( int i = image.width() * image.height(); i > 0; --i )
        {
            if( IsMagenta( *data ) )
                *data |= 0xE000FF00;
            else
                *data |= 0xFF000000;
            ++data;
        }
    }

    template< typename Messages >
    QPixmap CreatePixmap( const Messages& messages )
    {
        int w = 0;
        int h = 0;
        int x = 0;
        for( auto it = messages.begin(); it != messages.end(); ++it )
        {
            const QFontMetrics metrics( it->font_ );
            const int fontHeight = metrics.height();
            const auto list = QStringList::split( '\n', it->message_, true );
            for( auto itLine = list.begin(); itLine != list.end(); ++itLine )
            {
                if( itLine == list.fromLast() )
                {
                    x += metrics.width( *itLine );
                    w = std::max( w, x );
                }
                else
                {
                    x = 0;
                    w = std::max( w, metrics.width( *itLine ) );
                    h += fontHeight + 2;
                }
            }
        }
        w = std::max< int >( w + 8, 0 );
        h = std::max< int >( h + 4, 0 );
        return QPixmap( w, h );
    }
}

QImage Tooltip::GenerateImage() const
{
    if( messages_.empty() )
        return QImage();
    QPixmap pixmap = CreatePixmap( messages_ );
    pixmap.fill( Qt::magenta );
    QPainter p;
    p.begin( &pixmap );
    QFontMetrics metrics( p.font() );
    int fontHeight = metrics.height();
    p.setPen( Qt::black );
    p.drawRect( pixmap.rect().adjusted( 0, 0, -1, -1 ) );
    int x = 4;
    int y = fontHeight;
    for( auto it = messages_.begin(); it != messages_.end(); ++it )
    {
        p.setPen( it->color_ );
        if( p.font() != it->font_ )
        {
            p.setFont( it->font_ );
            metrics = QFontMetrics( p.font() );
            y += metrics.height() - fontHeight;
            fontHeight = metrics.height();
        }
        const auto list = QStringList::split( '\n', it->message_, true );
        for( auto itLine = list.begin(); itLine != list.end(); ++itLine )
        {
            if( !(*itLine).isEmpty() )
                if( QApplication::isLeftToRight() )
                    p.drawText( x, y, *itLine );
                else
                    p.drawText( p.window().width() - x - metrics.width( *itLine ), y, *itLine );
            if( itLine == list.fromLast() )
                x += metrics.width( *itLine );
            else
            {
                x = 4;
                y += fontHeight;
            }
        }
    }
    QImage image = pixmap.convertToImage();
    RestoreAlpha( image );
    return image;
}

bool Tooltip::Empty() const
{
    return messages_.empty();
}

namespace gui
{
    bool operator<( const QColor& lhs, const QColor& rhs )
    {
        if( lhs.red() < rhs.red() )
            return true;
        if( lhs.red() > rhs.red() )
            return false;
        if( lhs.green() < rhs.green() )
            return true;
        if( lhs.green() > rhs.green() )
            return false;
        if( lhs.blue() < rhs.blue() )
            return true;
        if( lhs.blue() > rhs.blue() )
            return false;
        return lhs.alpha() < rhs.alpha();
    }

    bool operator<( const Tooltip::Message& lhs, const Tooltip::Message& rhs )
    {
        if( lhs.message_ < rhs.message_ )
            return true;
        if( lhs.message_ > rhs.message_ )
            return false;
        if( lhs.font_ < rhs.font_ )
            return true;
        if( lhs.font_ != rhs.font_ )
            return false;
        return lhs.color_ < rhs.color_;
    }
}

bool Tooltip::operator<( const Tooltip& rhs ) const
{
    return messages_ < rhs.messages_;
}
