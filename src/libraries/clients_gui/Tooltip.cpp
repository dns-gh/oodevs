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
using namespace kernel;

Tooltip::Tooltip()
{
    // NOTHING
}

Tooltip::~Tooltip()
{
    // NOTHING
}

void Tooltip::Call( const QColor& value )
{
    color_ = value;
}

void Tooltip::Call( const QFont& font )
{
    font_ = font;
    font_.setStyleStrategy( QFont::NoAntialias );
}

void Tooltip::Call( const Styles::Style& value )
{
    font_.setBold( &value == &Styles::bold );
}

void Tooltip::Call( const E_OperationalStatus& value )
{
    font_.setItalic( true );
    if( value == eOperationalStatus_DetruitTactiquement )
        color_.setRgb( 255, 128, 0 );
    else if( value == eOperationalStatus_DetruitTotalement )
        color_.setRgb( 250, 0, 0 );
    Formatter< E_OperationalStatus >()( value, *this );
}

void Tooltip::Call( const E_ForceRatioStatus& value )
{
    font_.setBold( true );
    if( value == eForceRatioStatus_Defavorable )
        color_.setRgb( 160, 0, 0 );
    else
        color_.setRgb( 0, 160, 0 );
    Formatter< E_ForceRatioStatus >()( value, *this );
}

void Tooltip::Call( const E_MeetingEngagementStatus& value )
{
    if( value != eMeetingEngagementStatus_None )
        Formatter< E_MeetingEngagementStatus >()( value, *this );
}

Displayer_ABC& Tooltip::SubItem( const QString& name )
{
    currentItem_ = name;
    return *this;
}

void Tooltip::StartDisplay()
{
    color_ = Qt::black;
    font_  = QFont();
    font_.setStyleStrategy( QFont::NoAntialias );
    message_ = "";
}

void Tooltip::DisplayFormatted( const QString& formatted )
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

void Tooltip::EndDisplay()
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
    for( auto it = list.begin(); it != list.end(); ++it )
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

QImage Tooltip::GenerateImage()
{
    if( new_.empty() )
        return QImage();
    QPixmap pixmap( CreatePixmap() );
    pixmap.fill( Qt::magenta );
    QPainter p;
    p.begin( &pixmap );
    QFontMetrics metrics( p.font() );
    int fontHeight = metrics.height();
    p.setPen( Qt::black );
    p.drawRect( pixmap.rect().adjusted( 0, 0, -1, -1 ) );
    int x = 4;
    int y = fontHeight;
    for( auto it = new_.begin(); it != new_.end(); ++it )
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
    p.end();
    QImage image = pixmap.convertToImage();
    RestoreAlpha( image );
    std::swap( new_, current_ );
    new_.clear();
    return image;
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

void Tooltip::RestoreAlpha( QImage& image ) const
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

QPixmap Tooltip::CreatePixmap() const
{
    int w = 0;
    int h = 0;
    int x = 0;
    for( auto it = new_.begin(); it != new_.end(); ++it )
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
    w = std::max< int >( w + 8, 0 );
    h = std::max< int >( h + 4, 0 );
    return QPixmap( w, h );
}
