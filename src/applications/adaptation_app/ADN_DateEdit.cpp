// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_DateEdit.h"
#include "moc_ADN_DateEdit.cpp"
#include "ADN_Connector_String.h"

const QString ADN_DateEdit::strFormat_( "yyyy-MM-dd" );

// -----------------------------------------------------------------------------
// Name: ADN_DateEdit constructor
// Created: JSR 2011-10-13
// -----------------------------------------------------------------------------
ADN_DateEdit::ADN_DateEdit( QWidget* parent )
    : QDateEdit( parent )
{
    setDate( QDate( 1900, 1, 1 ) );
    setCalendarPopup( true );
    pConnector_.reset( new ADN_Connector_String< ADN_DateEdit >( this ) );
    connect( this, SIGNAL( dateChanged( const QDate& ) ), this, SLOT( DateChanged( const QDate& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_DateEdit destructor
// Created: JSR 2011-10-13
// -----------------------------------------------------------------------------
ADN_DateEdit::~ADN_DateEdit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_DateEdit::text
// Created: JSR 2011-10-13
// -----------------------------------------------------------------------------
QString ADN_DateEdit::text() const
{
    return date().toString( strFormat_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_DateEdit::setText
// Created: JSR 2011-10-13
// -----------------------------------------------------------------------------
void ADN_DateEdit::setText( const QString& text )
{
    if( text.isEmpty() )
        setDate( QDate( 1900, 1, 1 ) );
    else
        setDate( QDate::fromString( text, strFormat_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_DateEdit::DateChanged
// Created: JSR 2011-10-13
// -----------------------------------------------------------------------------
void ADN_DateEdit::DateChanged( const QDate& date )
{
    static_cast< ADN_Connector_String< ADN_DateEdit >& >( *pConnector_ ).SetDataChanged( date.toString( strFormat_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_DateEdit::Warn
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_DateEdit::Warn( ADN_ErrorStatus, const QString& )
{
    // NOTHING
}
