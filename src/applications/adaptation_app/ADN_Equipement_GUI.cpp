// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_GUI.h"
#include "moc_ADN_Equipement_GUI.cpp"
#include "ADN_ComboBox_Equipment_Nature.h"
#include "ADN_ComboBox_Equipment_LogisticSupplyClass.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Equipement_AmmoListView.h"
#include "ADN_Equipement_AttritionGraph.h"
#include "ADN_Equipement_AttritionTable.h"
#include "ADN_Equipement_GenericListView.h"
#include "ADN_Equipement_Postures_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_MainWindow.h"
#include "ADN_Tr.h"
#include "ADN_UrbanModifiersTable.h"
#include "ENT/ENT_Tr.h"

class ADN_Equipement_UrbanModifiersTable : public helpers::ADN_UrbanModifiersTable
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipement_UrbanModifiersTable( QWidget* pParent, ADN_Connector_ABC*& pGuiConnector ) : ADN_UrbanModifiersTable( pParent, pGuiConnector ) {}
    virtual ~ADN_Equipement_UrbanModifiersTable() {}
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void doValueChanged( int row, int col )
    {
        ADN_UrbanModifiersTable::doValueChanged( row, col );
        ADN_Workspace::GetWorkspace().GetEquipements().GetGui().UpdateGraph();
    }
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI constructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Equipement_GUI::ADN_Equipement_GUI( ADN_Equipement_Data& data )
    : ADN_GUI_ABC( "ADN_Equipement_GUI" )
    , data_      ( data )
    , pAttritionTable_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI destructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Equipement_GUI::~ADN_Equipement_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::Build
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;
    pMainWidget_ = new QWidget();
    QTabWidget* pTabWidget = new QTabWidget();
    this->BuildAmmunition( pTabWidget );
    this->BuildGeneric( eDotationFamily_Carburant, pTabWidget );
    this->BuildGeneric( eDotationFamily_Mine, pTabWidget );
    this->BuildGeneric( eDotationFamily_Explosif, pTabWidget );
    this->BuildGeneric( eDotationFamily_Barbele, pTabWidget );
    this->BuildGeneric( eDotationFamily_Ration, pTabWidget );
    this->BuildGeneric( eDotationFamily_AgentExtincteur, pTabWidget );
    this->BuildGeneric( eDotationFamily_Piece, pTabWidget );
    this->BuildGeneric( eDotationFamily_Energy, pTabWidget );
    QGridLayout* pMainLayout = new QGridLayout();
    pMainLayout->setSpacing( 10 );
    pMainLayout->setContentsMargins( 10, 10, 10, 10 );
    pMainLayout->addWidget( pTabWidget );
    pMainWidget_->setLayout( pMainLayout );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::BuildGeneric
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::BuildGeneric( E_DotationFamily nType, QTabWidget* pParent )
{
    ADN_GuiBuilder builder;
    QWidget* pPage = new QWidget();
    pParent->addTab( pPage, ENT_Tr::ConvertFromDotationFamily( nType, ENT_Tr_ABC::eToTr ).c_str() );
    ADN_Equipement_GenericListView* pListView = new ADN_Equipement_GenericListView( nType, pPage );
    pListView->GetConnector().Connect( &data_.GetDotation( nType ).categories_ );
    T_ConnectorVector vConnectors( eNbrGenericGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    
    QGroupBox* pGroupBox = new QGroupBox( ENT_Tr::ConvertFromDotationFamily( nType, ENT_Tr_ABC::eToTr ).c_str() );
    QWidget* pHolder = builder.AddFieldHolder( pGroupBox );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name" ), vConnectors[ eName ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeEMAT6" ), vConnectors[ eGenEMAT6Code ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeEMAT8" ), vConnectors[ eGenEMAT8Code ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeLFRIL" ), vConnectors[eGenLFRILCode ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeNNO" ), vConnectors[ eGenNNOCode ] );
    builder.AddField< ADN_ComboBox_Equipment_Nature >( pHolder, tr( "Nature" ), vConnectors[ eGenNature] );
    builder.AddField< ADN_ComboBox_Equipment_LogisticSupplyClass >( pHolder, tr( "Logistic supply class" ), vConnectors[ eGenLogisticSupplyClass] );
    Q3GroupBox* pPackagingGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Packaging" ), pGroupBox );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Nbr per package" ), vConnectors[ ePackageNbr ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package weight" ), vConnectors[ ePackageWeight ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package volume" ), vConnectors[ ePackageVolume ], tr( "m3" ), eGreaterZero );
    QVBoxLayout* pGroupBoxLayout = new QVBoxLayout();
    pGroupBoxLayout->addWidget( pHolder );
    pGroupBoxLayout->addWidget( pPackagingGroup, 1 );
    pGroupBox->setLayout( pGroupBoxLayout );

    pListView->SetItemConnectors( vConnectors );
    
    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pPage );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setContentsMargins( 10, 10, 10, 10 );
    pMainLayout->addWidget( pListView, 1 );
    pMainLayout->addWidget( pGroupBox, 4 );
    pPage->setLayout( pMainLayout );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::BuildAmmunition
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::BuildAmmunition( QTabWidget* pParent )
{
    ADN_GuiBuilder builder;
    QWidget* pPage = new QWidget();
    pParent->addTab( pPage, ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Munition, ENT_Tr_ABC::eToTr ).c_str() );
    
    pAmmoListView_ = new ADN_Equipement_AmmoListView( pPage );
    pAmmoListView_->GetConnector().Connect( &data_.GetDotation( eDotationFamily_Munition ).categories_ );
    
    T_ConnectorVector vConnectors( eNbrAmmoGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    
    QGroupBox* pGroupBox = new QGroupBox( ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Munition, ENT_Tr_ABC::eToTr ).c_str() );
    
    // Generic properties
    QWidget* pHolder = builder.AddFieldHolder( pGroupBox );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "Name" ), vConnectors[ eAmmoName ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeEMAT6" ), vConnectors[ eEMAT6Code ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeEMAT8" ), vConnectors[ eEMAT8Code ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeLFRIL" ), vConnectors[ eLFRILCode ] );
    builder.AddField< ADN_EditLine_String >( pHolder, tr( "CodeNNO" ), vConnectors[ eNNOCode ] );
    builder.AddEnumField< E_MunitionType >( pHolder, tr( "Type" ), vConnectors[ eType ], ADN_Tr::ConvertFromMunitionType );
    builder.AddField< ADN_ComboBox_Equipment_Nature >( pHolder, tr( "Nature" ), vConnectors[ eNature ] );
    builder.AddField< ADN_ComboBox_Equipment_LogisticSupplyClass >( pHolder, tr( "Logistic supply class" ), vConnectors[ eLogisticSupplyClass] );

    // Packaging
    Q3GroupBox* pPackagingGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Packaging" ), pGroupBox );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Nbr per package" ), vConnectors[ eAmmoPackageNbr ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package weight" ), vConnectors[ eAmmoPackageWeight ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package volume" ), vConnectors[ eAmmoPackageVolume ], tr( "m3" ), eGreaterZero );

    // Direct fire properties
    ADN_GroupBox* pDirectGroup = new ADN_GroupBox( tr( "Attritions" ), pGroupBox );
    vConnectors[ eDirect ] = &pDirectGroup->GetConnector();
    pAttritionTable_ = new ADN_Equipement_AttritionTable( pDirectGroup );
    vConnectors[ eAttritions ] = &pAttritionTable_->GetConnector();
    ADN_Equipement_UrbanModifiersTable* pUrbanTable = new ADN_Equipement_UrbanModifiersTable( pDirectGroup, vConnectors[ eUrbanAttritions ] );

    QGroupBox* pAttritionVisualisation = new QGroupBox( tr( "Simulation" ) );
    QWidget* pComboGroup = builder.AddFieldHolder( pAttritionVisualisation );
    pArmorCombo_ = builder.AddField< ADN_ComboBox_Vector< helpers::ArmorInfos > >( pComboGroup, tr( "Armor-Plating" ), vConnectors[ eArmor ] );
    connect( pArmorCombo_, SIGNAL( activated( int ) ), this, SLOT( SimulationCombosActivated() ) );
    pMaterialCombo_ = builder.AddField< ADN_ComboBox_Vector< helpers::ADN_UrbanAttritionInfos > >( pComboGroup, tr( "Urban material" ), vConnectors[ eMaterial ] );
    connect( pMaterialCombo_, SIGNAL( activated( int ) ), this, SLOT( SimulationCombosActivated() ) );
    pAttritionGraph_ = new ADN_Equipement_AttritionGraph( pAttritionVisualisation );
    vConnectors[ eAttritionGraph ] = &pAttritionGraph_->GetConnector();

    QVBoxLayout* pAttritionVisualisationLayout = new QVBoxLayout();
    pAttritionVisualisationLayout->addWidget( pComboGroup );
    pAttritionVisualisationLayout->addWidget( pAttritionGraph_ );
    pAttritionVisualisation->setLayout( pAttritionVisualisationLayout );

    QGridLayout* pDirectGroupLayout = new QGridLayout();
    pDirectGroupLayout->addWidget( pAttritionTable_, 0, 0 );
    pDirectGroupLayout->addWidget( pUrbanTable, 1, 0 );
    pDirectGroupLayout->addWidget( pAttritionVisualisation, 0, 1, 2, 1 );
    pDirectGroup->setLayout( pDirectGroupLayout );

    // Indirect fire properties
    ADN_GroupBox* pIndirectGroup = new ADN_GroupBox( tr( "Indirect fire" ), pGroupBox );
    vConnectors[ eIndirect ] = &pIndirectGroup->GetConnector();
    QWidget* pIndirectGroupHolder = builder.AddFieldHolder( pIndirectGroup );
    pIndirectTypeCombo_ = builder.AddEnumField< E_TypeMunitionTirIndirect >( pIndirectGroupHolder, tr( "Type" ), vConnectors[ eIndirectType ], ADN_Tr::ConvertFromTypeMunitionTirIndirect );
    pIndirectGroup->connect( pIndirectTypeCombo_, SIGNAL( activated( int ) ), this, SLOT( IndirectTypeComboActivated( int ) ) );
    builder.AddField< ADN_EditLine_Int >( pIndirectGroupHolder, tr( "Intervention" ), vConnectors[ eIntervention ], 0, eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroupHolder, tr( "X Dispersion" ), vConnectors[ eDispersionX ], tr( "m" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroupHolder, tr( "Y Dispersion" ), vConnectors[ eDispersionY ], tr( "m" ), eGreaterZero );
    
    {
        // Explosive parameters
        pExplosiveParametersGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Explosive ammo parameters" ), pIndirectGroup );
        ADN_Equipement_Postures_GUI* pStance = new ADN_Equipement_Postures_GUI( tr( "Stance" ), pExplosiveParametersGroup_ );
        vConnectors[ eModifStances ] = &pStance->GetConnector();
        QWidget* pExplosiveParametersGroupHolder = builder.AddFieldHolder( pExplosiveParametersGroup_ );
        builder.AddField< ADN_EditLine_Double >( pExplosiveParametersGroupHolder, tr( "Neutralization ratio" ), vConnectors[ eNeutralizationRatio ] );
        builder.SetValidator( new ADN_DoubleValidator( 1, INT_MAX, 2, this ) );

        // Flare parameters
        pFlareParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Flare/Smoke ammo parameters" ), pIndirectGroup );
        builder.AddField< ADN_TimeField >( pFlareParametersGroup_, tr( "Activation duration" ), vConnectors[ eDeployTime ] );
        builder.AddField< ADN_TimeField >( pFlareParametersGroup_, tr( "Span" ), vConnectors[ eFlareLifetime ] );

        // Effect (object) parameters
        pEffectParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Effect ammo parameters" ), pIndirectGroup );
        builder.AddField< ADN_EditLine_String >( pEffectParametersGroup_, tr( "Created object" ), vConnectors[ eEffectType ] );
        builder.AddField< ADN_TimeField >( pEffectParametersGroup_, tr( "Span" ), vConnectors[ eEffectLifetime ] );

        // Mine parameters
        pMineParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Mine ammo parameters" ), pIndirectGroup );
        builder.AddField< ADN_EditLine_Int >( pMineParametersGroup_, tr( "Mines quantity" ), vConnectors[ eMineNumber ], 0, eGreaterEqualZero );

        pIndirectEffectLayout_ = new QStackedLayout();
        pIndirectEffectLayout_->addWidget( pExplosiveParametersGroup_ );
        pIndirectEffectLayout_->addWidget( pFlareParametersGroup_ );
        pIndirectEffectLayout_->addWidget( pEffectParametersGroup_ );
        pIndirectEffectLayout_->addWidget( pMineParametersGroup_ );
    }

    QVBoxLayout* pIndirectGroupLayout = new QVBoxLayout();
    pIndirectGroupLayout->addWidget( pIndirectGroupHolder );
    pIndirectGroupLayout->addLayout( pIndirectEffectLayout_ );
    pIndirectGroup->setLayout( pIndirectGroupLayout );

    // Illumination
    ADN_GroupBox* pIlluminationGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Illumination capacity" ), pGroupBox );
    vConnectors[ eIlluminating ] = &pIlluminationGroup->GetConnector();
    builder.AddField< ADN_EditLine_Double >( pIlluminationGroup, tr( "Range" ), vConnectors[ eRange ], 0, eGreaterEqualZero );
    builder.AddField< ADN_CheckBox >( pIlluminationGroup, tr( "Must Maintain illumination" ), vConnectors[ eMaintainIllumination ] );

    // Guidance
    ADN_GroupBox* pGuidanceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Guidance" ), pGroupBox );
    vConnectors[ eGuided ] = &pGuidanceGroup->GetConnector();
    builder.AddField< ADN_CheckBox >( pGuidanceGroup, tr( "Must Maintain guidance" ), vConnectors[ eMaintainGuidance ] );
    builder.AddField< ADN_EditLine_Double >( pGuidanceGroup, tr( "Illumination range needed" ), vConnectors[ eGuidanceRange ], 0, eGreaterEqualZero );

    // Properties Layout
    QVBoxLayout* pGroupBoxLayout = new QVBoxLayout();
    pGroupBoxLayout->addWidget( pHolder );
    pGroupBoxLayout->addWidget( pPackagingGroup );
    pGroupBoxLayout->addWidget( pDirectGroup );
    pGroupBoxLayout->addWidget( pIndirectGroup );
    pGroupBoxLayout->addWidget( pIlluminationGroup );
    pGroupBoxLayout->addWidget( pGuidanceGroup );
    pGroupBox->setLayout( pGroupBoxLayout );

    //Connect
    pAmmoListView_->SetItemConnectors( vConnectors );
    
    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout();
    pMainLayout->setSpacing( 10 );
    pMainLayout->setContentsMargins( 10, 10, 10, 10 );
    pMainLayout->addWidget( pAmmoListView_, 1 );
    pMainLayout->addWidget( pGroupBox, 4 );
    pPage->setLayout( pMainLayout );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::UpdateGraph
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::UpdateGraph()
{
    if( pAttritionGraph_ )
        pAttritionGraph_->Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::InitializeArmorCombo
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::InitializeSimulationCombos()
{
    if( pArmorCombo_ )
    {
        for( int i = pArmorCombo_->count() - 1; i >= 0; --i )
            if( static_cast< helpers::ArmorInfos* >( pArmorCombo_->GetItem( i )->GetData() )->nType_ == eProtectionType_Human )
                pArmorCombo_->removeItem( i );
        if( pArmorCombo_->GetItem( 0 ) )
            pArmorCombo_->setCurrentItem( 0 );
    }

    if( pMaterialCombo_ )
    {
        ADN_ComboBoxItem* noneItem = new ADN_ComboBoxItem( *pMaterialCombo_, 0 );
        noneItem->setText( tr( "None" ) );
        pMaterialCombo_->insertItem( noneItem, 0 );
        pMaterialCombo_->setCurrentItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::GetSelectedArmor
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
helpers::ArmorInfos* ADN_Equipement_GUI::GetSelectedArmor() const
{
    return static_cast< helpers::ArmorInfos* >( pArmorCombo_->GetCurrentData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::GetSelectedMaterial
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
helpers::ADN_UrbanAttritionInfos* ADN_Equipement_GUI::GetSelectedMaterial() const
{
    return static_cast< helpers::ADN_UrbanAttritionInfos* >( pMaterialCombo_->GetCurrentData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::IndirectTypeComboActivated
// Created: APE 2005-01-04
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::IndirectTypeComboActivated( int nIndex )
{
    E_TypeMunitionTirIndirect type = static_cast< E_TypeMunitionTirIndirect >( pIndirectTypeCombo_->GetEnumIndexFromGUI( nIndex ) );
    if( type == eTypeMunitionTirIndirect_Explosif || type == eTypeMunitionTirIndirect_Aced || type == eTypeMunitionTirIndirect_Grenade )
        pIndirectEffectLayout_->setCurrentWidget( pExplosiveParametersGroup_ );
    else if( type == eTypeMunitionTirIndirect_Eclairant || type == eTypeMunitionTirIndirect_Fumigene )
        pIndirectEffectLayout_->setCurrentWidget( pFlareParametersGroup_ );
    else if( type == eTypeMunitionTirIndirect_Mine )
        pIndirectEffectLayout_->setCurrentWidget( pMineParametersGroup_ );
    else if( type == eTypeMunitionTirIndirect_Effect )
        pIndirectEffectLayout_->setCurrentWidget( pEffectParametersGroup_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::SimulationCombosActivated
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::SimulationCombosActivated()
{
    UpdateGraph();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::CreatePKTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
ADN_Table* ADN_Equipement_GUI::CreatePKTable()
{
    helpers::T_ArmorInfos_Vector& armorInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos();
    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );
    // Setup the header.
    pTable->setNumCols( 5 );
    pTable->horizontalHeader()->setLabel( 0, tr( "Ammunition" ) );
    pTable->horizontalHeader()->setLabel( 1, tr( "Target armor" ) );
    pTable->horizontalHeader()->setLabel( 2, tr( "On site fixable" ) );
    pTable->horizontalHeader()->setLabel( 3, tr( "Maintenance support needed" ) );
    pTable->horizontalHeader()->setLabel( 4, tr( "Destroyed" ) );
    pTable->horizontalHeader()->show();
    pTable->setNumRows( 1 );
    builder.AddTableCell( pTable, 0, 0, tr( "Ammunition" ) );
    builder.AddTableCell( pTable, 0, 1, tr( "Target armor" ) );
    builder.AddTableCell( pTable, 0, 2, tr( "On site fixable" ) );
    builder.AddTableCell( pTable, 0, 3, tr( "Maintenance support needed" ) );
    builder.AddTableCell( pTable, 0, 4, tr( "Destroyed" ) );
    pTable->hideRow( 0 );
    pTable->AddBoldGridRow( 0 );
    pTable->setSorting( false );
    // Fill the table.
    ADN_Equipement_Data::ResourceInfos& ammo = data_.GetDotation( eDotationFamily_Munition );
    int nRowSize = static_cast< int >( armorInfos.size() );
    int nRow = 1;
    for( ADN_Equipement_Data::IT_CategoryInfos_Vector it = ammo.categories_.begin(); it != ammo.categories_.end(); ++it )
    {
        ADN_Equipement_Data::AmmoCategoryInfo& ammoCategory = *static_cast< ADN_Equipement_Data::AmmoCategoryInfo* >( *it );
        if( !ammoCategory.bDirect_.GetData() )
            continue;
        pTable->setNumRows( nRow + nRowSize );
        pTable->AddBoldGridRow( nRow );
        builder.AddTableCell< ADN_TableItem_String >( pTable, *it, nRow, 0, nRowSize, 1, ammoCategory.strName_, eNone, Q3TableItem::Never );
        int nSubRow = 0;
        for( helpers::IT_AttritionInfos_Vector it2 = ammoCategory.attritions_.begin(); it2 != ammoCategory.attritions_.end(); ++it2, ++nSubRow )
        {
            builder.AddTableCell< ADN_TableItem_String >( pTable, *it, nRow + nSubRow, 1, ( *it2 )->ptrArmor_.GetData()->strName_, eNone, Q3TableItem::Never );
            builder.AddTableCell< ADN_TableItem_Double >( pTable, *it, nRow + nSubRow, 2, ( *it2 )->rRepairNoEvac_, ePercentage )->UseColor( true );
            builder.AddTableCell< ADN_TableItem_Double >( pTable, *it, nRow + nSubRow, 3, ( *it2 )->rRepairWithEvac_, ePercentage )->UseColor( true );
            builder.AddTableCell< ADN_TableItem_Double >( pTable, *it, nRow + nSubRow, 4, ( *it2 )->rDestroy_, ePercentage )->UseColor( true );
        }
        nRow += nRowSize;
    }
    pTable->AdjustColumns( 100 );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::RegisterTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "PKs" ), new ADN_Callback< ADN_Table*, ADN_Equipement_GUI >( this, &ADN_Equipement_GUI::CreatePKTable ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::ExportPKs
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::ExportPKs( ADN_HtmlBuilder& builder, ADN_Equipement_Data::AmmoCategoryInfo& infos )
{
    if( !infos.bDirect_.GetData() )
        return;
    pAmmoListView_->SetCurrentItem( &infos );
    builder.Section( tr( "PKs" ) );
    if( pAttritionTable_ )
        builder.CreateTableFrom( *pAttritionTable_ );
}
