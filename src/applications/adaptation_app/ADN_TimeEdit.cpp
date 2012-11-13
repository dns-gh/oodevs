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
#include "ADN_App.h"
#include "ADN_Workspace.h"

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit constructor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
ADN_TimeEdit::ADN_TimeEdit( QWidget* parent, const QString& format /* = "hh:mm"*/ )
    : QTimeEdit( parent )
    , format_( format )
{
    setTime( QTime( 0, 0 ) );
    pConnector_ = new ADN_Connector_String< ADN_TimeEdit >( this );
    connect( this, SIGNAL( timeChanged( const QTime& ) ), this, SLOT( TimeChanged( const QTime& ) ) );
    connect( ADN_App::GetMainWindow(), SIGNAL( OpenModeToggled() ), this, SLOT( UpdateEnableState() ) );
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
// Name: ADN_TimeEdit::setEnabled
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_TimeEdit::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QTimeEdit::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QTimeEdit::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit::text
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
QString ADN_TimeEdit::text() const
{
    return time().toString( format_ );
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
        setTime( QTime::fromString( text, format_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit::TimeChanged
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_TimeEdit::TimeChanged( const QTime& time )
{
    static_cast< ADN_Connector_String< ADN_TimeEdit >* >( pConnector_ )->SetDataChanged( time.toString( format_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_TimeEdit::UpdateEnableState
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_TimeEdit::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_String< ADN_TimeEdit >* >( pConnector_ )->IsConnected() );
}
