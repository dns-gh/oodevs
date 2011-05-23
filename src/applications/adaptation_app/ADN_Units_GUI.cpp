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
#include "ADN_ComboBox_Vector.h"
#include "ADN_CommonGfx.h"
#include "ADN_Composantes_Dotations_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_ListView_Units.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Nature_GUI.h"
#include "ADN_Point_GUI.h"
#include "ADN_SymbolWidget.h"
#include "ADN_TimeField.h"
#include "ADN_Tr.h"
#include "ADN_Units_Composantes_GUI.h"
#include "ADN_Units_Data.h"
#include "ADN_Units_Postures_GUI.h"
#include "ADN_Units_LogThreshold_GUI.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"
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
    builder.SetToolTip( "Le type de l'unité dans la simulation. Ce type doit correspondre au type du modèle comportemental associé." );
    connect( pTypeCombo_, SIGNAL( activated( const QString& ) ), this, SLOT( OnTypeChanged() ) );
    // Model
    builder.AddField< ADN_ComboBox_Vector<ADN_Models_Data::ModelInfos> >( pParamGroup, tr( "Doctrine model" ), vInfosConnectors[eModel] );
    builder.SetToolTip( "Le modèle comportemental associé à l'unité." );
    // Decontamination delay
    ADN_TimeField* pTimeField = builder.AddField<ADN_TimeField>( pParamGroup, tr( "Decontamination delay" ), vInfosConnectors[eDecontaminationDelay], 0, eGreaterZero );
    pTimeField->SetMinimumValueInSecond( 1 );
    // Feedback time
    builder.AddOptionnalField<ADN_TimeField>( pParamGroup, tr( "Force ratio feedback time" ), vInfosConnectors[eHasStrengthRatioFeedbackTime], vInfosConnectors[eStrengthRatioFeedbackTime] );
    // Can fly
    builder.AddField<ADN_CheckBox>( pParamGroup, tr( "Can fly" ), vInfosConnectors[eCanFly] );
    // Crossing height
    builder.AddEnumField< E_CrossingHeight >( pParamGroup, tr( "Crossing height" ), vInfosConnectors[ eCrossingHeight ], &ADN_Tr::ConvertFromCrossingHeight );
    // Is autonomous
    builder.AddField<ADN_CheckBox>( pParamGroup, tr( "Is autonomous (UAV)" ), vInfosConnectors[eIsAutonomous] );

    // Coup de sonde
    ADN_GroupBox* pReconGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Scan" ), pGroup );
    vInfosConnectors[eCanProbe] = &pReconGroup->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pReconGroup, tr( "Width" ), vInfosConnectors[eProbeWidth], tr( "m" ) );
    builder.AddField<ADN_EditLine_Double>( pReconGroup, tr( "Depth" ), vInfosConnectors[eProbeLength], tr( "m" ) );

    // Nature
    QGroupBox* pNatureGroup = new QGroupBox( 2, Qt::Horizontal, tr( "Nature" ), pGroup );

    QGroupBox* pNatureInternalGroup = new QGroupBox( 2, Qt::Vertical, pNatureGroup );
    //pNatureInternalGroup->setFrameStyle( QFrame::NoFrame );
    QGroupBox* subLayout = new QGroupBox( 3, Qt::Horizontal, pNatureInternalGroup );
    subLayout->setInsideMargin( 0 );
    subLayout->setInsideSpacing( 0 );
    subLayout->setFrameStyle( QFrame::Plain );

    builder.AddEnumField<E_NatureLevel>( subLayout, tr( "Level" ), vInfosConnectors[eNatureLevel], ENT_Tr::ConvertFromNatureLevel );

    // nature atlas type
    builder.AddEnumField<E_NatureAtlasType>( subLayout, tr( "Atlas" ), vInfosConnectors[eNatureAtlas], ADN_Tr::ConvertFromNatureAtlasType );

    ADN_Nature_GUI* natureGui = new ADN_Nature_GUI( pNatureInternalGroup );
    vInfosConnectors[eNatureNature] = &natureGui->GetConnector();

    // Symbol
    QVBox* pSymbolLayout = new QVBox( pNatureGroup );
    QLabel* pSymbolLabel = new QLabel( pSymbolLayout );
    pSymbolWidget_ = new ADN_SymbolWidget( pSymbolLayout );
    pSymbolWidget_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    pSymbolWidget_->setMinimumSize( 130, 140 );
    pSymbolWidget_->setMaximumSize( 130, 140 );
    connect( natureGui, SIGNAL( textChanged( const QString& ) ), pSymbolWidget_, SLOT( OnNatureChanged( const QString& ) ) );
    connect( pSymbolWidget_, SIGNAL( SymbolChanged( const QString& ) ), pSymbolLabel, SLOT( setText( const QString& ) ) );

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

    // Aptitudes
    QGroupBox* pSkillsGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Skills" ), pGroup );
    pSkillsGroup->setInsideMargin( 20 );
    pSkillsGroup->setInsideSpacing( 10 );

    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, tr( "Recon" ), vInfosConnectors[ eRecon ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, tr( "Combat support" ), vInfosConnectors[ eCombatSupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, tr( "Combat" ), vInfosConnectors[ eCombat ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, tr( "Mobility support" ), vInfosConnectors[ eMobilitySupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, tr( "Counter mobility support" ), vInfosConnectors[ eCounterMobilitySupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, tr( "Protection support" ), vInfosConnectors[ eProtectionSupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, tr( "Engineering recon" ), vInfosConnectors[ eEngineeringRecon ], tr( "%" ), ePercentage );

    // Efficiencies
    QGroupBox* pEfficienciesGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Efficiencies" ), pGroup );
    pEfficienciesGroup->setInsideMargin( 20 );
    pEfficienciesGroup->setInsideSpacing( 10 );
    builder.AddField< ADN_EditLine_Int >( pEfficienciesGroup, tr( "Urban area efficiency" ), vInfosConnectors[ eUrbanAreaEfficiency ], tr( "%" ), ePercentage );

    // Civilian
    ADN_GroupBox* pCivilianGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Civilian" ), pGroup );
    vInfosConnectors[ eIsCivilian ] = &pCivilianGroup->GetConnector();
    ADN_MultiPercentage* pMultiPercentage = new ADN_MultiPercentage( pCivilianGroup, builder );
    pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMalesPercent ] );
    pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemalesPercent ] );
    pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildrenPercent ] );
    pMultiPercentage->AddWarning();
    connect( pCivilianGroup, SIGNAL( toggled( bool ) ), pMultiPercentage, SLOT( PercentageChanged() ) );

    // set list units auto connectors
    pListUnits_->SetItemConnectors( vInfosConnectors );

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10 );
    pMainLayout->addWidget( pListUnits_, 1 );
    pMainLayout->addWidget( pGroup, 6 );

    QGridLayout* pGroupLayout = new QGridLayout( pGroup->layout(), 6, 6, 5 );
    pGroupLayout->setAlignment( Qt::AlignTop );
    pGroupLayout->addMultiCellWidget( pParamGroup, 0, 0, 0, 2 );
    pGroupLayout->addMultiCellWidget( pNatureGroup, 0, 0, 3, 5 );
    pGroupLayout->addMultiCellWidget( pDistancesGroup, 1, 2, 0, 1 );
    pGroupLayout->addMultiCellWidget( postureInstallationBox, 1, 2, 2, 3 );
    pGroupLayout->addMultiCellWidget( pReconGroup, 1, 1, 4, 5 );
    pGroupLayout->addMultiCellWidget( pCommandGroup, 2, 2, 4, 5 );
    pGroupLayout->addMultiCellWidget( pDotationsGroup, 3, 4, 0, 1 );
    pGroupLayout->addMultiCellWidget( pStockGroup_, 3, 4, 2, 3 );
    pGroupLayout->addMultiCellWidget( pSkillsGroup, 3, 3, 4, 5 );
    pGroupLayout->addMultiCellWidget( pEfficienciesGroup, 4, 4, 4, 5 );
    pGroupLayout->addMultiCellWidget( pComposantesGroup, 5, 5, 0, 3 );
    pGroupLayout->addMultiCellWidget( pCivilianGroup, 5, 5, 4, 5 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::OnTypeChanged
// Created: APE 2005-06-01
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnTypeChanged()
{
    QString strType = pTypeCombo_->currentText();
    if( strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLDRavitaillement, ENT_Tr_ABC::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLTRavitaillement, ENT_Tr_ABC::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGConvoi, ENT_Tr_ABC::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGTC2, ENT_Tr_ABC::eToTr ).c_str() )
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

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::ExportHtml
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void ADN_Units_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    QString strLocalPath = strPath + tr( "Units/" );
    ADN_Tools::CreatePathToFile( strLocalPath.ascii() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Units" ) );

    ADN_Units_Data::T_UnitInfos_Vector& units = data_.GetUnitsInfos();
    indexBuilder.BeginTable( static_cast< int >( units.size()+1 ), 2 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ), true );
    int n = 1;
    for( ADN_Units_Data::IT_UnitInfos_Vector it = units.begin(); it != units.end(); ++it, ++n )
    {
        ADN_Units_Data::UnitInfos& unit = **it;
        indexBuilder.TableItem( n, 0, unit.strName_.GetData().c_str() );
        indexBuilder.TableItem( n, 1, ADN_Tr::ConvertFromAgentTypePion( unit.eTypeId_.GetData() ).c_str() );

    }
    indexBuilder.EndTable();
    indexBuilder.WriteToFile( strLocalPath + "index.htm" );

    QString strText = "<a href=\"" + tr( "Units/" ) + "index.htm\">" + tr( "Units" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::GetSymbolWidget
// Created: RPD 2011-04-08
// -----------------------------------------------------------------------------
ADN_SymbolWidget* ADN_Units_GUI::GetSymbolWidget() const
{
    return pSymbolWidget_;
}