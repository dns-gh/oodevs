// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TimeEdit.h"
#include "moc_ADN_TimeEdit.cpp"
#include "ADN_Connector_String.h"
#include "ADN_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit constructor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
ADN_TimeEdit::ADN_TimeEdit( QWidget* parent )
    : QTimeEdit( parent )
{
    // For now, the only ADN_TimeEdit that we use doesn't need seconds, so we
    // remove them here and in ADN_Tools's locale time to xml time conversion
    // functions.
    // If seconds are needed someday, add an ADN_StandardItem's type to handle
    // seconds, and add an option to this editor.
    setDisplayFormat( ADN_Tools::GetLocalFormatWithoutSeconds() );
    setTime( QTime( 0, 0 ) );
    pConnector_ = new ADN_Connector_String< ADN_TimeEdit >( this );
    connect( this, SIGNAL( timeChanged( const QTime& ) ), this, SLOT( TimeChanged( const QTime& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit destructor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
ADN_TimeEdit::~ADN_TimeEdit()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit::text
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
QString ADN_TimeEdit::text() const
{
    return ADN_Tools::ConvertLocalTimeToXmlTime( time().toString( ADN_Tools::GetLocalFormatWithoutSeconds() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit::setText
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_TimeEdit::setText( const QString& text )
{
    if( text.isEmpty() )
        setTime( QTime( 0, 0 ) );
    else
        setTime( QTime::fromString( ADN_Tools::ConvertXmlTimeToLocalTime( text ), ADN_Tools::GetLocalFormatWithoutSeconds() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit::TimeChanged
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_TimeEdit::TimeChanged( const QTime& time )
{
    static_cast< ADN_Connector_String< ADN_TimeEdit >* >( pConnector_ )->SetDataChanged( ADN_Tools::ConvertLocalTimeToXmlTime( time.toString( ADN_Tools::GetLocalFormatWithoutSeconds() ) ) );
}
