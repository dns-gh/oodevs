//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_GUI.cpp $
// $Author: Ape $
// $Modtime: 22/06/05 18:36 $
// $Revision: 20 $
// $Workfile: ADN_Units_GUI.cpp $
//
//*****************************************************************************
#include "ADN_pch.h"
#include "ADN_Units_GUI.h"
#include "moc_ADN_Units_GUI.cpp"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_Units_Data.h"
#include "ADN_CommonGfx.h"
#include "ADN_ListView_Units.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Units_Composantes_GUI.h"
#include "ADN_Units_Postures_GUI.h"
#include "ADN_Point_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "ADN_GuiBuilder.h"
#include "ADN_TimeField.h"

#include <qcombo.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qwhatsthis.h>
#include <qtooltip.h>


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Units_GUI::ADN_Units_GUI( ADN_Units_Data& data )
: ADN_GUI_ABC( "ADN_Units_GUI" )
, data_      ( data )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Objects_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Units_GUI::~ADN_Units_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_GUI::Build
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
void ADN_Units_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget
    pMainWidget_ = new QWidget( 0 );

    // Unit listview
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );
    pListUnits_ = new ADN_ListView_Units( pMainWidget_ );
    connect( pListUnits_, SIGNAL( selectionChanged() ), this, SLOT( OnTypeChanged() ) );
    pListUnits_->GetConnector().Connect( &data_.GetUnitsInfos() );    

    // Unit data
    QGroupBox* pGroup = new QGroupBox( 0, Qt::Vertical, tr( "Unit" ), pMainWidget_ );

    ///////////////////
    // Unit parameters
    QWidget* pParamGroup = builder.AddFieldHolder( pGroup );

    // Name
    builder.AddField<ADN_EditLine_String>( pParamGroup, tr( "Name" ), vInfosConnectors[eName] );

    // Unit type
    pTypeCombo_ = builder.AddEnumField<E_AgentTypePion>( pParamGroup, tr( "Type" ), vInfosConnectors[eTypeId], &ADN_Tr::ConvertFromAgentTypePion );
    builder.SetToolTip( "Le type de l'unité dans la simulation. Ce type doit correspondre au type du modèle comportemental associé." );
    connect( pTypeCombo_, SIGNAL( activated( const QString& ) ), this, SLOT( OnTypeChanged() ) );

    // Model
    builder.AddField< ADN_ComboBox_Vector<ADN_Models_Data::ModelInfos> >( pParamGroup, tr( "Model" ), vInfosConnectors[eModel] );
    builder.SetToolTip( "Le modèle comportemental associé à l'unité." );

    // Decontamination delay
    builder.AddField<ADN_TimeField>( pParamGroup, tr( "Decontamination delay" ), vInfosConnectors[eDecontaminationDelay] );

    // Sensors reach
    builder.AddField<ADN_EditLine_Double>( pParamGroup, tr( "Sensors reach" ), vInfosConnectors[eSensorsReach], tr( "m" ) );

    // Weapons reach
    builder.AddField<ADN_EditLine_Double>( pParamGroup, tr( "Weapons reach" ), vInfosConnectors[eWeaponsReach], tr( "m" ) );

    // Feedback time
    builder.AddOptionnalField<ADN_TimeField>( pParamGroup, tr( "Force ratio feedback time" ), vInfosConnectors[eHasStrengthRatioFeedbackTime], vInfosConnectors[eStrengthRatioFeedbackTime] );

    // Can fly
    builder.AddField<ADN_CheckBox>( pParamGroup, tr( "Can fly" ), vInfosConnectors[eCanFly] );

    // Is autonomous
    builder.AddField<ADN_CheckBox>( pParamGroup, tr( "Is autonomous (UAV)" ), vInfosConnectors[eIsAutonomous] );

    // Coup de sonde
    ADN_GroupBox* pReconGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Scan" ), pGroup );
    vInfosConnectors[eCanProbe] = &pReconGroup->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pReconGroup, tr( "Width" ), vInfosConnectors[eProbeWidth], tr( "m" ) );
    builder.AddField<ADN_EditLine_Double>( pReconGroup, tr( "Depth" ), vInfosConnectors[eProbeLength], tr( "m" ) );


    // Nature
    QGroupBox* pNatureGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Nature" ), pGroup ); 
    pNatureGroup->setInsideMargin(20);
    pNatureGroup->setInsideSpacing(10);

    // nature level type
    builder.AddEnumField<E_NatureLevel>( pNatureGroup, tr( "Level" ), vInfosConnectors[eNatureLevel], ENT_Tr::ConvertFromNatureLevel );

    // nature weapon type
    builder.AddEnumField<E_UnitNatureWeapon>( pNatureGroup, tr( "Weapon" ), vInfosConnectors[eNatureWeapon], ENT_Tr::ConvertFromUnitNatureWeapon );

    // nature specialization type
    builder.AddEnumField<E_UnitNatureSpecialization>( pNatureGroup, tr( "Specialization" ), vInfosConnectors[eNatureSpec], ENT_Tr::ConvertFromUnitNatureSpecialization );

    // nature qualifier type
    builder.AddEnumField<E_UnitNatureQualifier>( pNatureGroup, tr( "Qualifier" ), vInfosConnectors[eNatureQualifer], ENT_Tr::ConvertFromUnitNatureQualifier );

    // nature category type
    builder.AddEnumField<E_UnitNatureCategory>( pNatureGroup, tr( "Category" ), vInfosConnectors[eNatureCategory], ENT_Tr::ConvertFromUnitNatureCategory );

    // nature mobility type
    builder.AddEnumField<E_UnitNatureMobility>( pNatureGroup, tr( "Mobility" ), vInfosConnectors[eNatureMobility], ENT_Tr::ConvertFromUnitNatureMobility );

    // nature atlas type
    builder.AddEnumField<E_NatureAtlasType>( pNatureGroup, tr( "Atlas" ), vInfosConnectors[eNatureAtlas], ADN_Tr::ConvertFromNatureAtlasType );

    // Mission capacity
    builder.AddEnumField<E_CapacityMission>( pNatureGroup, tr( "Mission capacity" ), vInfosConnectors[eMissionCapacity], ADN_Tr::ConvertFromCapacityMission );


    // Commandement
    QGroupBox* pCommandGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Command" ), pGroup ); 
    pCommandGroup->setInsideMargin(20);
    pCommandGroup->setInsideSpacing(10);

    // officer
    pOfficersEditLine_ = builder.AddField<ADN_EditLine_Int>( pCommandGroup, tr( "Nbr of officer(s)" ), vInfosConnectors[eNbOfficer] );
    connect( pOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfOfficersChanged() ) );

    // nc officer
    pNCOfficersEditLine_ = builder.AddField<ADN_EditLine_Int>( pCommandGroup, tr( "Nbr of NC officer(s)" ), vInfosConnectors[eNbNCOfficer] );
    connect( pNCOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfNCOfficersChanged() ) );

    // Postures
    QVGroupBox* pPosturesGroup = new QVGroupBox( tr( "Stances" ), pGroup );
    ADN_Units_Postures_GUI* pPostures = new ADN_Units_Postures_GUI( pPosturesGroup );
    vInfosConnectors[ePostures] = &pPostures->GetConnector();


    // Distances before point on path
    QGroupBox* pDistancesGroup = new QHGroupBox( tr( "Distances before point" ), pGroup );
    ADN_Point_GUI* pSensors = new ADN_Point_GUI( pDistancesGroup );
    vInfosConnectors[ePointInfos] = &pSensors->GetConnector();


    // Composantes
    QVGroupBox* pComposantesGroup = new QVGroupBox( tr( "Composantes" ), pGroup );
    ADN_Units_Composantes_GUI * pComposantes = new ADN_Units_Composantes_GUI( pComposantesGroup );
    vInfosConnectors[eComposantes] = &pComposantes->GetConnector();
    connect( pComposantes, SIGNAL( valueChanged ( int, int ) ), this, SLOT( OnNbrOfOfficersChanged() ) );
    connect( pComposantes, SIGNAL( currentChanged ( int, int ) ), this, SLOT( OnNbrOfOfficersChanged() ) );

    // Dotations
    ADN_GroupBox* pDotationsGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Complementary Dotations" ), pGroup );
    vInfosConnectors[eHasTC1] = &pDotationsGroup->GetConnector();
    ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( true, pDotationsGroup );
    vInfosConnectors[eContenancesTC1] = &pDotations->GetConnector();


    // Stock
    pStockGroup_ = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Stock" ), pGroup );
    vInfosConnectors[eHasStock] = &pStockGroup_->GetConnector();
    ADN_Composantes_Dotations_GUI* pStock = new ADN_Composantes_Dotations_GUI( false, pStockGroup_ );
    vInfosConnectors[eStock] = &pStock->GetConnector();


    // set list units auto connectors
    pListUnits_->SetItemConnectors( vInfosConnectors );    

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pListUnits_, 1 );
    pMainLayout->addWidget( pGroup, 4 );

    QGridLayout* pGroupLayout = new QGridLayout( pGroup->layout(), 1, 2, 5 );
    pGroupLayout->setAlignment( Qt::AlignTop );
    pGroupLayout->addWidget( pParamGroup, 0, 0 );
    pGroupLayout->addWidget( pNatureGroup,  0, 1 );
    pGroupLayout->addWidget( pReconGroup, 1, 0 );
    pGroupLayout->addWidget( pCommandGroup,  1, 1 );

    QHBoxLayout* pSubLayout = new QHBoxLayout( 5 );
    pSubLayout->addWidget( pDistancesGroup, 10 );
    pSubLayout->addWidget( pPosturesGroup, 10 );
    pSubLayout->addWidget( pComposantesGroup, 15 );

    QHBoxLayout* pSubLayout2 = new QHBoxLayout( 5 );
    pSubLayout2->addWidget( pDotationsGroup, 12 );
    pSubLayout2->addWidget( pStockGroup_, 10 );

    pGroupLayout->addMultiCellLayout( pSubLayout, 2, 2, 0, 1 );
    pGroupLayout->addMultiCellLayout( pSubLayout2, 3, 3, 0, 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::OnTypeChanged
// Created: APE 2005-06-01
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnTypeChanged()
{
    QString strType = pTypeCombo_->currentText();
    if(   strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLDRavitaillement ).c_str() 
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLTRavitaillement ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGConvoi ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGTC2 ).c_str() )
        pStockGroup_->setEnabled( true );
    else
        pStockGroup_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::OnNbrOfOfficersChanged
// Created: APE 2005-06-22
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnNbrOfOfficersChanged()
{
    ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListUnits_->GetCurrentData();
    if( pInfos == 0 )
        return;

    int nNbrHumans = 0;
    for( ADN_Units_Data::IT_ComposanteInfos_Vector it = pInfos->vComposantes_.begin(); it != pInfos->vComposantes_.end(); ++it )
    {
        nNbrHumans += (*it)->nNb_.GetData() * (*it)->ptrComposante_.GetData()->nTroopTransportCapacity_.GetData();
    }

    if( pInfos->nNbOfficer_.GetData() + pInfos->nNbNCOfficer_.GetData() > nNbrHumans )
        pInfos->nNbOfficer_ = std::max( 0, nNbrHumans - pInfos->nNbNCOfficer_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::OnNbrOfNCOfficersChanged
// Created: APE 2005-06-22
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnNbrOfNCOfficersChanged()
{
    ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListUnits_->GetCurrentData();
    if( pInfos == 0 )
        return;

    int nNbrHumans = 0;
    for( ADN_Units_Data::IT_ComposanteInfos_Vector it = pInfos->vComposantes_.begin(); it != pInfos->vComposantes_.end(); ++it )
    {
        nNbrHumans += (*it)->nNb_.GetData() * (*it)->ptrComposante_.GetData()->nTroopTransportCapacity_.GetData();
    }

    if( pInfos->nNbOfficer_.GetData() + pInfos->nNbNCOfficer_.GetData() > nNbrHumans )
        pInfos->nNbNCOfficer_ = std::max( 0, nNbrHumans - pInfos->nNbOfficer_.GetData() );
}
