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
#include "ADN_Equipments_Dotations_GUI.h"
#include "ADN_GoToButton.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_ListView_Units.h"
#include "ADN_MultiPercentage.h"
#include "ADN_Nature_GUI.h"
#include "ADN_Point_GUI.h"
#include "ADN_SearchListView.h"
#include "ADN_SymbolWidget.h"
#include "ADN_TimeField.h"
#include "ADN_Tr.h"
#include "ADN_UnitSymbolsComboBox.h"
#include "ADN_Units_Composantes_GUI.h"
#include "ADN_Units_Data.h"
#include "ADN_Units_Postures_GUI.h"
#include "ADN_Units_LogThreshold_GUI.h"
#include "ADN_Workspace.h"
#include "clients_kernel/SymbolFactory.h"
#include "ENT/ENT_Tr.h"
#include <numeric>
#include <boost/bind.hpp>

//-----------------------------------------------------------------------------
// Name: ADN_Units_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Units_GUI::ADN_Units_GUI( ADN_Units_Data& data )
    : ADN_GUI_ABC( "ADN_Units_GUI" )
    , data_( data )
    , pOfficersEditLine_( 0 )
    , pNCOfficersEditLine_( 0 )
    , pTypeCombo_( 0 )
    , pStockGroup_( 0 )
    , pStockLogThreshold_( 0 )
    , pInstallationGroup_( 0 )
    , pSymbolWidget_( 0 )
    , pNatureGui_( 0 )
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
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    // Name
    ADN_EditLine_String* pNameEditLine = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    connect( pNameEditLine, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNameChanged() ) );
    pNameEditLine->ConnectWithRefValidity( data_.GetUnitsInfos() );
    // Unit type
    pTypeCombo_ = builder.AddEnumField( pInfoHolder, tr( "Type" ), vInfosConnectors[ eTypeId ] );
    builder.SetToolTip( tr( "The type of unit in the simulation. This type must match the associated decisional model." ) );
    connect( pTypeCombo_, SIGNAL( activated( const QString& ) ), this, SLOT( OnTypeChanged() ) );
    // Model
    ADN_GoToButton* goToButton = new ADN_GoToButton( ::eModels, eEntityType_Pawn );
    goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, tr( "Doctrine model" ), vInfosConnectors[ eModel ], 0, eNone, goToButton ) );
    builder.SetToolTip( tr( "The decisional model associated to the unit." ) );

    // nbc suit
    builder.AddEnumField( pInfoHolder, tr( "NBC suit" ), vInfosConnectors[ eNbcSuit ] );

    // Decontamination delay
    ADN_TimeField* pTimeField = builder.AddField< ADN_TimeField >( pInfoHolder, tr( "Decontamination delay" ), vInfosConnectors[ eDecontaminationDelay ], 0, eGreaterZero );
    pTimeField->SetMinimumValueInSecond( 1 );
    // Feedback time
    builder.AddOptionnalField<ADN_TimeField>( pInfoHolder, tr( "Force ratio feedback time" ), vInfosConnectors[ eHasStrengthRatioFeedbackTime ], vInfosConnectors[ eStrengthRatioFeedbackTime ] );
    // Can fly
    builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Can fly" ), vInfosConnectors[ eCanFly ] );
    // Crossing height
    builder.AddEnumField( pInfoHolder, tr( "Crossing height" ), vInfosConnectors[ eCrossingHeight ] );
    // Is autonomous
    builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Is autonomous (UAV)" ), vInfosConnectors[ eIsAutonomous ] );
    builder.AddStretcher( pInfoHolder, Qt::Vertical );

    // Coup de sonde
    ADN_GroupBox* pReconGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Scan" ) );
    vInfosConnectors[eCanProbe] = &pReconGroup->GetConnector();
    builder.AddField< ADN_EditLine_Double >( pReconGroup, tr( "Width" ), vInfosConnectors[eProbeWidth], tr( "m" ) );
    builder.AddField< ADN_EditLine_Double >( pReconGroup, tr( "Depth" ), vInfosConnectors[eProbeLength], tr( "m" ) );

    // sensor & equipment ranges
    ADN_GroupBox* pRangeGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Ranges" ) );
    pRangeGroup->setObjectName( strClassName_ + "_Ranges" );
    vInfosConnectors[ eRanges ] = &pRangeGroup->GetConnector();
    builder.AddField< ADN_EditLine_Int >( pRangeGroup, tr( "Sensors" ), vInfosConnectors[ eSensorRange ], tr( "m" ) );
    builder.AddField< ADN_EditLine_Int >( pRangeGroup, tr( "Equipments" ), vInfosConnectors[ eEquipmentRange ], tr( "m" ) );

    // Nature group
    Q3GroupBox* pNatureGroup = new Q3GroupBox( 2, Qt::Horizontal, tr( "Nature" ) );
    {
        QGroupBox* pNatureInternalGroup = new QGroupBox( pNatureGroup );
        QGridLayout* pNatureInternalGroupLayout = new QGridLayout( pNatureInternalGroup );
        pNatureInternalGroupLayout->setMargin( 10 );
        pNatureInternalGroupLayout->setAlignment( Qt::AlignTop );
        // Level & Atlas
        Q3GroupBox* subLayout = new Q3GroupBox( 3, Qt::Horizontal );
        subLayout->setMargin( 0 );
        subLayout->setInsideMargin( 0 );
        subLayout->setFrameStyle( Q3Frame::Plain );
        builder.AddEnumField( subLayout, tr( "Level:" ), vInfosConnectors[ eNatureLevel ] );
        builder.AddEnumField( subLayout, tr( "Atlas:" ), vInfosConnectors[ eNatureAtlas ] );
        // Separator
        QFrame* separator = new QFrame;
        separator->setFrameStyle( QFrame::HLine | QFrame::Raised );
        pNatureInternalGroupLayout->addWidget( separator, 1, 0, 1, 2, Qt::AlignTop );
        // Nature GUI
        pNatureInternalGroupLayout->addWidget( subLayout, 0, 0, 1, 2, Qt::AlignTop );
        pNatureGui_ = new ADN_Nature_GUI( pNatureInternalGroupLayout, 2 );
        pNatureGui_->setObjectName( strClassName_ + "Nature" );
        vInfosConnectors[ eNatureNature ] = &pNatureGui_->GetConnector();
        // Symbol
        Q3VBox* pSymbolLayout = new Q3VBox( pNatureGroup );
        QLabel* pSymbolLabel = new QLabel( pSymbolLayout );
        pSymbolWidget_ = new ADN_SymbolWidget( pSymbolLayout );
        pSymbolWidget_->setObjectName( strClassName_ + "_Symbols" );
        pSymbolWidget_->makeCurrent();
        pSymbolWidget_->initializeGL();
        pSymbolWidget_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
        pSymbolWidget_->setMinimumSize( 130, 140 );
        pSymbolWidget_->setMaximumSize( 130, 140 );
        connect( pNatureGui_, SIGNAL( textChanged( const QString& ) ), pSymbolWidget_, SLOT( OnNatureChanged( const QString& ) ) );
        connect( pSymbolWidget_, SIGNAL( SymbolChanged( const QString& ) ), pSymbolLabel, SLOT( setText( const QString& ) ) );
        // Symbol combo
        QWidget* pHolder = builder.AddFieldHolder( pNatureGroup );
        ADN_UnitSymbolsComboBox* unitSymbolsCombo = builder.AddField< ADN_UnitSymbolsComboBox >( pHolder, tr( "UnitSymbol" ), vInfosConnectors[ eNatureSymbol ] );
        unitSymbolsCombo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
        connect( unitSymbolsCombo, SIGNAL( UnitSymbolChanged( const QString& ) ), pNatureGui_, SLOT( OnUnitSymbolChanged( const QString& ) ) );
        connect( pNatureGui_, SIGNAL( textChanged( const QString& ) ), unitSymbolsCombo, SLOT( OnNatureChanged( const QString& ) ) );
    }

    // Commandement
    Q3GroupBox* pCommandGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Command" ) );
    pCommandGroup->setInsideMargin( 20 );
    pCommandGroup->setInsideSpacing( 10 );

    // officer
    pOfficersEditLine_ = builder.AddField< ADN_EditLine_Int >( pCommandGroup, tr( "Nbr of officer(s)" ), vInfosConnectors[ eNbOfficer ] );
    pOfficersEditLine_->GetValidator().setRange( 0, 0 );
    connect( pOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfOfficersChanged() ) );

    // nc officer
    pNCOfficersEditLine_ = builder.AddField< ADN_EditLine_Int >( pCommandGroup, tr( "Nbr of warrant officer(s)" ), vInfosConnectors[ eNbNCOfficer ] );
    pNCOfficersEditLine_->GetValidator().setRange( 0, 0 );
    connect( pNCOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfNCOfficersChanged() ) );

    Q3VBox* postureInstallationBox = new Q3VBox();

    // Postures
    Q3VGroupBox* pPosturesGroup = new Q3VGroupBox( tr( "Stances" ), postureInstallationBox );
    new ADN_Units_Postures_GUI( strClassName_ + "_Postures", vInfosConnectors[ ePostures ], pPosturesGroup );

    // Installation
    pInstallationGroup_ = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Deployment" ), postureInstallationBox );
    pInstallationGroup_->setObjectName( strClassName_ + "_Installation" );
    vInfosConnectors[ eHasInstallation ] = &pInstallationGroup_->GetConnector();
    builder.AddField< ADN_TimeField >( pInstallationGroup_, tr( "Deployment duration" ), vInfosConnectors[ eInstallationDelay ] );
    builder.AddField< ADN_TimeField >( pInstallationGroup_, tr( "Un-deployment duration" ), vInfosConnectors[ eUninstallationDelay ] );

    // Distances before point on path
    Q3GroupBox* pDistancesGroup = new Q3HGroupBox( tr( "Key terrain features range" ) );
    new ADN_Point_GUI( strClassName_ + "_TerrainFeatures", vInfosConnectors[ ePointInfos ], pDistancesGroup );

    // Composantes
    Q3VGroupBox* pComposantesGroup = new Q3VGroupBox( tr( "Equipments" ) );
    ADN_Units_Composantes_GUI * pComposantes = new ADN_Units_Composantes_GUI( strClassName_ + "_Equipments", vInfosConnectors[ eComposantes ], pComposantesGroup );
    pComposantes->SetGoToOnDoubleClick( ::eEquipments );
    if( QAbstractProxyModel* model = static_cast< QAbstractProxyModel* >( pComposantes->model() ) )
        connect( model->sourceModel(), SIGNAL( itemChanged( QStandardItem* ) ), this, SLOT( OnComponentChanged() ) );

    // Dotations
    ADN_GroupBox* pDotationsGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Complementary resources" ) );
    pDotationsGroup->setObjectName( strClassName_ + "_ComplementaryResources" );
    vInfosConnectors[ eHasTC1 ] = &pDotationsGroup->GetConnector();
    ADN_Equipments_Dotations_GUI* pDotations = new ADN_Equipments_Dotations_GUI( strClassName_ + "_Dotations", vInfosConnectors[ eContenancesTC1 ], pDotationsGroup, ADN_Equipments_Dotations_GUI::eColumn_All ^ ADN_Equipments_Dotations_GUI::eColumn_Consumption, QHeaderView::ResizeToContents );
    pDotations->SetGoToOnDoubleClick( ::eResources );

    // Stock
    pStockGroup_ = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Stock" ) );
    pStockGroup_->setObjectName( strClassName_ + "_Stock" );
    vInfosConnectors[ eHasStock ] = &pStockGroup_->GetConnector();
    pStockLogThreshold_ = new ADN_Units_LogThreshold_GUI( strClassName_ + "_StockThreshold", vInfosConnectors[ eStock ], pStockGroup_ );

    // Aptitudes
    Q3GroupBox* pSkillsGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Skills" ) );
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
    Q3GroupBox* pEfficienciesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Efficiencies" ) );
    pEfficienciesGroup->setInsideMargin( 20 );
    pEfficienciesGroup->setInsideSpacing( 10 );
    builder.AddField< ADN_EditLine_Int >( pEfficienciesGroup, tr( "Urban area efficiency" ), vInfosConnectors[ eUrbanAreaEfficiency ], tr( "%" ), ePercentage );

    // Civilian
    ADN_GroupBox* pCivilianGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Civilian" ) );
    pCivilianGroup->setObjectName( strClassName_ + "_IsCivilian" );
    vInfosConnectors[ eIsCivilian ] = &pCivilianGroup->GetConnector();
    ADN_MultiPercentage_Double* pMultiPercentage = new ADN_MultiPercentage_Double( pCivilianGroup, builder, strClassName_ + "_CivilianRepartition" );
    pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMalesPercent ] );
    pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemalesPercent ] );
    pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildrenPercent ] );
    pMultiPercentage->AddWarning();

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QGridLayout* pContentLayout = new QGridLayout( pContent, 7, 6, 5 );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addMultiCellWidget( pInfoHolder, 0, 0, 0, 2 );
    pContentLayout->addMultiCellWidget( pNatureGroup, 0, 0, 3, 5 );
    pContentLayout->addMultiCellWidget( pComposantesGroup, 1, 3, 0, 3 );
    pContentLayout->addMultiCellWidget( pCommandGroup, 1, 1, 4, 5 );
    pContentLayout->addMultiCellWidget( pReconGroup, 2, 2, 4, 5 );
    pContentLayout->addMultiCellWidget( pRangeGroup, 3, 3, 4, 5 );
    pContentLayout->addMultiCellWidget( pDistancesGroup, 4, 4, 0, 1 );
    pContentLayout->addMultiCellWidget( postureInstallationBox, 4, 4, 2, 3 );
    pContentLayout->addMultiCellWidget( pDotationsGroup, 5, 6, 0, 1 );
    pContentLayout->addMultiCellWidget( pStockGroup_, 5, 6, 2, 3 );
    pContentLayout->addMultiCellWidget( pSkillsGroup, 4, 4, 4, 5 );
    pContentLayout->addMultiCellWidget( pEfficienciesGroup, 5, 5, 4, 5 );
    pContentLayout->addMultiCellWidget( pCivilianGroup, 6, 6, 4, 5 );

    // List view
    ADN_SearchListView< ADN_ListView_Units >* pSearchListView = new ADN_SearchListView< ADN_ListView_Units >( this, data_.GetUnitsInfos(), vInfosConnectors );
    pListView_ = pSearchListView->GetListView();
    pListView_->setObjectName( strClassName_ + "_List" );
    connect( pListView_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnTypeChanged() ) );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
    pMainWidget_->setObjectName( strClassName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::OnNameChanged
// Created: MMC 2012-09-05
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnNameChanged()
{
    UpdateValidators();
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
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGTC2, ENT_Tr_ABC::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionOrganization, ENT_Tr_ABC::eToTr ).c_str() )
    {
        // insert stock category list if not exist yet
        ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListView_->GetCurrentData();
        if( pInfos == 0 )
            return;
        if( pInfos->stocks_.vLogThresholds_.size() == 0 )
        {
            //pStockLogThreshold_->InitializeLogThresholds();
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
    ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListView_->GetCurrentData();
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
    ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListView_->GetCurrentData();
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
    ADN_Units_Data::UnitInfos* pInfos = (ADN_Units_Data::UnitInfos*)pListView_->GetCurrentData();
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
    ADN_Tools::CreatePathToFile( strLocalPath.toStdString() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Units" ) );

    ADN_Units_Data::T_UnitInfos_Vector& units = data_.GetUnitsInfos();
    indexBuilder.BeginTable( static_cast< int >( units.size()+1 ), 2 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ).toStdString().c_str(), true );
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

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::SetSymbolFactory
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
void ADN_Units_GUI::SetSymbolFactory( kernel::SymbolFactory& factory )
{
    if( pSymbolWidget_ )
    {
        pSymbolWidget_->SetSymbolFactory( factory );
        assert( pNatureGui_ );
        if( factory.GetSymbolRule() )
            pNatureGui_->SetRootSymbolRule( *factory.GetSymbolRule() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::IsSymbolAvailable
// Created: MMC 2011-07-06
// -----------------------------------------------------------------------------
bool ADN_Units_GUI::IsSymbolAvailable( const std::string& symbol )
{
    if( pSymbolWidget_ )
        return pSymbolWidget_->IsAvailable( symbol );

    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::PreloadUnitSymbolComboBox
// Created: MMC 2011-07-06
// -----------------------------------------------------------------------------
void ADN_Units_GUI::PreloadUnitSymbolComboBox( ADN_Units_Data::UnitInfos* pValidUnitInfos )
{
    if( !pListView_ )
        return;
    if( ADN_ListView_Units* unitListView = dynamic_cast< ADN_ListView_Units* >( pListView_ ) )
        unitListView->ConnectNatureSymbol( pValidUnitInfos );
}
