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
#include "ADN_UnitSymbolWidget.h"
#include "ADN_TimeField.h"
#include "ADN_Tr.h"
#include "ADN_UnitSymbolsComboBox.h"
#include "ADN_Units_Composantes_GUI.h"
#include "ADN_Units_Data.h"
#include "ADN_Units_Postures_GUI.h"
#include "ADN_Units_LogThreshold_GUI.h"
#include "clients_kernel/SymbolFactory.h"
#include "ENT/ENT_Tr.h"
#include <numeric>
#include <boost/bind.hpp>

//-----------------------------------------------------------------------------
// Name: ADN_Units_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Units_GUI::ADN_Units_GUI( ADN_Units_Data& data )
    : ADN_GUI_ABC( eUnits )
    , data_( data )
    , pOfficersEditLine_( 0 )
    , pNCOfficersEditLine_( 0 )
    , pTypeCombo_( 0 )
    , pStockGroup_( 0 )
    , pStockLogThreshold_( 0 )
    , pInstallationGroup_( 0 )
    , pNatureGui_( 0 )
    , labelSymbol_( 0 )
    , labelMoveSymbol_( 0 )
    , labelStaticSymbol_( 0 )
    , unitSymbolWidget_( 0 )
    , unitSymbolMoveWidget_( 0 )
    , unitSymbolStaticWidget_( 0 )
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
    ADN_EditLine_ABC* pNameEditLine = builder.AddLocalizedField( data_.GetUnitsInfos(), pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
    connect( pNameEditLine, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNameChanged() ) );
    // Unit type
    pTypeCombo_ = builder.AddEnumField( pInfoHolder, "type", tr( "Type" ), vInfosConnectors[ eTypeId ] );
    builder.SetToolTip( tr( "The type of unit in the simulation. This type must match the associated decisional model." ) );
    connect( pTypeCombo_, SIGNAL( activated( const QString& ) ), this, SLOT( OnTypeChanged() ) );
    // Model
    ADN_GoToButton* goToButton = new ADN_GoToButton( ::eModels, eEntityType_Pawn );
    goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "doctrine-model", tr( "Doctrine model" ), vInfosConnectors[ eModel ], 0, eNone, goToButton ) );
    builder.SetToolTip( tr( "The decisional model associated to the unit." ) );

    // nbc suit
    builder.AddEnumField( pInfoHolder, "nbc-suit", tr( "NBC suit" ), vInfosConnectors[ eNbcSuit ] );

    // Decontamination delay
    ADN_TimeField* pTimeField = builder.AddField< ADN_TimeField >( pInfoHolder, "decontamination-delay", tr( "Decontamination delay" ), vInfosConnectors[ eDecontaminationDelay ], 0, eGreaterZero );
    pTimeField->SetMinimumValueInSecond( 1 );
    // Feedback time
    builder.AddCheckableField<ADN_TimeField>( pInfoHolder, "force-ratio-feedback-time", tr( "Force ratio feedback time" ), vInfosConnectors[ eHasStrengthRatioFeedbackTime ], vInfosConnectors[ eStrengthRatioFeedbackTime ] );
    // Crossing height
    builder.AddEnumField( pInfoHolder, "crossing-height", tr( "Crossing height" ), vInfosConnectors[ eCrossingHeight ] );
    // Is autonomous
    builder.AddField< ADN_CheckBox >( pInfoHolder, "is-autonomous", tr( "Is autonomous (UAV)" ), vInfosConnectors[ eIsAutonomous ] );
    builder.AddStretcher( pInfoHolder, Qt::Vertical );
    // Footprint
    builder.AddField< ADN_EditLine_Double >( pInfoHolder, "footprint", tr( "Footprint" ), vInfosConnectors[eFootprint], tr( "m" ) );
    // Can fly
    Q3VBox* pCanFlyBox = new Q3VBox();
    ADN_GroupBox* pCanFlyGroup = builder.AddGroupBox( pCanFlyBox, "can-fly", tr( "Can fly" ), vInfosConnectors[ eCanFly ], 3 );
    builder.AddField< ADN_EditLine_Int >( pCanFlyGroup, "standard-height", tr( "Standard flying height" ), vInfosConnectors[ eStandardFlyingHeight ], tr( "m" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Int >( pCanFlyGroup, "tactical-height", tr( "Tactical flying height" ), vInfosConnectors[ eTacticalFlyingHeight ], tr( "m" ), eGreaterEqualZero );

    // Coup de sonde
    ADN_GroupBox* pReconGroup = builder.AddGroupBox( 0, "can-probe", tr( "Scan" ), vInfosConnectors[eCanProbe], 3 );
    builder.AddField< ADN_EditLine_Double >( pReconGroup, "width", tr( "Width" ), vInfosConnectors[eProbeWidth], tr( "m" ) );
    builder.AddField< ADN_EditLine_Double >( pReconGroup, "depth", tr( "Depth" ), vInfosConnectors[eProbeLength], tr( "m" ) );

    // sensor & equipment ranges
    ADN_GroupBox* pRangeGroup = builder.AddGroupBox( 0, "ranges", tr( "Ranges" ), vInfosConnectors[ eRanges ], 3 );
    builder.AddField< ADN_EditLine_Int >( pRangeGroup, "sensors", tr( "Sensors" ), vInfosConnectors[ eSensorRange ], tr( "m" ) );
    builder.AddField< ADN_EditLine_Int >( pRangeGroup, "equipments", tr( "Equipments" ), vInfosConnectors[ eEquipmentRange ], tr( "m" ) );

    // Nature group
    Q3GroupBox* pNatureGroup = new Q3GroupBox( 2, Qt::Horizontal, tr( "Nature" ) );
    {
        Q3VBox* natureLayout = new Q3VBox( pNatureGroup );
        QGroupBox* pNatureInternalGroup = new gui::RichGroupBox( "nature-group-box", natureLayout );
        QGridLayout* pNatureInternalGroupLayout = new QGridLayout( pNatureInternalGroup );
        pNatureInternalGroupLayout->setMargin( 10 );
        pNatureInternalGroupLayout->setAlignment( Qt::AlignTop );
        // Level & Atlas
        Q3GroupBox* subLayout = new Q3GroupBox( 3, Qt::Horizontal );
        subLayout->setMargin( 0 );
        subLayout->setInsideMargin( 0 );
        subLayout->setFrameStyle( Q3Frame::Plain );
        builder.AddEnumField( subLayout, "level", tr( "Level:" ), vInfosConnectors[ eNatureLevel ] );
        builder.AddEnumField( subLayout, "atlas", tr( "Atlas:" ), vInfosConnectors[ eNatureAtlas ] );
        // Separator
        QFrame* separator = new QFrame;
        separator->setFrameStyle( QFrame::HLine | QFrame::Raised );
        pNatureInternalGroupLayout->addWidget( separator, 1, 0, 1, 2, Qt::AlignTop );
        // Nature GUI
        pNatureInternalGroupLayout->addWidget( subLayout, 0, 0, 1, 2, Qt::AlignTop );
        pNatureGui_ = builder.AddWidget< ADN_Nature_GUI >( "nature", pNatureInternalGroupLayout, 2 );
        vInfosConnectors[ eNatureNature ] = &pNatureGui_->GetConnector();
        connect( pNatureGui_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNatureChanged() ) );
        const kernel::SymbolFactory& factory = ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolFactory();
        if( factory.GetSymbolRule() )
            pNatureGui_->SetRootSymbolRule( *factory.GetSymbolRule() );

        // Symbol combo
        QWidget* pHolder = builder.AddFieldHolder( natureLayout );
        ADN_UnitSymbolsComboBox* unitSymbolsCombo = builder.AddField< ADN_UnitSymbolsComboBox >( pHolder, "unit-symbol", tr( "Unit Symbol" ), vInfosConnectors[ eNatureSymbol ] );
        unitSymbolsCombo->setMinimumHeight( SYMBOL_PIXMAP_SIZE );
        connect( unitSymbolsCombo, SIGNAL( UnitSymbolChanged( const QString& ) ), pNatureGui_, SLOT( OnUnitSymbolChanged( const QString& ) ) );
        connect( pNatureGui_, SIGNAL( textChanged( const QString& ) ), unitSymbolsCombo, SLOT( OnNatureChanged( const QString& ) ) );

        // Symbol
        Q3VBox* pSymbolLayout = new Q3VBox( pNatureGroup );
        pSymbolLayout->layout()->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
        CreateUnitSymbolWidget( builder, "symbols"       , pSymbolLayout, &ADN_Symbols_Data::SymbolsUnit::GetSamplePixmap,  &ADN_Symbols_Data::SymbolsUnit::GetSymbol       , labelSymbol_      , unitSymbolWidget_ );
        CreateUnitSymbolWidget( builder, "move-symbols"  , pSymbolLayout, &ADN_Symbols_Data::SymbolsUnit::GetMovePixmap,    &ADN_Symbols_Data::SymbolsUnit::GetMoveSymbol   , labelMoveSymbol_  , unitSymbolMoveWidget_ );
        CreateUnitSymbolWidget( builder, "static-symbols", pSymbolLayout, &ADN_Symbols_Data::SymbolsUnit::GetStaticPixmap,  &ADN_Symbols_Data::SymbolsUnit::GetStaticSymbol , labelStaticSymbol_, unitSymbolStaticWidget_ );
        SetOnlyApp6SymbolVisible();
    }

    // Commandement
    Q3GroupBox* pCommandGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Command" ) );
    pCommandGroup->setInsideMargin( 20 );
    pCommandGroup->setInsideSpacing( 10 );

    // officer
    pOfficersEditLine_ = builder.AddField< ADN_EditLine_Int >( pCommandGroup, "nbr-officers", tr( "Nbr of officer(s)" ), vInfosConnectors[ eNbOfficer ] );
    pOfficersEditLine_->GetValidator().setRange( 0, 0 );
    connect( pOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfOfficersChanged() ) );

    // nc officer
    pNCOfficersEditLine_ = builder.AddField< ADN_EditLine_Int >( pCommandGroup, "nbr-warrant", tr( "Nbr of warrant officer(s)" ), vInfosConnectors[ eNbNCOfficer ] );
    pNCOfficersEditLine_->GetValidator().setRange( 0, 0 );
    connect( pNCOfficersEditLine_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnNbrOfNCOfficersChanged() ) );

    Q3VBox* postureInstallationBox = new Q3VBox();

    // Postures
    Q3VGroupBox* pPosturesGroup = new Q3VGroupBox( tr( "Stances" ), postureInstallationBox );
    new ADN_Units_Postures_GUI( builder.GetChildName( "postures-table" ), vInfosConnectors[ ePostures ], pPosturesGroup );

    // Installation
    pInstallationGroup_ = builder.AddGroupBox( postureInstallationBox, "installation", tr( "Deployment" ), vInfosConnectors[ eHasInstallation ], 3 );
    builder.AddField< ADN_TimeField >( pInstallationGroup_, "deployment-duration", tr( "Deployment duration" ), vInfosConnectors[ eInstallationDelay ] );
    builder.AddField< ADN_TimeField >( pInstallationGroup_, "un-deployment-duration", tr( "Un-deployment duration" ), vInfosConnectors[ eUninstallationDelay ] );

    // Distances before point on path
    Q3GroupBox* pDistancesGroup = new Q3HGroupBox( tr( "Key terrain features range" ) );
    new ADN_Point_GUI( builder.GetChildName( "terrain-features-table" ), vInfosConnectors[ ePointInfos ], pDistancesGroup );

    // Composantes
    Q3VGroupBox* pComposantesGroup = new Q3VGroupBox( tr( "Equipments" ) );
    ADN_Units_Composantes_GUI * pComposantes = new ADN_Units_Composantes_GUI( builder.GetChildName( "equipments-table" ), vInfosConnectors[ eComposantes ], pComposantesGroup );
    pComposantes->SetGoToOnDoubleClick( ::eEquipments );
    if( QAbstractProxyModel* model = static_cast< QAbstractProxyModel* >( pComposantes->model() ) )
        connect( model->sourceModel(), SIGNAL( itemChanged( QStandardItem* ) ), this, SLOT( OnComponentChanged() ) );

    // Trafic
    Q3GroupBox* pTrafficGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Traffic" ) );
    builder.AddField< ADN_EditLine_Int >( pTrafficGroup, "footprint-radius", tr( "Footprint radius" ), vInfosConnectors[ eFootprintRadius ], tr( "m" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pTrafficGroup, "speed-reduction-modifier", tr( "Speed reduction modifier" ), vInfosConnectors[ eSpeedModifier ], 0, eZeroOne );

    // Dotations
    ADN_GroupBox* pDotationsGroup = builder.AddGroupBox( 0, "complementary-resources", tr( "Complementary load" ), vInfosConnectors[ eHasTC1 ], 1 );
    ADN_Equipments_Dotations_GUI* pDotations = new ADN_Equipments_Dotations_GUI( builder.GetChildName( "resources-table" ), vInfosConnectors[ eContenancesTC1 ], pDotationsGroup, ADN_Equipments_Dotations_GUI::eColumn_All ^ ADN_Equipments_Dotations_GUI::eColumn_Consumption, QHeaderView::ResizeToContents );
    pDotations->SetGoToOnDoubleClick( ::eSupplies );

    // Stock
    pStockGroup_ = builder.AddGroupBox( 0, "stock", tr( "Stock" ), vInfosConnectors[ eHasStock ], 1 );
    pStockLogThreshold_ = new ADN_Units_LogThreshold_GUI( builder.GetChildName( "stock-threshold-table" ), vInfosConnectors[ eStock ], pStockGroup_ );

    // Aptitudes
    Q3GroupBox* pSkillsGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Skills" ) );
    pSkillsGroup->setInsideMargin( 20 );
    pSkillsGroup->setInsideSpacing( 10 );

    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, "recon", tr( "Recon" ), vInfosConnectors[ eRecon ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, "combat-support", tr( "Combat support" ), vInfosConnectors[ eCombatSupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, "combat", tr( "Combat" ), vInfosConnectors[ eCombat ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, "mobility-support", tr( "Mobility support" ), vInfosConnectors[ eMobilitySupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, "counter-mobility-support", tr( "Counter mobility support" ), vInfosConnectors[ eCounterMobilitySupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, "protection-support", tr( "Protection support" ), vInfosConnectors[ eProtectionSupport ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Int >( pSkillsGroup, "engineering-recon", tr( "Engineering recon" ), vInfosConnectors[ eEngineeringRecon ], tr( "%" ), ePercentage );

    // Efficiencies
    Q3GroupBox* pEfficienciesGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Efficiencies" ) );
    pEfficienciesGroup->setInsideMargin( 20 );
    pEfficienciesGroup->setInsideSpacing( 10 );
    builder.AddField< ADN_EditLine_Int >( pEfficienciesGroup, "urban-area-efficiency", tr( "Urban area efficiency" ), vInfosConnectors[ eUrbanAreaEfficiency ], tr( "%" ), ePercentage );

    // Civilian
    ADN_GroupBox* pCivilianGroup = builder.AddGroupBox( 0, "is-civilian", tr( "Civilian" ), vInfosConnectors[ eIsCivilian ], 1 );
    ADN_MultiPercentage_Double* pMultiPercentage = new ADN_MultiPercentage_Double( pCivilianGroup, builder, builder.GetChildName( "civilian-repartition" ) );
    pMultiPercentage->AddLine( tr( "Males" ), vInfosConnectors[ eMalesPercent ], "males" );
    pMultiPercentage->AddLine( tr( "Females" ), vInfosConnectors[ eFemalesPercent ], "females" );
    pMultiPercentage->AddLine( tr( "Children" ), vInfosConnectors[ eChildrenPercent ], "children" );
    pMultiPercentage->AddWarning();

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QGridLayout* pContentLayout = new QGridLayout( pContent, 8, 6, 5 );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addMultiCellWidget( pInfoHolder, 0, 0, 0, 2 );
    pContentLayout->addMultiCellWidget( pCanFlyBox, 1, 1, 0, 2 );
    pContentLayout->addMultiCellWidget( pNatureGroup, 0, 1, 3, 5 );
    pContentLayout->addMultiCellWidget( pComposantesGroup, 2, 4, 0, 3 );
    pContentLayout->addMultiCellWidget( pCommandGroup, 2, 2, 4, 5 );
    pContentLayout->addMultiCellWidget( pReconGroup, 3, 3, 4, 5 );
    pContentLayout->addMultiCellWidget( pRangeGroup, 4, 4, 4, 5 );
    pContentLayout->addMultiCellWidget( pDistancesGroup, 5, 5, 0, 1 );
    pContentLayout->addMultiCellWidget( postureInstallationBox, 5, 5, 2, 3 );
    pContentLayout->addMultiCellWidget( pDotationsGroup, 6, 7, 0, 1 );
    pContentLayout->addMultiCellWidget( pStockGroup_, 6, 7, 2, 3 );
    pContentLayout->addMultiCellWidget( pSkillsGroup, 5, 5, 4, 5 );
    pContentLayout->addMultiCellWidget( pEfficienciesGroup, 6, 6, 4, 5 );
    pContentLayout->addMultiCellWidget( pCivilianGroup, 7, 7, 4, 5 );
    pContentLayout->addMultiCellWidget( pTrafficGroup, 8, 8, 0, 3 );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_ListView_Units >( this, data_.GetUnitsInfos(), vInfosConnectors );
    connect( pListView_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnTypeChanged() ) );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
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
    if( strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLDRavitaillement, ENT_Tr::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGBLTRavitaillement, ENT_Tr::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGConvoi, ENT_Tr::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionLOGTC2, ENT_Tr::eToTr ).c_str()
        || strType == ADN_Tr::ConvertFromAgentTypePion( eAgentTypePionOrganization, ENT_Tr::eToTr ).c_str() )
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
// Name: ADN_Units_GUI::OnNatureChanged
// Created: MMC 2013-04-11
// -----------------------------------------------------------------------------
void ADN_Units_GUI::OnNatureChanged()
{
    if( labelMoveSymbol_ && labelStaticSymbol_ )
        SetOnlyApp6SymbolVisible( labelMoveSymbol_->text().isEmpty() && labelStaticSymbol_->text().isEmpty() );
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
// Name: ADN_Units_GUI::CreateUnitSymbolWidget
// Created: MMC 2013-04-10
// -----------------------------------------------------------------------------
void ADN_Units_GUI::CreateUnitSymbolWidget( ADN_GuiBuilder& builder, const std::string widgetName, QWidget* parent,
                                            ADN_UnitSymbolWidget::T_PixmapExtractor pixmapAccessor,
                                            ADN_UnitSymbolWidget::T_StringExtractor stringAccessor,
                                            QLabel*& pLabel, ADN_UnitSymbolWidget*& pSymbolWidget )
{
    pLabel = new QLabel( parent );
    pSymbolWidget = builder.AddWidget< ADN_UnitSymbolWidget, QWidget*, ADN_UnitSymbolWidget::T_PixmapExtractor, ADN_UnitSymbolWidget::T_StringExtractor >( "symbols", parent, pixmapAccessor, stringAccessor );
    pSymbolWidget->setMinimumSize( 130, 140 );
    pSymbolWidget->setMaximumSize( 130, 140 );
    connect( pNatureGui_, SIGNAL( textChanged( const QString& ) ), pSymbolWidget, SLOT( OnNatureChanged( const QString& ) ) );
    connect( pSymbolWidget, SIGNAL( SymbolChanged( const QString& ) ), pLabel, SLOT( setText( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::SetOnlyApp6SymbolVisible
// Created: MMC 2013-04-10
// -----------------------------------------------------------------------------
void ADN_Units_GUI::SetOnlyApp6SymbolVisible( bool bVisible /*= true*/ )
{
    labelSymbol_->setVisible( true );
    unitSymbolWidget_->setVisible( bVisible );
    labelMoveSymbol_->setVisible( !bVisible );
    unitSymbolMoveWidget_->setVisible( !bVisible );
    labelStaticSymbol_->setVisible( !bVisible );
    unitSymbolStaticWidget_->setVisible( !bVisible );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_GUI::ExportHtml
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void ADN_Units_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const tools::Path& strPath )
{
    tools::Path strLocalPath = strPath / tools::Path::FromUnicode( tr( "Units/" ).toStdWString() );
    strLocalPath.CreateDirectories();
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Units" ) );

    ADN_Units_Data::T_UnitInfos_Vector& units = data_.GetUnitsInfos();
    indexBuilder.BeginTable( static_cast< int >( units.size()+1 ), 2 );
    indexBuilder.TableItem( 0, 0, tr( "Name" ).toStdString().c_str(), true );
    indexBuilder.TableItem( 0, 1, tr( "Type" ).toStdString().c_str(), true );
    int n = 1;
    for( auto it = units.begin(); it != units.end(); ++it, ++n )
    {
        ADN_Units_Data::UnitInfos& unit = **it;
        indexBuilder.TableItem( n, 0, unit.strName_.GetData().c_str() );
        indexBuilder.TableItem( n, 1, ADN_Tr::ConvertFromAgentTypePion( unit.eTypeId_.GetData() ).c_str() );

    }
    indexBuilder.EndTable();
    indexBuilder.WriteToFile( strLocalPath / "index.htm" );

    QString strText = "<a href=\"" + tr( "Units/" ) + "index.htm\">" + tr( "Units" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}
