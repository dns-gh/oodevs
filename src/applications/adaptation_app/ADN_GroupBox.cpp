// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GroupBox.cpp $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 5 $
// $Workfile: ADN_GroupBox.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_GroupBox.h"
#include "moc_ADN_GroupBox.cpp"

#include "ADN_Connector_Bool.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"
#include "ADN_Enums.h"
#include "ADN_Workspace.h"


// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( QWidget * parent /*= 0*/, const char * name /*= 0*/ )
: QGroupBox( parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( const QString & title, QWidget * parent /*= 0*/, const char * name /*= 0*/ )
: QGroupBox( title, parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( int strips, Orientation orientation, QWidget * parent /*= 0*/, const char * name /*= 0*/ )
: QGroupBox( strips, orientation, parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( int strips, Orientation orientation, const QString & title, QWidget * parent /*= 0*/, const char * name /*= 0*/ )
: QGroupBox( strips, orientation, title, parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}


// -----------------------------------------------------------------------------
// Name: ADN_GroupBox destructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::~ADN_GroupBox()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_GroupBox::BoolChanged
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_GroupBox::BoolChanged(bool b)
{
    static_cast<ADN_Connector_Bool<ADN_GroupBox>*>(pConnector_)->SetDataChanged(b);
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox::setEnabled
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_GroupBox::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QGroupBox::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QGroupBox::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_GroupBox::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_Bool<ADN_GroupBox>* >( pConnector_ )->IsConnected() );
}
