//*****************************************************************************
//
// $Created: JDY 03-07-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Radars_GUI.cpp $
// $Author: Ape $
// $Modtime: 22/06/05 14:41 $
// $Revision: 2 $
// $Workfile: ADN_Radars_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Radars_GUI.h"

#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3vbox.h>

#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Radars_Data.h"
#include "ADN_Radars_ListView.h"
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
}


//-----------------------------------------------------------------------------
// Name: ADN_Radars_GUI destructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Radars_GUI::~ADN_Radars_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Radars_GUI::Build
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
void ADN_Radars_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "frame radars" );

    // Radar listview
    ADN_Radars_ListView* pRadars = new ADN_Radars_ListView( pMainWidget_ );
    pRadars->GetConnector().Connect(&data_.vRadars_);
    T_ConnectorVector vConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Radar data
    Q3GroupBox* pGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Special sensor" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vConnectors[eName] );
    builder.AddEnumField<E_RadarType>( pHolder,  tr( "Type" ), vConnectors[eType], ADN_Tr::ConvertFromRadarType );

    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Range" ), vConnectors[eRange], tr( "m" ), eGreaterEqualZero );
    builder.AddOptionnalField<ADN_EditLine_Double>( pHolder, tr( "Min. height" ), vConnectors[eHasMinHeight], vConnectors[eMinHeight], tr( "m" ), eGreaterEqualZero );
    builder.AddOptionnalField<ADN_EditLine_Double>( pHolder, tr( "Max. height" ), vConnectors[eHasMaxHeight], vConnectors[eMaxHeight], tr( "m" ), eGreaterEqualZero );

    ADN_GroupBox* pDetectableActivitiesGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Detectable activities" ), pGroup );
    vConnectors[eHasDetectableActivities] = &pDetectableActivitiesGroup->GetConnector();
    for( int n = 0; n < eNbrConsumptionType; ++n )
        builder.AddField<ADN_CheckBox>( pDetectableActivitiesGroup, ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)n ).c_str(), vConnectors[eHasDetectableActivities + 1 + n] );

    ADN_GroupBox* pDetectTimesGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Durations" ), pGroup );
    vConnectors[eHasDetectionTimes] = &pDetectTimesGroup->GetConnector();
    pHolder = builder.AddFieldHolder( pDetectTimesGroup );
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

    builder.AddStretcher( pGroup, Qt::Vertical );

    // Connect the gui to the data.
    pRadars->SetItemConnectors(vConnectors);

    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pRadars, 1 );
    pMainLayout->addWidget( pGroup, 4 );
}
