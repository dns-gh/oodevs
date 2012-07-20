// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_GUI.h"
#include "moc_ADN_Equipement_GUI.cpp"
#include "ADN_ComboBox_Equipment_Nature.h"
#include "ADN_ComboBox_Equipment_LogisticSupplyClass.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_DataException.h"
#include "ADN_Equipement_AmmoListView.h"
#include "ADN_Equipement_AttritionGraph.h"
#include "ADN_Equipement_AttritionTable.h"
#include "ADN_Equipement_GenericListView.h"
#include "ADN_Equipement_Postures_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_MainWindow.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_SearchListView.h"
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
    : ADN_Tabbed_GUI_ABC( "ADN_Equipement_GUI" )
    , data_           ( data )
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
    assert( pMainWidget_ == 0 );

    // Tab management
    pTabWidget_ = new QTabWidget();
    BuildAmmunition(); // eDotationFamily_Munition
    for( int i = eDotationFamily_Munition + 1; i < eNbrDotationFamily; ++i ) // Change enum order to change tab order.
        BuildGeneric( static_cast< E_DotationFamily >( i ) );

    // Main widget
    pMainWidget_ = new QWidget();
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget_ );
    pMainWidget_->setObjectName( strClassName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::BuildGeneric
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::BuildGeneric( E_DotationFamily nType )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    const QString builderName = strClassName_ + ENT_Tr::ConvertFromDotationFamily( nType, ENT_Tr_ABC::eToTr ).c_str();
    ADN_GuiBuilder builder( builderName );
    T_ConnectorVector vConnectors( eNbrGenericGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vConnectors[ eName ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code NNO" ), vConnectors[ eGenNNOCode ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code EMAT8" ), vConnectors[ eGenEMAT8Code ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code EMAT6" ), vConnectors[ eGenEMAT6Code ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code LFRIL" ), vConnectors[eGenLFRILCode ] );
    builder.AddField< ADN_ComboBox_Equipment_Nature >( pInfoHolder, tr( "Nature" ), vConnectors[ eGenNature] );
    builder.AddField< ADN_ComboBox_Equipment_LogisticSupplyClass >( pInfoHolder, tr( "Logistic supply class" ), vConnectors[ eGenLogisticSupplyClass] );
    ADN_CheckBox* networkUsableCheckBox = builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Usable within a resource network" ), vConnectors[ eGenNetworkUsable ] );
    vNetworkUsableCheckBoxs_.push_back( networkUsableCheckBox );
    connect( networkUsableCheckBox, SIGNAL( stateChanged( int ) ), SLOT( NetworkUsableActivated( int ) ) );

    // Packaging
    Q3GroupBox* pPackagingGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Packaging" ) );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Nbr per package" ), vConnectors[ ePackageNbr ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package weight" ), vConnectors[ ePackageWeight ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package volume" ), vConnectors[ ePackageVolume ], tr( "m3" ), eGreaterZero );

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
    pContentLayout->addWidget( pPackagingGroup );

    // List view
    ADN_SearchListView< ADN_Equipement_GenericListView >* pSearchListView = new ADN_SearchListView< ADN_Equipement_GenericListView >( nType, data_.GetDotation( nType ).categories_, vConnectors, nType );
    pSearchListView->GetListView()->setObjectName( builderName + "List" );
    connect( this, SIGNAL( ApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ), pSearchListView, SLOT( OnApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ) );
    vListViews_.push_back( pSearchListView->GetListView() );
    assert( nType == vListViews_.size() - 1 );

    // Main page
    QWidget* pPage = CreateScrollArea( *pContent, pSearchListView );
    pTabWidget_->addTab( pPage, ENT_Tr::ConvertFromDotationFamily( nType, ENT_Tr_ABC::eToTr ).c_str() );
}

namespace
{
    void CreateCheckbox( Q3GroupBox* parent, T_ConnectorVector& vConnectors, QString name,
                         QButtonGroup* group, ADN_Equipement_GUI::E_AmmoGuiElements id )
    {
        ADN_CheckBox* checkbox = new ADN_CheckBox( name, parent );
        checkbox->setObjectName( "ADN_Equipement_GUI_Ammunition" + name );
        vConnectors[ id ] = &checkbox->GetConnector();
        group->addButton( checkbox, id );
    }
}
// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::BuildAmmunition
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::BuildAmmunition()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder( strClassName_ + "Ammunition" );
    T_ConnectorVector vConnectors( eNbrAmmoGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vConnectors[ eAmmoName ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code NNO" ), vConnectors[ eNNOCode ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code EMAT8" ), vConnectors[ eEMAT8Code ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code EMAT6" ), vConnectors[ eEMAT6Code ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Code LFRIL" ), vConnectors[ eLFRILCode ] );
    builder.AddEnumField< E_MunitionType >( pInfoHolder, tr( "Type" ), vConnectors[ eType ], ENT_Tr::ConvertFromAmmunitionType );
    builder.AddField< ADN_ComboBox_Equipment_Nature >( pInfoHolder, tr( "Nature" ), vConnectors[ eNature ] );
    builder.AddField< ADN_ComboBox_Equipment_LogisticSupplyClass >( pInfoHolder, tr( "Logistic supply class" ), vConnectors[ eLogisticSupplyClass] );
    ADN_CheckBox* networkUsableCheckBox = builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Usable within a resource network" ), vConnectors[ eNetworkUsable ] );
    networkUsableCheckBox->setObjectName( strClassName_ + "AmmunitionUsable" );
    vNetworkUsableCheckBoxs_.push_back( networkUsableCheckBox );
    connect( networkUsableCheckBox, SIGNAL( stateChanged( int ) ), SLOT( NetworkUsableActivated( int ) ) );
    builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Improvised explosive device" ), vConnectors[ eIsIED ] );

    // Packaging
    Q3GroupBox* pPackagingGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Packaging" ) );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Nbr per package" ), vConnectors[ eAmmoPackageNbr ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package weight" ), vConnectors[ eAmmoPackageWeight ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, tr( "Package volume" ), vConnectors[ eAmmoPackageVolume ], tr( "m3" ), eGreaterZero );

    // Direct fire properties
    ADN_GroupBox* pDirectGroup = new ADN_GroupBox( tr( "Attritions" ) );
    pDirectGroup->setObjectName( strClassName_ + "MunitionsAttritions" );
    vConnectors[ eDirect ] = &pDirectGroup->GetConnector();
    pAttritionTable_ = new ADN_Equipement_AttritionTable( pDirectGroup );
    pAttritionTable_->setObjectName( strClassName_ + "MunitionsAttritionsTable" );
    vConnectors[ eAttritions ] = &pAttritionTable_->GetConnector();
    ADN_Equipement_UrbanModifiersTable* pUrbanTable = new ADN_Equipement_UrbanModifiersTable( pDirectGroup, vConnectors[ eUrbanAttritions ] );
    pUrbanTable->setObjectName( strClassName_ + "MunitionsUrbanTable" );

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
    ADN_GroupBox* pIndirectGroup = new ADN_GroupBox( tr( "Indirect fire" ) );
    vConnectors[ eIndirect ] = &pIndirectGroup->GetConnector();
    QWidget* pIndirectGroupHolder = builder.AddFieldHolder( pIndirectGroup );
    builder.AddField< ADN_EditLine_Int >( pIndirectGroupHolder, tr( "Intervention" ), vConnectors[ eIntervention ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroupHolder, tr( "X Dispersion" ), vConnectors[ eDispersionX ], tr( "m" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroupHolder, tr( "Y Dispersion" ), vConnectors[ eDispersionY ], tr( "m" ), eGreaterZero );

    Q3GroupBox* pEffects = new Q3GroupBox( 5, Qt::Horizontal, tr( "Effects" ), pIndirectGroup );
    {
        buttonGroup_ = new QButtonGroup();
        buttonGroup_->setExclusive( false );

        CreateCheckbox( pEffects, vConnectors, tr( "Explosive" ), buttonGroup_, eExplosivePresent );
        CreateCheckbox( pEffects, vConnectors, tr( "Smoke" ), buttonGroup_, eSmokePresent );
        CreateCheckbox( pEffects, vConnectors, tr( "Illumination shell" ), buttonGroup_, eFlarePresent );
        CreateCheckbox( pEffects, vConnectors, tr( "Mine" ), buttonGroup_, eMinePresent );
        CreateCheckbox( pEffects, vConnectors, tr( "Effect" ), buttonGroup_, eEffectPresent );
        connect( buttonGroup_, SIGNAL( buttonClicked( int ) ), this, SLOT( IndirectTypeChanged() ) );
    }

    QWidget* pEffectsInfo = new Q3VBox();
    {
        // Explosive parameters
        pExplosiveParametersGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Explosive ammo parameters" ), pEffectsInfo );
        ADN_Equipement_Postures_GUI* pStance = new ADN_Equipement_Postures_GUI( tr( "Stance" ), pExplosiveParametersGroup_ );
        pStance->setObjectName( strClassName_ + "AmmunitionsStance" );
        vConnectors[ eModifStances ] = &pStance->GetConnector();
        QWidget* pExplosiveParametersGroupHolder = builder.AddFieldHolder( pExplosiveParametersGroup_ );
        builder.AddField< ADN_EditLine_Double >( pExplosiveParametersGroupHolder, tr( "Neutralization ratio" ), vConnectors[ eNeutralizationRatio ] );
        builder.SetValidator( new ADN_DoubleValidator( 1, INT_MAX, 2, this ) );
        builder.AddField< ADN_EditLine_Double >( pExplosiveParametersGroupHolder, tr( "Dispersion factor per iteration" ), vConnectors[ eDispersionFactor ], 0, eZeroOne );

        // Smoke parameters
        pSmokeParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Smoke ammo parameters" ), pEffectsInfo );
        builder.AddField< ADN_TimeField >( pSmokeParametersGroup_, tr( "Activation duration" ), vConnectors[ eSmokeDeployTime ] );
        builder.AddField< ADN_TimeField >( pSmokeParametersGroup_, tr( "Span" ), vConnectors[ eSmokeLifetime ] );

        // Flare parameters
        pFlareParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Flare ammo parameters" ), pEffectsInfo );
        builder.AddField< ADN_TimeField >( pFlareParametersGroup_, tr( "Activation duration" ), vConnectors[ eFlareDeployTime ] );
        builder.AddField< ADN_TimeField >( pFlareParametersGroup_, tr( "Span" ), vConnectors[ eFlareLifetime ] );

        // Effect (object) parameters
        pEffectParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Effect ammo parameters" ), pEffectsInfo );
        builder.AddField< ADN_ComboBox_Vector< ADN_Objects_Data_ObjectInfos> >( pEffectParametersGroup_, tr( "Created object" ), vConnectors[ eEffectType ] );
        builder.AddField< ADN_TimeField >( pEffectParametersGroup_, tr( "Span" ), vConnectors[ eEffectLifetime ] );

        // Mine parameters
        pMineParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Mine ammo parameters" ), pEffectsInfo );
        builder.AddField< ADN_EditLine_Int >( pMineParametersGroup_, tr( "Mines quantity" ), vConnectors[ eMineNumber ], 0, eGreaterEqualZero );
    }

    QVBoxLayout* pIndirectGroupLayout_ = new QVBoxLayout( pIndirectGroup );
    pIndirectGroupLayout_->addWidget( pIndirectGroupHolder );
    pIndirectGroupLayout_->addWidget( pEffects );
    pIndirectGroupLayout_->addWidget( pEffectsInfo );

    // Illumination
    ADN_GroupBox* pIlluminationGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Illumination capacity" ) );
    pIlluminationGroup->setObjectName( strClassName_ + "_AmmuntitionsIllumination" );
    vConnectors[ eIlluminating ] = &pIlluminationGroup->GetConnector();
    builder.AddField< ADN_EditLine_Double >( pIlluminationGroup, tr( "Range" ), vConnectors[ eRange ], 0, eGreaterEqualZero );
    builder.AddField< ADN_CheckBox >( pIlluminationGroup, tr( "Must Maintain illumination" ), vConnectors[ eMaintainIllumination ] );

    // Guidance
    ADN_GroupBox* pGuidanceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Guidance" ) );
    pIlluminationGroup->setObjectName( strClassName_ + "_AmmuntitionsGuidance" );
    vConnectors[ eGuided ] = &pGuidanceGroup->GetConnector();
    builder.AddField< ADN_CheckBox >( pGuidanceGroup, tr( "Must Maintain guidance" ), vConnectors[ eMaintainGuidance ] );
    builder.AddField< ADN_EditLine_Double >( pGuidanceGroup, tr( "Illumination range needed" ), vConnectors[ eGuidanceRange ], 0, eGreaterEqualZero );

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
    pContentLayout->addWidget( pPackagingGroup );
    pContentLayout->addWidget( pDirectGroup );
    pContentLayout->addWidget( pIndirectGroup );
    pContentLayout->addWidget( pIlluminationGroup );
    pContentLayout->addWidget( pGuidanceGroup );

    // List view
    ADN_SearchListView< ADN_Equipement_AmmoListView >* pSearchListView = new ADN_SearchListView< ADN_Equipement_AmmoListView >( data_.GetDotation( eDotationFamily_Munition ).categories_, vConnectors, eDotationFamily_Munition );
    pSearchListView->GetListView()->setObjectName( strClassName_ + "_Ammunition_List" );
    connect( pSearchListView->GetListView(), SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    connect( this, SIGNAL( ApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ), pSearchListView, SLOT( OnApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ) );
    vListViews_.push_back( pSearchListView->GetListView() );
    assert( eDotationFamily_Munition == vListViews_.size() - 1 );

    // Main page
    QWidget* pPage = CreateScrollArea( *pContent, pSearchListView );
    pTabWidget_->addTab( pPage, ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Munition, ENT_Tr_ABC::eToTr ).c_str() );

    IndirectTypeChanged();
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

    IndirectTypeChanged();
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
// Name: ADN_Equipement_GUI::IndirectTypeChanged
// Created: LGY 2012-02-21
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::IndirectTypeChanged()
{
    pExplosiveParametersGroup_->setVisible( buttonGroup_->button( eExplosivePresent )->isChecked() );
    pSmokeParametersGroup_->setVisible( buttonGroup_->button( eSmokePresent )->isChecked() );
    pEffectParametersGroup_->setVisible( buttonGroup_->button( eEffectPresent )->isChecked() );
    pMineParametersGroup_->setVisible( buttonGroup_->button( eMinePresent )->isChecked() );
    pFlareParametersGroup_->setVisible( buttonGroup_->button( eFlarePresent )->isChecked() );
    data_.Initialize();
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
            helpers::AttritionInfos* pAttrition = *it2;

            builder.AddTableCell< ADN_TableItem_String >( pTable, *it, nRow + nSubRow, 1, ( *it2 )->ptrArmor_.GetData()->strName_, eNone, Q3TableItem::Never );

            ADN_TableItem_DoublePercentage* pItem0 = builder.AddTableCell< ADN_TableItem_DoublePercentage >( pTable, *it, nRow + nSubRow, 2, ( *it2 )->rRepairNoEvac_, ePercentage );
            static_cast< ADN_DoublePercentageValidator* >( &pItem0->GetValidator() )->AddLinkedValue( pAttrition->rDestroy_ );
            static_cast< ADN_DoublePercentageValidator* >( &pItem0->GetValidator() )->AddLinkedValue( pAttrition->rRepairWithEvac_ );
            pItem0->SetUseColor( true );

            ADN_TableItem_DoublePercentage* pItem1 = builder.AddTableCell< ADN_TableItem_DoublePercentage >( pTable, *it, nRow + nSubRow, 3, ( *it2 )->rRepairWithEvac_, ePercentage );
            static_cast< ADN_DoublePercentageValidator* >( &pItem1->GetValidator() )->AddLinkedValue( pAttrition->rDestroy_ );
            static_cast< ADN_DoublePercentageValidator* >( &pItem1->GetValidator() )->AddLinkedValue( pAttrition->rRepairNoEvac_ );
            pItem1->SetUseColor( true );

            ADN_TableItem_DoublePercentage* pItem2 = builder.AddTableCell< ADN_TableItem_DoublePercentage >( pTable, *it, nRow + nSubRow, 4, ( *it2 )->rDestroy_, ePercentage );
            static_cast< ADN_DoublePercentageValidator* >( &pItem2->GetValidator() )->AddLinkedValue( pAttrition->rRepairNoEvac_ );
            static_cast< ADN_DoublePercentageValidator* >( &pItem2->GetValidator() )->AddLinkedValue( pAttrition->rRepairWithEvac_ );
            pItem2->SetUseColor( true );
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
    assert( vListViews_[ eDotationFamily_Munition ] != 0 );
    vListViews_[ eDotationFamily_Munition ]->SetCurrentItem( &infos );
    builder.Section( tr( "PKs" ) );
    if( pAttritionTable_ )
        builder.CreateTableFrom( *pAttritionTable_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::NetworkUsableActivated
// Created: ABR 2012-02-29
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::NetworkUsableActivated( int state )
{
    assert( pTabWidget_ );
    int currentIndex = pTabWidget_->currentIndex();
    ADN_Equipement_Data::T_CategoryInfos_Vector& networkUsableVector = data_.GetNetworkUsableDotation();
    assert( vListViews_[ currentIndex ] && vNetworkUsableCheckBoxs_[ currentIndex ] );
    ADN_Equipement_Data::CategoryInfo* current = static_cast< ADN_Equipement_Data::CategoryInfo* >( vListViews_[ currentIndex ]->GetCurrentData() );
    if( !current )
        return;

    bool founded = false;
    for( ADN_Equipement_Data::CIT_CategoryInfos_Vector it = networkUsableVector.begin(); it != networkUsableVector.end() && !founded; ++it )
        if( ( *it )->nMosId_ == current->nMosId_ )
            founded = true;

    if( state == Qt::Checked && !founded )
    {
        networkUsableVector.AddItem( current );
    }
    else if( state == Qt::Unchecked && founded )
    {
        ADN_ResourceNetworks_Data::T_ResourceNetworkInfosVector& resourceNetworks = ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos();
        QString associatedResourceNetwork;
        for( ADN_ResourceNetworks_Data::IT_ResourceNetworkInfosVector it = resourceNetworks.begin(); it != resourceNetworks.end(); ++it )
        {
            ADN_ResourceNetworks_Data::ResourceNetworkInfos* networkInfo = *it;
            if( networkInfo )
            {
                ADN_Equipement_Data::CategoryInfo* equipmentInfo = networkInfo->ptrCategory_.GetData();
                if( equipmentInfo && equipmentInfo->nMosId_ == current->nMosId_ )
                    associatedResourceNetwork += ((*it)->strName_.GetData() + '\n').c_str();
            }
        }
        if( !associatedResourceNetwork.isEmpty() )
        {
            int result = QMessageBox::warning( 0,
                tr( "Warning" ),
                tr( "This will delete all the following associated resource network :" ) + "\n\n" + associatedResourceNetwork + '\n' + tr( "Proceed anyway ?" ),
                QMessageBox::Ok, QMessageBox::Cancel );
            if( result == QMessageBox::Cancel )
            {
                vNetworkUsableCheckBoxs_[ currentIndex ]->setCheckState( Qt::Checked );
                return;
            }
        }
        networkUsableVector.RemItem( current );
    }
}
