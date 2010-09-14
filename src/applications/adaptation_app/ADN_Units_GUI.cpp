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
#include "adaptation_app_pch.h"
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
#include "ADN_Units_LogThreshold_GUI.h"
#include "ADN_Point_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "ADN_GuiBuilder.h"
#include "ADN_TimeField.h"
#include "ADN_Nature_GUI.h"
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
#include <numeric>
#include <boost/bind.hpp>

//-----------------------------------------------------------------------------
// Name: ADN_Units_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Units_GUI::ADN_Units_GUI( ADN_Units_Data& data )
    : ADN_GUI_ABC( "ADN_Units_GUI" )
    , data_( data )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Units_GUI::~ADN_Units_GUI()
{
    // NOTHING
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

    // Unit parameters
    QWidget* pParamGroup = builder.AddFieldHolder( pGroup );

    // Name
    builder.AddField<ADN_EditLine_String>( pParamGroup, tr( "Name" ), vInfosConnectors[eName] );

    // Unit type
    pTypeCombo_ = builder.AddEnumField<E_AgentTypePion>( pParamGroup, tr( "Type" ), vInfosConnectors[eTypeId], &ADN_Tr::ConvertFromAgentTypePion );
    builder.SetToolTip( "Le type de l'unit� dans la simulation. Ce type doit correspondre au type du mod�le comportemental associ�." );
    connect( pTypeCombo_, SIGNAL( activated( const QString& ) ), this, SLOT( OnTypeChanged() ) );

    // Model
    builder.AddField< ADN_ComboBox_Vector<ADN_Models_Data::ModelInfos> >( pParamGroup, tr( "Doctrine model" ), vInfosConnectors[eModel] );
    builder.SetToolTip( "Le mod�le comportemental associ� � l'unit�." );

    // Decontamination delay
    builder.AddField<ADN_TimeField>( pParamGroup, tr( "Decontamination delay" ), vInfosConnectors[eDecontaminationDelay], 0, eGreaterZero );

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
    QGroupBox* pNatureGroup = new QGroupBox( 2, Qt::Vertical, tr( "Nature" ), pGroup );
    QGroupBox* subLayout = new QGroupBox( 3, Qt::Horizontal, pNatureGroup );
    subLayout->setInsideMargin( 0 );
    subLayout->setInsideSpacing( 0 );
    subLayout->setFrameStyle( QFrame::Plain );

    builder.AddEnumField<E_NatureLevel>( subLayout, tr( "Level" ), vInfosConnectors[eNatureLevel], ENT_Tr::ConvertFromNatureLevel );

    // nature atlas type
    builder.AddEnumField<E_NatureAtlasType>( subLayout, tr( "Atlas" ), vInfosConnectors[eNatureAtlas], ADN_Tr::ConvertFromNatureAtlasType );

    ADN_Nature_GUI* natureGui = new ADN_Nature_GUI( pNatureGroup );
    vInfosConnectors[eNatureNature] = &natureGui->GetConnector();

    // Commandement
    QGroupBox* pCommandGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Command" ), pGroup );
    pCommandGroup->setInsideMargin(20);
    pCommandGroup->setInsideSpacing(10);

    // officer
    pOfficersEditLine_ = builder.AddField<ADN_EditLine_Int>( pCommandGroup, tr( "Nbr of officer(s)" ), vInfosConnectors[eNbOfficer] );
    pOfficersEditLine_->GetValidator().setRange( 0, 0 );
    connect( pOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfOfficersChanged() ) );

    // nc officer
    pNCOfficersEditLine_ = builder.AddField<ADN_EditLine_Int>( pCommandGroup, tr( "Nbr of warrant officer(s)" ), vInfosConnectors[eNbNCOfficer] );
    pNCOfficersEditLine_->GetValidator().setRange( 0, 0 );
    connect( pNCOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfNCOfficersChanged() ) );

    QVBox* postureInstallationBox = new QVBox( pGroup );

    // Postures
    QVGroupBox* pPosturesGroup = new QVGroupBox( tr( "Stances" ), postureInstallationBox );
    ADN_Units_Postures_GUI* pPostures = new ADN_Units_Postures_GUI( pPosturesGroup );
    vInfosConnectors[ePostures] = &pPostures->GetConnector();

    // Installation
    pInstallationGroup_ = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Deployment" ), postureInstallationBox );
    vInfosConnectors[eHasInstallation] = &pInstallationGroup_->GetConnector();
    builder.AddField<ADN_TimeField>( pInstallationGroup_, tr( "Deployment duration" ), vInfosConnectors[eInstallationDelay] );
    builder.AddField<ADN_TimeField>( pInstallationGroup_, tr( "Un-deployment duration" ), vInfosConnectors[eUninstallationDelay] );

    // Distances before point on path
    QGroupBox* pDistancesGroup = new QHGroupBox( tr( "Key terrain features range" ), pGroup );
    ADN_Point_GUI* pSensors = new ADN_Point_GUI( pDistancesGroup );
    vInfosConnectors[ePointInfos] = &pSensors->GetConnector();

    // Composantes
    QVGroupBox* pComposantesGroup = new QVGroupBox( tr( "Equipments" ), pGroup );
    ADN_Units_Composantes_GUI * pComposantes = new ADN_Units_Composantes_GUI( pComposantesGroup );
    vInfosConnectors[eComposantes] = &pComposantes->GetConnector();
    connect( pComposantes, SIGNAL( valueChanged ( int, int ) ), this, SLOT( OnComponentChanged() ) );
    connect( pComposantes, SIGNAL( currentChanged ( int, int ) ), this, SLOT( OnComponentChanged() ) );
    connect( pComposantes, SIGNAL( contextMenuRequested ( int, int, const QPoint& ) ), this, SLOT( OnComponentChanged() ) );

    // Dotations
    ADN_GroupBox* pDotationsGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Complementary resources" ), pGroup );
    vInfosConnectors[eHasTC1] = &pDotationsGroup->GetConnector();
    ADN_Composantes_Dotations_GUI* pDotations = new ADN_Composantes_Dotations_GUI( false, pDotationsGroup );
    vInfosConnectors[eContenancesTC1] = &pDotations->GetConnector();

    // Stock
    pStockGroup_ = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Stock" ), pGroup );
    vInfosConnectors[eHasStock] = &pStockGroup_->GetConnector();
    pStockLogThreshold_ = new ADN_Units_LogThreshold_GUI( pStockGroup_ );
    vInfosConnectors[eStock] = &pStockLogThreshold_->GetConnector();

    // set list units auto connectors
    pListUnits_->SetItemConnectors( vInfosConnectors );

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10 );
    pMainLayout->addWidget( pListUnits_, 1 );
    pMainLayout->addWidget( pGroup, 6 );

    QGridLayout* pGroupLayout = new QGridLayout( pGroup->layout(), 4, 6, 5 );
    pGroupLayout->setAlignment( Qt::AlignTop );
    pGroupLayout->addMultiCellWidget( pParamGroup, 0, 0, 0, 2 );
    pGroupLayout->addMultiCellWidget( pNatureGroup, 0, 0, 3, 5 );
    pGroupLayout->addMultiCellWidget( pReconGroup, 1, 1, 0, 2 );
    pGroupLayout->addMultiCellWidget( pCommandGroup, 1, 1, 3, 5 );
    pGroupLayout->addMultiCellWidget( pDistancesGroup, 2, 2, 0, 1 );
    pGroupLayout->addMultiCellWidget( postureInstallationBox, 2, 2, 2, 3 );
    pGroupLayout->addMultiCellWidget( pComposantesGroup, 2, 2, 4, 5 );
    pGroupLayout->addMultiCellWidget( pDotationsGroup, 3, 3, 0, 2 );
    pGroupLayout->addMultiCellWidget( pStockGroup_, 3, 3, 3, 5 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::OnTypeChanged
// Created: APE 2005-06-01
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnTypeChanged()
{
    QString strType = pTypeCombo_->currentText();
    if( strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLDRavitaillement ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLTRavitaillement ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGConvoi ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGTC2 ).c_str() )
    {
        // insert stock category list if not exist yet
        ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListUnits_->GetCurrentData();
        if( pInfos == 0 )
            return;
        if( pInfos->stocks_.vLogThresholds_.size() == 0 )
        {
            pStockLogThreshold_->InitializeLogThresholds();
            //pInfos->stocks_.InitializeLogThresholds();
        }
        pStockGroup_->setEnabled( true );
    }
    else
        pStockGroup_->setEnabled( false );
}

namespace
{
    unsigned int Compute( unsigned int result, ADN_Units_Data::ComposanteInfos* i )
    {
        return result + i->nNb_.GetData() * i->nNbrHumanInCrew_.GetData();
    }
    unsigned int GetCapacity( ADN_Units_Data::UnitInfos& infos )
    {
        return std::accumulate( infos.vComposantes_.begin(), infos.vComposantes_.end(), 0u, boost::bind( &Compute, _1, _2 ) );
    }
    void UpdateOfficers( ADN_Type_Int& value, ADN_Type_Int& complement, int capacity )
    {
        if( value.GetData() + complement.GetData() > capacity )
            value = std::max( 0, capacity - complement.GetData() );
    }
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
    UpdateOfficers( pInfos->nNbOfficer_, pInfos->nNbNCOfficer_, GetCapacity( *pInfos ) );
    UpdateValidators();
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
    UpdateOfficers( pInfos->nNbNCOfficer_, pInfos->nNbOfficer_, GetCapacity( *pInfos ) );
    UpdateValidators();
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::OnComponentChanged
// Created: LGY 2010-07-28
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnComponentChanged()
{
    OnNbrOfNCOfficersChanged();
    OnNbrOfOfficersChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::UpdateValidators
// Created: LGY 2010-07-28
// -----------------------------------------------------------------------------
void ADN_Units_GUI::UpdateValidators()
{
    ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListUnits_->GetCurrentData();
    if( pInfos == 0 )
        return;
    pNCOfficersEditLine_->GetValidator().setTop( GetCapacity( *pInfos ) - pInfos->nNbOfficer_.GetData() );
    pOfficersEditLine_->GetValidator().setTop( GetCapacity( *pInfos ) - pInfos->nNbNCOfficer_.GetData() );
}
