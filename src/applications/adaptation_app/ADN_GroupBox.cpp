// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

#include "adaptation_app_pch.h"
#include "ADN_GroupBox.h"
#include "moc_ADN_GroupBox.cpp"
#include "ADN_Connector_Bool.h"

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( QWidget * parent /* = 0*/, const char * name /* = 0*/ )
: Q3GroupBox( parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( const QString & title, QWidget * parent /* = 0*/, const char * name /* = 0*/ )
: Q3GroupBox( title, parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( int strips, Qt::Orientation orientation, QWidget * parent /* = 0*/, const char * name /* = 0*/ )
: Q3GroupBox( strips, orientation, parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox constructor
// Created: APE 2005-01-03
// -----------------------------------------------------------------------------
ADN_GroupBox::ADN_GroupBox( int strips, Qt::Orientation orientation, const QString & title, QWidget * parent /* = 0*/, const char * name /* = 0*/ )
: Q3GroupBox( strips, orientation, title, parent, name )
, ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
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
// Name: ADN_GroupBox::ADN_GroupBox2
// Created: ABR 2012-01-23
// -----------------------------------------------------------------------------
ADN_GroupBox2::ADN_GroupBox2( QWidget * parent /* = 0 */, const char * name /* = 0 */ )
    : QGroupBox( parent, name )
    , ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox2>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox::ADN_GroupBox2
// Created: ABR 2012-01-23
// -----------------------------------------------------------------------------
ADN_GroupBox2::ADN_GroupBox2( const QString & title, QWidget * parent /* = 0 */, const char * name /* = 0 */ )
    : QGroupBox( title, parent, name )
    , ADN_Gfx_ABC()
{
    this->setCheckable( true );
    this->setChecked( false );
    pConnector_ = new ADN_Connector_Bool<ADN_GroupBox2>( this );
    connect( this, SIGNAL( toggled( bool ) ), this, SLOT( BoolChanged( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox::~ADN_GroupBox2
// Created: ABR 2012-01-23
// -----------------------------------------------------------------------------
ADN_GroupBox2::~ADN_GroupBox2()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GroupBox::BoolChanged
// Created: ABR 2012-01-23
// -----------------------------------------------------------------------------
void ADN_GroupBox2::BoolChanged(bool b)
{
    static_cast<ADN_Connector_Bool<ADN_GroupBox2>*>(pConnector_)->SetDataChanged(b);
}
