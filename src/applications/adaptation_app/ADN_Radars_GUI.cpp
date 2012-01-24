// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Radars_GUI.h"
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Radars_Data.h"
#include "ADN_Radars_ListView.h"
#include "ADN_SearchListView.h"
#include "ADN_GroupBox.h"
#include "ADN_Tr.h"
#include "ADN_TimeField.h"

//-----------------------------------------------------------------------------
// Name: ADN_Radars_GUI constructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Radars_GUI::ADN_Radars_GUI( ADN_Radars_Data& data )
    : ADN_GUI_ABC( "ADN_Radars_GUI" )
    , data_      ( data )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Radars_GUI destructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Radars_GUI::~ADN_Radars_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Radars_GUI::Build
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
void ADN_Radars_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;
    T_ConnectorVector vConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField<ADN_EditLine_String>( pInfoHolder, tr( "Name" ), vConnectors[eName] );
    builder.AddEnumField<E_RadarType>( pInfoHolder,  tr( "Type" ), vConnectors[eType], ADN_Tr::ConvertFromRadarType );
    builder.AddField<ADN_EditLine_Double>( pInfoHolder, tr( "Range" ), vConnectors[eRange], tr( "m" ), eGreaterEqualZero );
    builder.AddOptionnalField<ADN_EditLine_Double>( pInfoHolder, tr( "Min. height" ), vConnectors[eHasMinHeight], vConnectors[eMinHeight], tr( "m" ), eGreaterEqualZero );
    builder.AddOptionnalField<ADN_EditLine_Double>( pInfoHolder, tr( "Max. height" ), vConnectors[eHasMaxHeight], vConnectors[eMaxHeight], tr( "m" ), eGreaterEqualZero );

    // Detectable activities
    ADN_GroupBox* pDetectableActivitiesGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Detectable activities" ) );
    vConnectors[eHasDetectableActivities] = &pDetectableActivitiesGroup->GetConnector();
    for( int n = 0; n < eNbrConsumptionType; ++n )
        builder.AddField<ADN_CheckBox>( pDetectableActivitiesGroup, ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)n, ADN_Tr::eToTr ).c_str(), vConnectors[eHasDetectableActivities + 1 + n] );

    // Detect times
    ADN_GroupBox* pDetectTimesGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Durations" ) );
    vConnectors[eHasDetectionTimes] = &pDetectTimesGroup->GetConnector();
    QWidget* pHolder = builder.AddFieldHolder( pDetectTimesGroup );
    builder.AddOptionnalField<ADN_TimeField>( pHolder, tr( "Detection duration" ), vConnectors[eHasDetectionTime], vConnectors[eDetectionTime] );
    builder.AddOptionnalField<ADN_TimeField>( pHolder, tr( "Recognition duration" ), vConnectors[eHasRecoTime], vConnectors[eRecoTime] );
    builder.AddOptionnalField<ADN_TimeField>( pHolder, tr( "Identification duration" ), vConnectors[eHasIdentificationTime], vConnectors[eIdentificationTime] );

    ADN_GroupBox* pHQDetectTimesGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "CP durations" ), pDetectTimesGroup );
    vConnectors[eHasHQDetectionTimes] = &pHQDetectTimesGroup->GetConnector();
    pHolder = builder.AddFieldHolder( pHQDetectTimesGroup );
    builder.AddOptionnalField<ADN_TimeField>( pHolder, tr( "Detection duration" ), vConnectors[eHasHQDetectionTime], vConnectors[eHQDetectionTime] );
    builder.AddOptionnalField<ADN_TimeField>( pHolder, tr( "Recognition duration" ), vConnectors[eHasHQRecoTime], vConnectors[eHQRecoTime] );
    builder.AddOptionnalField<ADN_TimeField>( pHolder, tr( "Identification duration" ), vConnectors[eHasHQIdentificationTime], vConnectors[eHQIdentificationTime] );

    QLabel* pLabel = new QLabel( pDetectTimesGroup );
    pLabel->setText( tr( "When 'Durations' is unchecked, units are recognized instantly.\n"
                         "When 'HQ Durations' is unchecked, the detection times for HQs are the same as the others." ) );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pDetectableActivitiesGroup );
    pContentLayout->addWidget( pDetectTimesGroup );

    // List view
    ADN_SearchListView< ADN_Radars_ListView >* pSearchListView = new ADN_SearchListView< ADN_Radars_ListView >( data_.vRadars_, vConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView, false, false, true, 0, 0 );
}
