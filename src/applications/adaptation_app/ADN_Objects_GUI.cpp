//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Objects_GUI.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:37 $
// $Revision: 18 $
// $Workfile: ADN_Objects_GUI.cpp $
//
//*****************************************************************************

#include "ADN_pch.h"
#include "ADN_Objects_GUI.h"
#include "moc_ADN_Objects_GUI.cpp"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Objects_Data.h"
#include "ADN_ListView_Objects.h"
#include "ADN_Table_Objects_LocationScore.h"
#include "ADN_Equipement_AttritionTable.h"
#include "ADN_GroupBox.h"
#include "ADN_ComboBox_Enum.h"
#include "ADN_EquipementSelector.h"
#include "ADN_Tr.h"
#include "ADN_GuiBuilder.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qgrid.h>
#include <qwhatsthis.h>


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::ADN_Objects_GUI( ADN_Objects_Data& data )
: ADN_GUI_ABC( "ADN_Objects_GUI" )
, data_      ( data )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Objects_GUI::~ADN_Objects_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::Build
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Objects_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;
    
    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // Create the object listview.
    ADN_ListView_Objects* pList = new ADN_ListView_Objects( pMainWidget_ );
    pList->GetConnector().Connect( &data_.GetObjectInfos() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );
    
    // Create the object panel.
    QGroupBox* pGroup = new QGroupBox( 0, Qt::Horizontal, tr( "Object" ), pMainWidget_ );
    QWidget* pParamGroup = builder.AddFieldHolder( pGroup );

    // Name
    builder.AddField<ADN_EditLine_String>( pParamGroup, tr( "Name"), vInfosConnectors[eName] );
    builder.SetEnabled( false );

    // Bypassed
    builder.AddField<ADN_CheckBox>( pParamGroup, tr( "Can be bypassed"), vInfosConnectors[eCanBeBypassed] );

    // Prepared
    builder.AddField<ADN_CheckBox>( pParamGroup, tr( "Can be prepared"), vInfosConnectors[eCanBePrepared] );

    // Valorized
    builder.AddField<ADN_CheckBox>( pParamGroup, tr( "Can be mined"), vInfosConnectors[eCanBeValorized] );

    // Avoid distance
    builder.AddField<ADN_EditLine_Double>( pParamGroup, tr( "Avoid distance"), vInfosConnectors[eAvoidDistance], tr( "m" ) );

    // Default speed
    builder.AddField<ADN_EditLine_Double>( pParamGroup, tr( "Default speed"), vInfosConnectors[eDefaultSpeed], tr( "km/h" ) );

    // Default speed when bypassed
    builder.AddField<ADN_EditLine_Double>( pParamGroup, tr( "Default speed when bypassed"), vInfosConnectors[eDefaultBypassSpeed], tr( "km/h" ) );

    // Max interaction height
    builder.AddField<ADN_EditLine_Double>( pParamGroup, tr( "Max interaction height"), vInfosConnectors[eMaxInteractionHeight], tr( "m" ) );

    // Consumption
    builder.AddEnumField<E_ConsumptionType>( pParamGroup, tr( "Default consumption"), vInfosConnectors[eDefaultConsumption], ADN_Tr::ConvertFromConsumptionType  );

    // Speed impact
    pSpeedImpactCombo_ = builder.AddEnumField<E_SpeedImpact>( pParamGroup, tr( "Speed impact"), vInfosConnectors[eSpeedImpact], ADN_Tr::ConvertFromSpeedImpact  );

    // Max agent speed
    pMaxAgentSpeed_ = builder.AddField<ADN_EditLine_Double>( pParamGroup, tr( "Max agent speed"), vInfosConnectors[eMaxAgentSpeedPercentage], tr( "%" ), ePercentage );

    connect( pSpeedImpactCombo_, SIGNAL( activated( int ) ), this, SLOT( OnSpeedImpactComboChanged() ) );

    // Outgoing population density
    builder.AddOptionnalField<ADN_EditLine_Double>( pParamGroup, tr( "Outgoing population density"), vInfosConnectors[eHasOutgoingPopulationDensity], vInfosConnectors[eOutgoingPopulationDensity], tr( "people/m²" ), eGreaterEqualZero );

    // Users
    builder.AddField<ADN_EditLine_Int>( pParamGroup, tr( "Max nbr of animators"), vInfosConnectors[eMaxNbrUsers], 0, eGreaterEqualZero );

    // Prepared
    ADN_GroupBox* pBuildGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Construction" ), pGroup );
    vInfosConnectors[eHasToBuild] = &pBuildGroup->GetConnector();
    builder.AddField<ADN_EquipementSelector>( pBuildGroup, tr( "uses"), vInfosConnectors[eToBuild] );

    // Valorized
    ADN_GroupBox* pValorizeGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Mining" ), pGroup );
    vInfosConnectors[eHasToReinforce] = &pValorizeGroup->GetConnector();
    builder.AddField<ADN_EquipementSelector>( pValorizeGroup, tr( "uses"), vInfosConnectors[eToReinforce] );

    //-------------
    // Placement scores
    QGroupBox* pGroupScoreLocation = new QVGroupBox( tr( "Location weights" ), pGroup );

    ADN_Table_Objects_LocationScore* pScoreLocation = new ADN_Table_Objects_LocationScore( pGroupScoreLocation );
    vInfosConnectors[eLocationScores] = &pScoreLocation->GetConnector();
    
    //-------------
    // Attrition
    ADN_GroupBox* pGroupAttrition = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Attrition" ), pGroup );
    vInfosConnectors[eHasAttritions] = &pGroupAttrition->GetConnector();

    ADN_Equipement_AttritionTable* pAttritionTable = new ADN_Equipement_AttritionTable( pGroupAttrition );
    vInfosConnectors[eAttritions] = &pAttritionTable->GetConnector();

    //-------------
    // Population attrition
    ADN_GroupBox* pGroupPopulationAttrition = new ADN_GroupBox( 2, Qt::Horizontal, tr( "Population attrition" ), pGroup );
    vInfosConnectors[eHasPopulationAttritions] = &pGroupPopulationAttrition->GetConnector();

    QWidget* pPopulationAttritionParamGroup = builder.AddFieldHolder( pGroupPopulationAttrition );

    builder.AddField< ADN_EditLine_Double >( pPopulationAttritionParamGroup, tr( "Attrition surface" ), vInfosConnectors[ePopulationAttritionSurface], 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pPopulationAttritionParamGroup, tr( "PH" ), vInfosConnectors[ePopulationAttritionPh], 0, eGreaterEqualZero );

    // Connect the list to the interface.
    pList->SetItemConnectors(vInfosConnectors);

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pList, 1 );
    pMainLayout->addWidget( pGroup, 3 );

    QGridLayout* pGroupLayout = new QGridLayout( pGroup->layout(), 1, 2, 5 );
    pGroupLayout->setAlignment( Qt::AlignTop );

    pGroupLayout->addMultiCellWidget( pParamGroup, 0, 0, 0, 1, Qt::AlignTop );
    QHBoxLayout* pSubLayout = new QHBoxLayout( 0 );
    pSubLayout->addWidget( pBuildGroup );
    pSubLayout->addWidget( pValorizeGroup );
    pGroupLayout->addMultiCellLayout( pSubLayout, 1, 1, 0, 1, Qt::AlignTop );

    pGroupLayout->addWidget( pGroupScoreLocation, 3, 0, Qt::AlignTop );
    pGroupLayout->addWidget( pGroupAttrition, 3, 1, Qt::AlignTop );
    pGroupLayout->addWidget( pGroupPopulationAttrition, 4, 1, Qt::AlignTop );
//    pGroupLayout->setRowStretch( 4, 10000 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Objects_GUI::OnSpeedImpactComboChanged
// Created: APE 2005-04-29
// -----------------------------------------------------------------------------
void ADN_Objects_GUI::OnSpeedImpactComboChanged()
{
    if( pSpeedImpactCombo_->currentText() == ADN_Tr::ConvertFromSpeedImpact( eSpeedImpact_VitesseMaxAgent ).c_str() )
        pMaxAgentSpeed_->setEnabled( true );
    else
        pMaxAgentSpeed_->setEnabled( false );
}

