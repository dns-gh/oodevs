// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TooltipDisplayer.h"
#include "Tooltip.h"

using namespace gui;
using namespace kernel;

TooltipDisplayer::TooltipDisplayer()
{
    // NOTHING
}

TooltipDisplayer::~TooltipDisplayer()
{
    // NOTHING
}

void TooltipDisplayer::Call( const QColor& value )
{
    color_ = value;
}

void TooltipDisplayer::Call( const QFont& font )
{
    font_ = font;
    font_.setStyleStrategy( QFont::NoAntialias );
}

void TooltipDisplayer::Call( const Styles::Style& value )
{
    font_.setBold( &value == &Styles::bold );
}

void TooltipDisplayer::Call( const E_OperationalStatus& value )
{
    font_.setItalic( true );
    if( value == eOperationalStatus_DetruitTactiquement )
        color_.setRgb( 255, 128, 0 );
    else if( value == eOperationalStatus_DetruitTotalement )
        color_.setRgb( 250, 0, 0 );
    Formatter< E_OperationalStatus >()( value, *this );
}

void TooltipDisplayer::Call( const E_ForceRatioStatus& value )
{
    font_.setBold( true );
    if( value == eForceRatioStatus_Defavorable )
        color_.setRgb( 160, 0, 0 );
    else
        color_.setRgb( 0, 160, 0 );
    Formatter< E_ForceRatioStatus >()( value, *this );
}

void TooltipDisplayer::Call( const E_MeetingEngagementStatus& value )
{
    if( value != eMeetingEngagementStatus_None )
        Formatter< E_MeetingEngagementStatus >()( value, *this );
}

Displayer_ABC& TooltipDisplayer::SubItem( const QString& name )
{
    currentItem_ = name;
    return *this;
}

void TooltipDisplayer::StartDisplay()
{
    color_ = Qt::black;
    font_ = QFont();
    font_.setStyleStrategy( QFont::NoAntialias );
    message_.clear();
}

void TooltipDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

namespace
{
    struct Style
    {
        Style( const QFont& font )
            : font_( font )
        {}
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

void TooltipDisplayer::Collect( Tooltip::T_Messages& messages ) const
{
    QRegExp rx( "<(/?[abiu])>" );
    QStringList styles;
    if( rx.search( message_ ) > 0 )
    {
        styles = rx.capturedTexts();
        styles.pop_front();
    }
    const auto list = QStringList::split( rx, message_ );
    Style style( font_ );
    auto itStyle = styles.begin();
    if( itStyle != styles.end() && ! itStyle->startsWith( "/" ) && message_.startsWith( QString( "<%1>" ).arg( *itStyle ) ) )
    {
        style.Push( *itStyle );
        ++itStyle;
    }
    for( auto it = list.begin(); it != list.end(); ++it )
    {
        const Tooltip::Message message = { *it, color_, style.Font() };
        messages.push_back( message );
        if( itStyle != styles.end() )
        {
            if( itStyle->startsWith( "/" ) )
                style.Pop();
            else
                style.Push( *itStyle );
            ++itStyle;
        }
    }
}

void TooltipDisplayer::EndDisplay()
{
    if( !currentItem_.isEmpty() )
        message_ = currentItem_ + " " + message_ + "\n";
    else if( ! message_.isEmpty() )
        message_ += "\n";
    message_.replace( QRegExp( "<a *href=\".*\" *>" ), "<a>" );
    Collect( messages_ );
    DirtyImage();
}

Tooltip TooltipDisplayer::GetTooltip()
{
    const Tooltip tooltip( messages_ );
    messages_.clear();
    return tooltip;
}
