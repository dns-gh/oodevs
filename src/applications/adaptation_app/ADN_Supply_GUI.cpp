// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Supply_GUI.h"

#include "ADN_Supply_Data.h"
#include "ADN_Supply_TrucksTable.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Tr.h"
#include "ADN_SupplyUnitSelector.h"
#include "ADN_AvailabilityWarningTable.h"

#include "ENT/ENT_Tr.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qhgroupbox.h>
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
    QGroupBox* pGroup = new QGroupBox( 5, Qt::Horizontal, tr( "Supply" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );

    builder.AddField< ADN_SupplyUnitSelector >( pHolder, tr( "Unit" ), data_.ptrUnit_ );
    builder.AddEnumField<E_UnitMission>( pHolder, tr( "Mission" ), data_.nSupplyMission_, ENT_Tr::ConvertFromUnitMission );

    QHGroupBox* pTrucksGroup = new QHGroupBox( tr( "Convoy setup times" ), pGroup );
    ADN_Supply_TrucksTable* pTrucksTable = new ADN_Supply_TrucksTable( pTrucksGroup );
    pTrucksTable->GetConnector().Connect( & data_.vConvoySetupInfos_ );

    pTrucksGroup = new QHGroupBox( tr( "Convoy loading times" ), pGroup );
    pTrucksTable = new ADN_Supply_TrucksTable( pTrucksGroup );
    pTrucksTable->GetConnector().Connect( & data_.vConvoyLoadingInfos_ );

    pTrucksGroup = new QHGroupBox( tr( "Convoy unloading times" ), pGroup );
    pTrucksTable = new ADN_Supply_TrucksTable( pTrucksGroup );
    pTrucksTable->GetConnector().Connect( & data_.vConvoyUnloadingInfos_ );

    QHGroupBox* pVectorGroup = new QHGroupBox( tr( "Vector availability warnings" ), pGroup );
    ADN_AvailabilityWarningTable* pWarningTable = new ADN_AvailabilityWarningTable( pVectorGroup );
    pWarningTable->GetConnector().Connect( & data_.vVectorWarnings_ );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
    builder.AddStretcher( pLayout, Qt::Vertical );
}
