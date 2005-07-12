// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_GUI.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 18:14 $
// $Revision: 6 $
// $Workfile: ADN_Supply_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Supply_GUI.h"

#include "ADN_Supply_Data.h"
#include "ADN_Supply_TrucksTable.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Tr.h"
#include "ADN_SupplyUnitSelector.h"

#include "ENT/ENT_Tr.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>


// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Supply_GUI::ADN_Supply_GUI( ADN_Supply_Data& data )
: ADN_GUI_ABC( "ADN_Supply_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Supply_GUI::~ADN_Supply_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Supply_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Supply_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );
    QGroupBox* pGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Supply" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );

    builder.AddField< ADN_SupplyUnitSelector >( pHolder, tr( "Unit" ), data_.ptrUnit_ );
    builder.AddEnumField<E_UnitMission>( pHolder, tr( "Mission" ), data_.nSupplyMission_, ENT_Tr::ConvertFromUnitMission );

    QGroupBox* pTrucksGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Convoy setup times" ), pGroup );
    ADN_Supply_TrucksTable* pTrucksTable = new ADN_Supply_TrucksTable( pTrucksGroup );
    pTrucksTable->GetConnector().Connect( & data_.vConvoySetupInfos_ );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
    builder.AddStretcher( pLayout, Qt::Vertical );
}
