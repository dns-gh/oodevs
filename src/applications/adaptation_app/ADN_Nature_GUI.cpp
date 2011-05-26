// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Nature_GUI.h"
#include "moc_ADN_Nature_GUI.cpp"
#include "ADN_Connector_String.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI constructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
ADN_Nature_GUI::ADN_Nature_GUI( QWidget* parent )
    : NatureEditionWidget( parent )
    , ADN_Gfx_ABC()
{
    pConnector_ = new ADN_Connector_String< ADN_Nature_GUI >( this );
    connect( this, SIGNAL( textChanged( const QString& ) ), SLOT( OnTextChanged( const QString& ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL( OpenModeToggled() ), SLOT( UpdateEnableState() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI destructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
ADN_Nature_GUI::~ADN_Nature_GUI()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI::OnTextChanged
// Created: SBO 2010-02-15
// -----------------------------------------------------------------------------
void ADN_Nature_GUI::OnTextChanged( const QString& value )
{
    static_cast< ADN_Connector_String< ADN_Nature_GUI >* >( pConnector_ )->SetDataChanged( value );
}

// -----------------------------------------------------------------------------
// Name: ADN_Nature_GUI::UpdateEnableState
// Created: SBO 2010-02-15
// -----------------------------------------------------------------------------
void ADN_Nature_GUI::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_String< ADN_Nature_GUI >* >( pConnector_ )->IsConnected() );
}
