// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DurationEditor.h"
#include "clients_kernel/Tools.h"
#include <boost/numeric/conversion/cast.hpp>

using namespace gui;

namespace
{
    int ComputeModifier( const std::string& text, int cursor )
    {
        const std::size_t ucursor = boost::numeric_cast< std::size_t >( cursor );
        if( qApp->isLeftToRight() )
            return ucursor <= text.find_first_of( ':' ) ? 3600
            : ucursor <= text.find_last_of( ':' )       ? 60
            :                                             1;

        return ucursor <= text.find_first_of( ':' ) ? 1
            : ucursor <= text.find_last_of( ':' )   ? 60
            :                                         3600;
    }
}

// -----------------------------------------------------------------------------
// Name: DurationEditor constructor
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
DurationEditor::DurationEditor( QWidget* parent /*= 0*/ )
    : QSpinBox( parent )
    , exact_       ( tools::BuildDurationString( "\\d+", "[0-5]\\d", "[0-5]\\d" ) )
    , intermediate_( tools::BuildDurationString( "\\d*", "\\d*", "\\d*" ) )
{
    setValue( 0 );
    setMaximum( std::numeric_limits< int >::max() );
}

// -----------------------------------------------------------------------------
// Name: DurationEditor destructor
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
DurationEditor::~DurationEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DurationEditor::valueFromText
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
int DurationEditor::valueFromText( const QString& text ) const
{
    QStringList buffer = text.split( ":" );
    if( buffer.size() < 3 )
        throw std::exception( tools::translate( "DurationEditor", "Invalid duration '%1'" ).arg( text ) );
    if( qApp->isLeftToRight() )
        return buffer.at( 0 ).toInt() * 3600 + buffer.at( 1 ).toInt() * 60 + buffer.at( 2 ).toInt();
    return buffer.at( 2 ).toInt() * 3600 + buffer.at( 1 ).toInt() * 60 + buffer.at( 0 ).toInt();
}

// -----------------------------------------------------------------------------
// Name: DurationEditor::textFromValue
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
QString DurationEditor::textFromValue( int value ) const
{
    return tools::DurationFromSeconds( value );
}

// -----------------------------------------------------------------------------
// Name: DurationEditor::stepBy
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
void DurationEditor::stepBy( int steps )
{
    const std::string text( lineEdit()->text().toStdString() );
    const int cursor = lineEdit()->cursorPosition();
    setValue( value() + steps * ComputeModifier( text, cursor ) );
}

// -----------------------------------------------------------------------------
// Name: DurationEditor::validate
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
QValidator::State DurationEditor::validate( QString& text, int& ) const
{
    return exact_.exactMatch( text )        ? QValidator::Acceptable :
           intermediate_.exactMatch( text ) ? QValidator::Intermediate :
                                              QValidator::Invalid;
}
