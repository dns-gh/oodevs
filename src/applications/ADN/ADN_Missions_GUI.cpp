// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Missions_GUI.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 17:53 $
// $Revision: 3 $
// $Workfile: ADN_Missions_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Missions_GUI.h"

#include "ADN_Missions_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"

#include "ENT/ENT_Tr.h"

#include <qgroupbox.h>
#include <qhbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlayout.h>

// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Missions_GUI::ADN_Missions_GUI( ADN_Missions_Data& data )
: ADN_GUI_ABC( "ADN_Missions_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Missions_GUI::~ADN_Missions_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Missions_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0 );
    
    QGroupBox* pGroupBox = new QGroupBox( 1, Qt::Horizontal, tr( "Special missions" ), pMainWidget_ );
    QWidget* pHolder = builder.AddFieldHolder( pGroupBox );
    builder.AddEnumField<E_AutomataMission>( pHolder, tr( "'Surrender' mission" ), data_.nSurrenderMission_, ENT_Tr::ConvertFromAutomataMission );
    builder.AddEnumField<E_AutomataMission>( pHolder, tr( "'Go to refugee camp' mission" ), data_.nGoToRefugeeCampMission_, ENT_Tr::ConvertFromAutomataMission );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 5 );
    pLayout->addWidget( pGroupBox );
    builder.AddStretcher( pLayout, Qt::Vertical );
}
