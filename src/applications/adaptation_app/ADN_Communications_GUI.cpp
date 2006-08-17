// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Communications_GUI.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 17:48 $
// $Revision: 4 $
// $Workfile: ADN_Communications_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Communications_GUI.h"

#include "ADN_Communications_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"

#include <qgroupbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>


// -----------------------------------------------------------------------------
// Name: ADN_Communications_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Communications_GUI::ADN_Communications_GUI( ADN_Communications_Data& data )
: ADN_GUI_ABC( "ADN_Communications_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Communications_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Communications_GUI::~ADN_Communications_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Communications_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Communications_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0 );

    QGroupBox* pGroupBox = new QGroupBox( 3, Qt::Horizontal, tr( "Communications blackout effects" ), pMainWidget_ );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Reload time factor" ), data_.rReloadModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Speed factor" ), data_.rSpeedModifier_, 0, eGreaterEqualZero );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->setAlignment( Qt::AlignTop );
    pLayout->addWidget( pGroupBox );
    builder.AddStretcher( pLayout, Qt::Vertical );
}
