// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InformationToolTip.h"
#include "moc_InformationToolTip.cpp"
#include "clients_kernel/Styles.h"
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

namespace
{
    void DrawFrame( QPainter& painter, const QRect& rect )
    {
        painter.setPen( Qt::black );
        painter.drawRect( rect.adjusted( 0, 0, -1, -1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip constructor
// Created: NPT 2013-02-11
// -----------------------------------------------------------------------------
InformationToolTip::InformationToolTip()
    : QDialog( QApplication::activeWindow(), Qt::ToolTip )
    , frameDrawer_( boost::bind( &::DrawFrame, _1, _2 ) )
{
    setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip destructor
// Created: NPT 2013-02-11
// -----------------------------------------------------------------------------
InformationToolTip::~InformationToolTip()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Call
// -----------------------------------------------------------------------------
void InformationToolTip::Call( const QColor& value )
{
    color_ = value;
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Call
// -----------------------------------------------------------------------------
void InformationToolTip::Call( const QFont& font )
{
    font_ = font;
    font_.setStyleStrategy( QFont::NoAntialias );
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Call
// -----------------------------------------------------------------------------
void InformationToolTip::Call( const Styles::Style& value )
{
    font_.setBold( &value == &Styles::bold );
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Call
// -----------------------------------------------------------------------------
void InformationToolTip::Call( const E_OperationalStatus& value )
{
    font_.setItalic( true );
    if( value == eOperationalStatus_DetruitTactiquement )
        color_.setRgb( 255, 128, 0 );
    else if( value == eOperationalStatus_DetruitTotalement )
        color_.setRgb( 250, 0, 0 );
    Formatter< E_OperationalStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Call
// -----------------------------------------------------------------------------
void InformationToolTip::Call( const E_ForceRatioStatus& value )
{
    font_.setBold( true );
    if( value == eForceRatioStatus_Defavorable )
        color_.setRgb( 160, 0, 0 );
    else
        color_.setRgb( 0, 160, 0 );
    Formatter< E_ForceRatioStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Call
// -----------------------------------------------------------------------------
void InformationToolTip::Call( const E_MeetingEngagementStatus& value )
{
    if( value != eMeetingEngagementStatus_None )
        Formatter< E_MeetingEngagementStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::SubItem
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
Displayer_ABC& InformationToolTip::SubItem( const QString& name )
{
    currentItem_ = name;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::StartDisplay
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void InformationToolTip::StartDisplay()
{
    color_ = Qt::black;
    font_  = QFont();
    font_.setStyleStrategy( QFont::NoAntialias );
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::DisplayFormatted
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void InformationToolTip::DisplayFormatted( const QString& formatted )
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
// Name: InformationToolTip::EndDisplay
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void InformationToolTip::EndDisplay()
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

// -----------------------------------------------------------------------------
// Name: GlTooltip::DirtyImage
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void InformationToolTip::DirtyImage()
{
    image_ = QImage();
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Draw
// Created: NPT 2013-02-19
// -----------------------------------------------------------------------------
void InformationToolTip::Draw()
{
    if( !isVisible() && !image_.isNull() && QApplication::activePopupWidget() == 0 )
        show();

    if( image_.isNull() )
        GenerateImage();
    new_.clear();
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::GenerateImage
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void InformationToolTip::GenerateImage()
{
    if( new_.empty() )
        return;

    QPixmap pixmap( CreatePixmap() );
    pixmap.fill( Qt::magenta );
    QPainter p;
    p.begin( &pixmap );
    QFontMetrics metrics( p.font() );
    int fontHeight = metrics.height();
    frameDrawer_( p, pixmap.rect() );
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
    image_ = pixmap.convertToImage();
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
// Name: InformationToolTip::RestoreAlpha
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void InformationToolTip::RestoreAlpha()
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
// Name: InformationToolTip::CreatePixmap
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
QPixmap InformationToolTip::CreatePixmap()
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
    return QPixmap( w, h);
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::Hide
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void InformationToolTip::Hide()
{
    new_.clear();
    current_.clear();
    image_ = QImage();
    if( isVisible() )
        setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::operator kernel::Displayer_ABC&
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
InformationToolTip::operator kernel::Displayer_ABC&()
{
    return *this;
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::SetFrameDrawer
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void InformationToolTip::SetFrameDrawer( const FrameDrawer& frameDrawer )
{
    frameDrawer_ = frameDrawer;
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::BoundingBox
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
QSize InformationToolTip::Size() const
{
    return image_.size();
}

// -----------------------------------------------------------------------------
// Name: InformationToolTip::paintEvent
// Created: NPT 2013-02-19
// -----------------------------------------------------------------------------
void InformationToolTip::paintEvent( QPaintEvent * /*event*/ )
{
    if( !image_.isNull() && QApplication::activeWindow() )
    {
        QPainter p( this );
        QPoint pos = QCursor::pos();
        QRect screen = QApplication::desktop()->screenGeometry( QApplication::desktop()->screenNumber( pos ) );
        if( screen.left() + screen.width() < pos.x() + image_.width() )
            pos.setX( pos.x() - image_.width() );
        if( screen.top() + screen.height() < pos.y() + image_.height() )
            pos.setY( pos.y() - image_.height() );
        move( pos + QPoint( 0, 20 ) ); //mouse shape default size
        p.drawImage( 0, 0, image_ );
        setFixedSize( image_.width(), image_.height() );
    }
}
