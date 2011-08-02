// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlTooltip.h"
#include "TooltipsLayer_ABC.h"
#include "clients_kernel/Styles.h"
#include <boost/bind.hpp>

using namespace gui;
using namespace kernel;

namespace
{
    void DrawFrame( QPainter& painter, const QRect& rect )
    {
        painter.setPen( Qt::black );
        painter.drawRect( rect );
    }
}

// -----------------------------------------------------------------------------
// Name: GlTooltip constructor
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
GlTooltip::GlTooltip( TooltipsLayer_ABC& layer )
    : layer_      ( layer )
    , frameDrawer_( boost::bind( &::DrawFrame, _1, _2 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlTooltip destructor
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
GlTooltip::~GlTooltip()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const QColor& value )
{
    color_ = value;
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void GlTooltip::Call( const QFont& font )
{
    font_ = font;
    font_.setStyleStrategy( QFont::NoAntialias );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void GlTooltip::Call( const Styles::Style& value )
{
    font_.setBold( &value == &Styles::bold );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_OperationalStatus& value )
{
    font_.setItalic( true );
    if( value == eOperationalStatus_DetruitTactiquement )
        color_.setRgb( 255, 128, 0 );
    else if( value == eOperationalStatus_DetruitTotalement )
        color_.setRgb( 250, 0, 0 );
    Formatter< E_OperationalStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_ForceRatioStatus& value )
{
    font_.setBold( true );
    if( value == eForceRatioStatus_Defavorable )
        color_.setRgb( 160, 0, 0 );
    else
        color_.setRgb( 0, 160, 0 );
    Formatter< E_ForceRatioStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_MeetingEngagementStatus& value )
{
    if( value != eMeetingEngagementStatus_None )
        Formatter< E_MeetingEngagementStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::SubItem
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
Displayer_ABC& GlTooltip::SubItem( const QString& name )
{
    currentItem_ = name;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::StartDisplay
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::StartDisplay()
{
    color_ = Qt::black;
    font_  = QFont();
    font_.setStyleStrategy( QFont::NoAntialias );
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::DisplayFormatted
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

namespace
{
    struct Style
    {
        explicit Style( const QFont& font ) : font_( font ) {}
        void Push( const QString& style )
        {
            styles_ += style;
        }
        void Pop()
        {
            styles_.truncate( styles_.length() ? styles_.length() - 1 : 0 );
        }
        QFont Font()
        {
            QFont result( font_ );
            result.setBold( result.bold() || styles_.contains( 'b' ) );
            result.setItalic( result.italic() || styles_.contains( 'i' ) );
            result.setUnderline( result.underline() || styles_.contains( 'u' ) || styles_.contains( 'a' ) );
            return result;
        }
        QColor color_;
        QFont font_;
        QString styles_;
    };
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::EndDisplay
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::EndDisplay()
{
    if( ! currentItem_.isEmpty() )
        message_ = currentItem_ + " " + message_ + "\n";
    else if( ! message_.isEmpty() )
        message_ += "\n";
    message_.replace( QRegExp( "<a *href=\".*\" *>" ), "<a>" );
    QRegExp rx( "<(/?[abiu])>" );
    QStringList styles;
    if( rx.search( message_ ) > 0 )
    {
        styles = rx.capturedTexts();
        styles.pop_front();
    }
    const QStringList list = QStringList::split( rx, message_ );
    ::Style style( font_ );
    QStringList::const_iterator itStyle = styles.begin();
    if( itStyle != styles.end() && ! (*itStyle).startsWith( "/" ) && message_.startsWith( QString( "<%1>" ).arg( *itStyle ) ) )
    {
        style.Push( *itStyle );
        ++itStyle;
    }
    for( QStringList::const_iterator it = list.begin(); it != list.end(); ++it )
    {
        new_.push_back( T_Message( *it, T_Style( color_, style.Font() ) ) );
        if( itStyle != styles.end() )
        {
            if( (*itStyle).startsWith( "/" ) )
                style.Pop();
            else
                style.Push( *itStyle );
            ++itStyle;
        }
    }
    DirtyImage();
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::DirtyImage
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void GlTooltip::DirtyImage()
{
    // $$$$ SBO 2008-10-06: optimisation shows a side effect in new_ filling order... fix and uncomment
//    if( new_.size()  != current_.size()
//     || new_.front() != current_.front()
//     || new_.back()  != current_.at( new_.size() - 1 ) )
        image_ = QImage();
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Draw
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Draw( const geometry::Point2f& position, int width /* = 0*/, int height /* = 0*/, float factor /* = 1.f*/ )
{
    if( image_.isNull() )
        GenerateImage( std::abs( width ), std::abs( height ) );
    new_.clear();
    geometry::Point2f point = position;
    if( width < 0 )
        point.Set( point.X() - image_.width() * factor, point.Y() );
    if( height < 0 )
        point.Set( point.X(), point.Y() - image_.height() * factor );
    layer_.Draw( point, image_ );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::GenerateImage
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::GenerateImage( unsigned int width, unsigned int height )
{
    if( new_.empty() )
        return;
    QPixmap pixmap( CreatePixmap( width, height ) );
    pixmap.fill( Qt::magenta );
    QPainter p;
    p.begin( &pixmap );
        QFontMetrics metrics( p.font() );
        int fontHeight = metrics.height();
        frameDrawer_( p, pixmap.rect() );
        int x = 4;
        int y = fontHeight;
        for( CIT_Messages it = new_.begin(); it != new_.end(); ++it )
        {
            p.setPen( it->second.first );
            if( p.font() != it->second.second )
            {
                p.setFont( it->second.second );
                metrics = QFontMetrics( p.font() );
                y += metrics.height() - fontHeight;
                fontHeight = metrics.height();
            }
            const QStringList list = QStringList::split( '\n', it->first, true );
            for( QStringList::const_iterator itLine = list.begin(); itLine != list.end(); ++itLine )
            {
                if( !(*itLine).isEmpty() )
                    p.drawText( x, y, *itLine );
                if( itLine == list.fromLast() )
                    x += metrics.width( *itLine );
                else
                {
                    x = 4;
                    y += fontHeight;
                }
            }
        }
    p.end();
    image_ = pixmap.convertToImage().mirror();
    RestoreAlpha();
    std::swap( new_, current_ );
    new_.clear();
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
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::RestoreAlpha
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::RestoreAlpha()
{
    if( image_.isNull() )
        return;
    unsigned int* data = (unsigned int*)image_.bits();
    for( int i = image_.width() * image_.height(); i > 0; --i )
    {
        if( IsMagenta( *data ) )
            *data |= 0xE000FF00;
        else
            *data |= 0xFF000000;
        ++data;
    }
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::CreatePixmap
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
QPixmap GlTooltip::CreatePixmap( unsigned int width, unsigned int height )
{
    int w = 0;
    int h = 0;
    int x = 0;
    for( CIT_Messages it = new_.begin(); it != new_.end(); ++it )
    {
        const QFontMetrics metrics( it->second.second );
        int fontHeight = metrics.height();

        const QStringList list = QStringList::split( '\n', it->first, true );
        for( QStringList::const_iterator itLine = list.begin(); itLine != list.end(); ++itLine )
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
    w = std::max< int >( w + 8, width );
    h = std::max< int >( h + 4, height );
    return QPixmap( w, h);
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Hide
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Hide()
{
    current_.clear();
    new_.clear();
    image_ = QImage();
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::operator kernel::Displayer_ABC&
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
GlTooltip::operator kernel::Displayer_ABC&()
{
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::SetFrameDrawer
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void GlTooltip::SetFrameDrawer( const FrameDrawer& frameDrawer )
{
    frameDrawer_ = frameDrawer;
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::BoundingBox
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
QSize GlTooltip::Size() const
{
    return image_.size();
}
