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
#include "moc_ADN_Radars_GUI.cpp"
#include "ADN_GuiBuilder.h"
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
    : ADN_GUI_ABC( eSensors )
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
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "special-tab" );
    T_ConnectorVector vConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    ADN_EditLine_ABC* nameField = builder.AddLocalizedField( pInfoHolder, "name", tr( "Name" ), vConnectors[ eName ] );
    nameField->ConnectWithRefValidity( data_.GetRadars() );

    builder.AddEnumField( pInfoHolder, "type", tr( "Type" ), vConnectors[ eType ] );
    builder.AddField< ADN_EditLine_Double >( pInfoHolder, "range", tr( "Range" ), vConnectors[ eRange ], tr( "m" ), eGreaterEqualZero );
    builder.AddCheckableField< ADN_EditLine_Double >( pInfoHolder, "min-height", tr( "Min. height" ), vConnectors[ eHasMinHeight ], vConnectors[ eMinHeight ], tr( "m" ), eGreaterEqualZero );
    builder.AddCheckableField< ADN_EditLine_Double >( pInfoHolder, "max-height", tr( "Max. height" ), vConnectors[ eHasMaxHeight ], vConnectors[ eMaxHeight ], tr( "m" ), eGreaterEqualZero );

    // Detectable activities
    ADN_GroupBox* pDetectableActivitiesGroup = builder.AddGroupBox( 0, "detectable", tr( "Detectable activities" ), vConnectors[ eHasDetectableActivities ], 3 );
    for( int n = 0; n < eNbrConsumptionType; ++n )
    {
        builder.AddField< ADN_CheckBox >( pDetectableActivitiesGroup,
                                          ADN_Tr::ConvertFromConsumptionType( static_cast< E_ConsumptionType >( n ), ADN_Tr::eToSim ).c_str(),
                                          ADN_Tr::ConvertFromConsumptionType( static_cast< E_ConsumptionType >( n ), ADN_Tr::eToTr ).c_str(),
                                          vConnectors[ eHasDetectableActivities + 1 + n ] );
    }

    // Detect times
    ADN_GroupBox* pDetectTimesGroup = builder.AddGroupBox( 0, "durations", tr( "Durations" ), vConnectors[ eHasDetectionTimes ], 3 );
    builder.AddCheckableField< ADN_TimeField >( pDetectTimesGroup, "detection-duration", tr( "Detection duration" ), vConnectors[ eHasDetectionTime ], vConnectors[ eDetectionTime ] );
    builder.AddCheckableField< ADN_TimeField >( pDetectTimesGroup, "recognition-duration", tr( "Recognition duration" ), vConnectors[ eHasRecoTime ], vConnectors[ eRecoTime ] );
    builder.AddCheckableField< ADN_TimeField >( pDetectTimesGroup, "identification-duration", tr( "Identification duration" ), vConnectors[ eHasIdentificationTime ], vConnectors[ eIdentificationTime ] );

    ADN_GroupBox* pHQDetectTimesGroup = builder.AddGroupBox( pDetectTimesGroup, "cp-durations", tr( "CP durations" ), vConnectors[ eHasHQDetectionTimes ], 3 );
    builder.AddCheckableField< ADN_TimeField >( pHQDetectTimesGroup, "detection-duration", tr( "Detection duration" ), vConnectors[ eHasHQDetectionTime ], vConnectors[ eHQDetectionTime ] );
    builder.AddCheckableField< ADN_TimeField >( pHQDetectTimesGroup, "recognition-duration", tr( "Recognition duration" ), vConnectors[ eHasHQRecoTime ], vConnectors[ eHQRecoTime ] );
    builder.AddCheckableField< ADN_TimeField >( pHQDetectTimesGroup, "identification-duration", tr( "Identification duration" ), vConnectors[ eHasHQIdentificationTime ], vConnectors[ eHQIdentificationTime ] );

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
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Radars_ListView >( this, data_.vRadars_, vConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView, false, false, true, 0, 0 );
    builder.PopSubName(); //! special-tab
}
