// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources_GUI.h"
#include "moc_ADN_Resources_GUI.cpp"
#include "ADN_ComboBoxItem.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Resources_AmmoListView.h"
#include "ADN_Resources_AttritionGraph.h"
#include "ADN_Resources_AttritionTable.h"
#include "ADN_Resources_GenericListView.h"
#include "ADN_Resources_Postures_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_GuiBuilder.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_MainWindow.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_Tr.h"
#include "ADN_UrbanModifiersTable.h"
#include "ENT/ENT_Tr.h"
#include "protocol/Protocol.h"

class ADN_Resources_UrbanModifiersTable : public helpers::ADN_UrbanModifiersTable
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Resources_UrbanModifiersTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 )
                 : ADN_UrbanModifiersTable( objectName, connector, pParent ) {}
    virtual ~ADN_Resources_UrbanModifiersTable() {}
    //@}

protected:
    //! @name Operations
    //@{
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
    {
        ADN_UrbanModifiersTable::dataChanged( topLeft, bottomRight );
        if( topLeft == bottomRight )
            ADN_Workspace::GetWorkspace().GetResources().GetGui().UpdateGraph();
    }
    //@}
};

class ADN_PK_Table: public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_PK_Table( const QString& objectName, QWidget* pParent = 0 ) : ADN_Table( objectName, pParent )
    {
        dataModel_.setColumnCount( 5 );
        QStringList horizontalHeaders;
        horizontalHeaders << tools::translate( "ADN_Resources_GUI", "Ammunition" )
                          << tools::translate( "ADN_Resources_GUI", "Target armor" )
                          << tools::translate( "ADN_Resources_GUI", "On site fixable" )
                          << tools::translate( "ADN_Resources_GUI", "Maintenance support needed" )
                          << tools::translate( "ADN_Resources_GUI", "Destroyed" );
        dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
        horizontalHeader()->setResizeMode( QHeaderView::Stretch );
        verticalHeader()->setVisible( false );
        setAlternatingRowColors( false );
        setSortingEnabled( false );
        setShowGrid( true );
        delegate_.AddDoubleSpinBoxOnColumn( 2, 0, 100 );
        delegate_.AddDoubleSpinBoxOnColumn( 3, 0, 100 );
        delegate_.AddDoubleSpinBoxOnColumn( 4, 0, 100 );
        delegate_.AddColorOnColumn( 2, 0, 100 );
        delegate_.AddColorOnColumn( 3, 0, 100 );
        delegate_.AddColorOnColumn( 4, 0, 100 );
        int myints[] = { 2, 3, 4 };
        std::vector< int > columns( myints, myints + sizeof( myints ) / sizeof( int ) );
        delegate_.SetColumnsSumRestriction( columns, gui::CommonDelegate::eLTE, 100. );
    }
    virtual ~ADN_PK_Table() {}
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI constructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Resources_GUI::ADN_Resources_GUI( ADN_Resources_Data& data )
    : ADN_Tabbed_GUI_ABC( eSupplies )
    , data_           ( data )
    , buttonGroup_( 0 )
    , pExplosiveParametersGroup_( 0 )
    , pFlareParametersGroup_( 0 )
    , pEffectParametersGroup_( 0 )
    , pSmokeParametersGroup_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI destructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Resources_GUI::~ADN_Resources_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::Build
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Tab management
    pTabWidget_ = new QTabWidget();
    for( int i = sword::DotationType_MIN; i <= sword::DotationType_MAX; ++i )
        BuildGeneric( static_cast< sword::DotationType >( i ) );

    // Main widget
    pMainWidget_ = new QWidget();
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget_ );
    pMainWidget_->setObjectName( strClassName_ );

    connect( pTabWidget_, SIGNAL( currentChanged( int ) ), this, SLOT( OnCurrentTabChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::BuildGeneric
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::BuildGeneric( sword::DotationType nType )
{
    if( nType == sword::dotation_type_ammunition )
        return BuildAmmunition();

    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( ENT_Tr::ConvertFromDotationType( nType, ENT_Tr::eToSim ).c_str() );
    T_ConnectorVector vConnectors( eNbrGenericGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddLocalizedField( data_.GetResources(), pInfoHolder, "name", tr( "Name" ), vConnectors[ eName ] );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-nno", tr( "Code NNO" ), vConnectors[ eGenNNOCode ], optionalWidgets_ );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-emat8", tr( "Code EMAT8" ), vConnectors[ eGenEMAT8Code ], optionalWidgets_ );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-emat6", tr( "Code EMAT6" ), vConnectors[ eGenEMAT6Code ], optionalWidgets_ );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-lfril", tr( "Code LFRIL" ), vConnectors[ eGenLFRILCode ], optionalWidgets_ );
    if( ADN_Resources_Data::IsMineOrExplosive( nType ) )
        builder.AddEnumField( pInfoHolder, "type", tr( "Type" ), vConnectors[ eGenType ] );
    builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "nature", tr( "Nature" ), vConnectors[ eGenNature] );
    builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "logistic-supply-class", tr( "Class" ), vConnectors[ eGenLogisticSupplyClass] );
    ADN_CheckBox* networkUsableCheckBox = builder.AddField< ADN_CheckBox >( pInfoHolder, "resource-network-usable", tr( "Usable within a resource network" ), vConnectors[ eGenNetworkUsable ] );
    vNetworkUsablecheckboxes_.push_back( networkUsableCheckBox );
    connect( networkUsableCheckBox, SIGNAL( stateChanged( int ) ), SLOT( NetworkUsableActivated( int ) ) );

    // Packaging
    Q3GroupBox* pPackagingGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Packaging" ) );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, "package-nbr", tr( "Nbr per package" ), vConnectors[ ePackageNbr ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, "package-weight", tr( "Package weight" ), vConnectors[ ePackageWeight ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, "package-volume", tr( "Package volume" ), vConnectors[ ePackageVolume ], tr( "m3" ), eGreaterZero );

    // Attritions
    Q3GroupBox* pDirectGroup = 0;
    if( ADN_Resources_Data::IsMineOrExplosive( nType ) )
    {
        pDirectGroup = new Q3GroupBox( tr( "Attritions" ) );
        pAttritionTables_[ nType ] = new ADN_Resources_AttritionTable( builder.GetChildName( "attritions-table" ), vConnectors[ eGenAttritions ], pDirectGroup );
        ADN_Resources_UrbanModifiersTable* pUrbanTable = new ADN_Resources_UrbanModifiersTable( builder.GetChildName( "urban-modifiers-table" ), vConnectors[ eGenUrbanAttritions ], pDirectGroup );

        QGroupBox* pAttritionVisualisation = new gui::RichGroupBox( "simulation", tr( "Simulation" ) );
        QWidget* pComboGroup = builder.AddFieldHolder( pAttritionVisualisation );
        pArmorCombos_[ nType ] = builder.AddField< ADN_ComboBox_Vector >( pComboGroup, "armor-plating", tr( "Armor-Plating" ), vConnectors[ eGenArmor ] );
        connect( pArmorCombos_[ nType ], SIGNAL( activated( int ) ), this, SLOT( SimulationCombosActivated() ) );
        pMaterialCombos_[ nType ] = builder.AddField< ADN_ComboBox_Vector >( pComboGroup, "urban-material", tr( "Urban material" ), vConnectors[ eGenMaterial ] );
        connect( pMaterialCombos_[ nType ], SIGNAL( activated( int ) ), this, SLOT( SimulationCombosActivated() ) );
        pAttritionGraphs_[ nType ] = new ADN_Resources_AttritionGraph( pAttritionVisualisation );
        vConnectors[ eGenAttritionGraph ] = &pAttritionGraphs_[ nType ]->GetConnector();

        QVBoxLayout* pAttritionVisualisationLayout = new QVBoxLayout();
        pAttritionVisualisationLayout->addWidget( pComboGroup );
        pAttritionVisualisationLayout->addWidget( pAttritionGraphs_[ nType ], 1 );
        pAttritionVisualisation->setLayout( pAttritionVisualisationLayout );

        QGridLayout* pDirectGroupLayout = new QGridLayout();
        pDirectGroupLayout->addWidget( pAttritionTables_[ nType ], 0, 0 );
        pDirectGroupLayout->addWidget( pUrbanTable, 1, 0 );
        pDirectGroupLayout->addWidget( pAttritionVisualisation, 0, 1, 2, 1 );
        pDirectGroup->setLayout( pDirectGroupLayout );
    }

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
    if( ADN_Resources_Data::IsMineOrExplosive( nType ) )
        pContentLayout->addWidget( pDirectGroup );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Resources_GenericListView >( this, nType, data_.GetResource( nType ).categories_, vConnectors, nType );
    assert( nType == static_cast< int >( vListViews_.size() - 1) );

    // Main page
    pTabWidget_->addTab( CreateScrollArea( builder.GetName(), *pContent, pSearchListView ), ENT_Tr::ConvertFromDotationType( nType, ENT_Tr::eToTr ).c_str() );
    builder.PopSubName(); //! tab name
}

namespace
{
    void CreateCheckbox( Q3GroupBox* parent, T_ConnectorVector& vConnectors, ADN_GuiBuilder& builder, const char* objectName,
                         QString name, QButtonGroup* group, ADN_Resources_GUI::E_AmmoGuiElements id )
    {
        ADN_CheckBox* checkbox = new ADN_CheckBox( name, parent );
        checkbox->setObjectName( builder.GetChildName( objectName ) );
        vConnectors[ id ] = &checkbox->GetConnector();
        group->addButton( checkbox, id );
    }
}
// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::BuildAmmunition
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::BuildAmmunition()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( ENT_Tr::ConvertFromDotationType( sword::dotation_type_ammunition, ENT_Tr::eToSim ).c_str() );
    T_ConnectorVector vConnectors( eNbrAmmoGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddLocalizedField( data_.GetResources(), pInfoHolder, "name", tr( "Name" ), vConnectors[ eAmmoName ] );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-nno", tr( "Code NNO" ), vConnectors[ eNNOCode ], optionalWidgets_ );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-emat8", tr( "Code EMAT8" ), vConnectors[ eEMAT8Code ], optionalWidgets_ );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-emat6", tr( "Code EMAT6" ), vConnectors[ eEMAT6Code ], optionalWidgets_ );
    builder.AddOptionalField< ADN_EditLine_String >( pInfoHolder, "code-lfril", tr( "Code LFRIL" ), vConnectors[ eLFRILCode ], optionalWidgets_ );
    builder.AddEnumField( pInfoHolder, "type", tr( "Type" ), vConnectors[ eType ] );
    builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "nature", tr( "Nature" ), vConnectors[ eNature ] );
    builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "logistic-supply-class", tr( "Class" ), vConnectors[ eLogisticSupplyClass] );
    ADN_CheckBox* networkUsableCheckBox = builder.AddField< ADN_CheckBox >( pInfoHolder, "resource-network-usable", tr( "Usable within a resource network" ), vConnectors[ eNetworkUsable ] );
    vNetworkUsablecheckboxes_.push_back( networkUsableCheckBox );
    connect( networkUsableCheckBox, SIGNAL( stateChanged( int ) ), SLOT( NetworkUsableActivated( int ) ) );
    builder.AddField< ADN_CheckBox >( pInfoHolder, "improvised-explosive", tr( "Improvised explosive device" ), vConnectors[ eIsIED ] );

    // Packaging
    Q3GroupBox* pPackagingGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Packaging" ) );
    builder.PushSubName( "packaging" );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, "nbr", tr( "Nbr per package" ), vConnectors[ eAmmoPackageNbr ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, "weight", tr( "Package weight" ), vConnectors[ eAmmoPackageWeight ], tr( "T" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pPackagingGroup, "volume", tr( "Package volume" ), vConnectors[ eAmmoPackageVolume ], tr( "m3" ), eGreaterZero );
    builder.PopSubName();

    // Direct fire properties
    builder.PushSubName( "attritions" );
    ADN_GroupBox* pDirectGroup = builder.AddGroupBox( 0, "attritions", tr( "Attritions" ), vConnectors[ eDirect ] );
    pAttritionTables_[ sword::dotation_type_ammunition ] = new ADN_Resources_AttritionTable( builder.GetChildName( "attritions-table" ), vConnectors[ eAttritions ], pDirectGroup );
    ADN_Resources_UrbanModifiersTable* pUrbanTable = new ADN_Resources_UrbanModifiersTable( builder.GetChildName( "urban-modifiers" ), vConnectors[ eUrbanAttritions ], pDirectGroup );

    QGroupBox* pAttritionVisualisation = new gui::RichGroupBox( "simulation", tr( "Simulation" ) );
    builder.PushSubName( "simulation" );
    QWidget* pComboGroup = builder.AddFieldHolder( pAttritionVisualisation );
    pArmorCombos_[ sword::dotation_type_ammunition ] = builder.AddField< ADN_ComboBox_Vector >( pComboGroup, "armor-plating", tr( "Armor-Plating" ), vConnectors[ eArmor ] );
    connect( pArmorCombos_[ sword::dotation_type_ammunition ], SIGNAL( activated( int ) ), this, SLOT( SimulationCombosActivated() ) );
    pMaterialCombos_[ sword::dotation_type_ammunition ] = builder.AddField< ADN_ComboBox_Vector >( pComboGroup, "urban-material", tr( "Urban material" ), vConnectors[ eMaterial ] );
    connect( pMaterialCombos_[ sword::dotation_type_ammunition ], SIGNAL( activated( int ) ), this, SLOT( SimulationCombosActivated() ) );
    pAttritionGraphs_[ sword::dotation_type_ammunition ] = new ADN_Resources_AttritionGraph( pAttritionVisualisation );
    vConnectors[ eAttritionGraph ] = &pAttritionGraphs_[ sword::dotation_type_ammunition ]->GetConnector();
    builder.PopSubName(); //! simulation
    builder.PopSubName(); //! attrition

    QVBoxLayout* pAttritionVisualisationLayout = new QVBoxLayout();
    pAttritionVisualisationLayout->addWidget( pComboGroup );
    pAttritionVisualisationLayout->addWidget( pAttritionGraphs_[ sword::dotation_type_ammunition ], 1 );
    pAttritionVisualisation->setLayout( pAttritionVisualisationLayout );

    QGridLayout* pDirectGroupLayout = new QGridLayout();
    pDirectGroupLayout->addWidget( pAttritionTables_[ sword::dotation_type_ammunition ], 0, 0 );
    pDirectGroupLayout->addWidget( pUrbanTable, 1, 0 );
    pDirectGroupLayout->addWidget( pAttritionVisualisation, 0, 1, 2, 1 );
    pDirectGroup->setLayout( pDirectGroupLayout );

    // Indirect fire properties
    builder.PushSubName( "indirect-fire" );
    ADN_GroupBox* pIndirectGroup = builder.AddGroupBox( 0, "indirect-fire", tr( "Indirect fire" ), vConnectors[ eIndirect ], 1 );
    QWidget* pIndirectGroupHolder = builder.AddFieldHolder( pIndirectGroup );
    builder.AddField< ADN_EditLine_Int >( pIndirectGroupHolder, "intervention", tr( "Intervention" ), vConnectors[ eIntervention ], 0, eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroupHolder, "x-dispersion", tr( "X Dispersion" ), vConnectors[ eDispersionX ], tr( "m" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroupHolder, "y-dispersion", tr( "Y Dispersion" ), vConnectors[ eDispersionY ], tr( "m" ), eGreaterZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroupHolder, "detection-range", tr( "Sound detection range" ), vConnectors[ eDetectionRange ], tr( "m" ), eGreaterZero );

    Q3GroupBox* pEffects = new Q3GroupBox( 5, Qt::Horizontal, tr( "Effects" ), pIndirectGroup );
    {
        builder.PushSubName( "effects" );
        buttonGroup_ = new QButtonGroup( pEffects );
        buttonGroup_->setExclusive( false );

        CreateCheckbox( pEffects, vConnectors, builder, "explosive", tr( "Explosive" ), buttonGroup_, eExplosivePresent );
        CreateCheckbox( pEffects, vConnectors, builder, "smoke", tr( "Smoke" ), buttonGroup_, eSmokePresent );
        CreateCheckbox( pEffects, vConnectors, builder, "illumination-shell", tr( "Illumination shell" ), buttonGroup_, eFlarePresent );
        CreateCheckbox( pEffects, vConnectors, builder, "effect", tr( "Effect" ), buttonGroup_, eEffectPresent );
        connect( buttonGroup_, SIGNAL( buttonClicked( int ) ), this, SLOT( IndirectTypeChanged() ) );
        builder.PopSubName();
    }

    QWidget* pEffectsInfo = new Q3VBox( pIndirectGroup );
    {
        // Explosive parameters
        pExplosiveParametersGroup_ = new Q3GroupBox( 1, Qt::Horizontal, tr( "Explosive ammo parameters" ), pEffectsInfo );
        builder.PushSubName( "param-explosive" );
        new ADN_Resources_Postures_GUI( builder.GetChildName( "ammunitions-stance" ), tr( "Stance" ), vConnectors[ eModifStances ], pExplosiveParametersGroup_ );
        QWidget* pExplosiveParametersGroupHolder = builder.AddFieldHolder( pExplosiveParametersGroup_ );
        builder.AddField< ADN_EditLine_Double >( pExplosiveParametersGroupHolder, "neutralization-ratio", tr( "Neutralization ratio" ), vConnectors[ eNeutralizationRatio ] );
        builder.SetValidator( new ADN_DoubleValidator( 1, INT_MAX, 2, this ) );
        builder.AddField< ADN_EditLine_Double >( pExplosiveParametersGroupHolder, "dispersion-factor", tr( "Dispersion factor per iteration" ), vConnectors[ eExplosiveDispersion ], 0, eZeroOne );
        builder.PopSubName();

        // Smoke parameters
        pSmokeParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Smoke ammo parameters" ), pEffectsInfo );
        builder.PushSubName( "param-smoke" );
        builder.AddField< ADN_TimeField >( pSmokeParametersGroup_, "activation-duration", tr( "Activation duration" ), vConnectors[ eSmokeDeployTime ] );
        builder.AddField< ADN_TimeField >( pSmokeParametersGroup_, "span", tr( "Span" ), vConnectors[ eSmokeLifetime ] );
        builder.AddField< ADN_EditLine_Double >( pSmokeParametersGroup_, "dispersion-factor", tr( "Dispersion factor per iteration" ), vConnectors[ eSmokeDispersion ], 0, eZeroOne );
        builder.PopSubName();

        // Flare parameters
        pFlareParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Flare ammo parameters" ), pEffectsInfo );
        builder.PushSubName( "param-flare" );
        builder.AddField< ADN_TimeField >( pFlareParametersGroup_, "activation-duration", tr( "Activation duration" ), vConnectors[ eFlareDeployTime ] );
        builder.AddField< ADN_TimeField >( pFlareParametersGroup_, "span", tr( "Span" ), vConnectors[ eFlareLifetime ] );
        builder.AddField< ADN_EditLine_Double >( pFlareParametersGroup_, "dispersion-factor", tr( "Dispersion factor per iteration" ), vConnectors[ eFlareDispersion ], 0, eZeroOne );
        builder.PopSubName();

        // Effect (object) parameters
        pEffectParametersGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Effect ammo parameters" ), pEffectsInfo );
        builder.PushSubName( "param-effect" );
        builder.AddField< ADN_ComboBox_Vector >( pEffectParametersGroup_, "created-object", tr( "Created object" ), vConnectors[ eEffectType ] );
        builder.AddField< ADN_TimeField >( pEffectParametersGroup_, "span", tr( "Span" ), vConnectors[ eEffectLifetime ] );
        builder.PopSubName();
    }
    builder.PopSubName(); //! indirect-fire

    // Illumination
    builder.PushSubName( "illumination" );
    ADN_GroupBox* pIlluminationGroup = builder.AddGroupBox( 0, "illumination", tr( "Illumination capacity" ), vConnectors[ eIlluminating ], 1 );
    builder.AddField< ADN_EditLine_Double >( pIlluminationGroup, "range", tr( "Range" ), vConnectors[ eRange ], 0, eGreaterEqualZero );
    builder.AddField< ADN_CheckBox >( pIlluminationGroup, "must-maintain", tr( "Must Maintain illumination" ), vConnectors[ eMaintainIllumination ] );
    builder.PopSubName(); //! illumination

    // Guidance
    builder.PushSubName( "guidance" );
    ADN_GroupBox* pGuidanceGroup = builder.AddGroupBox( 0, "guidance", tr( "Guidance" ), vConnectors[ eGuided ], 1 );
    builder.AddField< ADN_CheckBox >( pGuidanceGroup, "must-maintain", tr( "Must Maintain guidance" ), vConnectors[ eMaintainGuidance ] );
    builder.AddField< ADN_EditLine_Double >( pGuidanceGroup, "illumination-range", tr( "Illumination range needed" ), vConnectors[ eGuidanceRange ], 0, eGreaterEqualZero );
    builder.PopSubName();

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
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Resources_AmmoListView >( this, data_.GetResource( sword::dotation_type_ammunition ).categories_, vConnectors, sword::dotation_type_ammunition );
    assert( sword::dotation_type_ammunition == vListViews_.size() - 1 );

    // Main page
    pTabWidget_->addTab( CreateScrollArea( builder.GetName(), *pContent, pSearchListView ), ENT_Tr::ConvertFromDotationType( sword::dotation_type_ammunition, ENT_Tr::eToTr ).c_str() );
    builder.PopSubName(); //! tab name

    IndirectTypeChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::UpdateGraph
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::UpdateGraph()
{
    if( pAttritionGraphs_[ currentTab_ ] )
        pAttritionGraphs_[ currentTab_ ]->Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::InitializeArmorCombo
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::InitializeSimulationCombos()
{
    if( pArmorCombos_[ currentTab_ ] )
    {
        for( int i = pArmorCombos_[ currentTab_ ]->count() - 1; i >= 0; --i )
        {
            auto& armorType = static_cast< ADN_Armors_Data::ArmorInfos* >( pArmorCombos_[ currentTab_ ]->GetItem( i )->GetData() )->nType_;
            if( armorType == eProtectionType_Human || armorType == eProtectionType_Crowd )
                pArmorCombos_[ currentTab_ ]->removeItem( i );
        }
        if( pArmorCombos_[ currentTab_ ]->GetItem( 0 ) )
            pArmorCombos_[ currentTab_ ]->setCurrentItem( 0 );
    }

    if( pMaterialCombos_[ currentTab_ ] )
    {
        ADN_ComboBoxItem* noneItem = new ADN_ComboBoxItem( *pMaterialCombos_[ currentTab_ ], 0 );
        noneItem->setText( tr( "None" ) );
        pMaterialCombos_[ currentTab_ ]->insertItem( noneItem, 0 );
        pMaterialCombos_[ currentTab_ ]->setCurrentItem( 0 );
    }

    IndirectTypeChanged();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::GetSelectedArmor
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
ADN_Armors_Data::ArmorInfos* ADN_Resources_GUI::GetSelectedArmor() const
{
    auto it = pArmorCombos_.find( currentTab_ );
    if( it == pArmorCombos_.end() )
        throw MASA_EXCEPTION( "cannot find armor element" );
    return static_cast< ADN_Armors_Data::ArmorInfos* >( it->second->GetCurrentData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::GetSelectedMaterial
// Created: JSR 2010-05-03
// -----------------------------------------------------------------------------
helpers::ADN_UrbanAttritionInfos* ADN_Resources_GUI::GetSelectedMaterial() const
{
    auto it = pMaterialCombos_.find( currentTab_ );
    if( it == pMaterialCombos_.end() )
        throw MASA_EXCEPTION( "cannot find material element" );
    return static_cast< helpers::ADN_UrbanAttritionInfos* >( it->second->GetCurrentData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::IndirectTypeChanged
// Created: LGY 2012-02-21
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::IndirectTypeChanged()
{
    pExplosiveParametersGroup_->setVisible( buttonGroup_->button( eExplosivePresent )->isChecked() );
    pSmokeParametersGroup_->setVisible( buttonGroup_->button( eSmokePresent )->isChecked() );
    pEffectParametersGroup_->setVisible( buttonGroup_->button( eEffectPresent )->isChecked() );
    pFlareParametersGroup_->setVisible( buttonGroup_->button( eFlarePresent )->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::SimulationCombosActivated
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::SimulationCombosActivated()
{
    UpdateGraph();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::CreatePKTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
QWidget* ADN_Resources_GUI::CreatePKTable()
{
    ADN_Armors_Data::T_ArmorInfos_Vector& armorInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos();

    ADN_Table* pTable = new ADN_PK_Table( tr( "PKs" ) );
    // Fill the table.
    ADN_Resources_Data::ResourceInfos& ammo = data_.GetResource( sword::dotation_type_ammunition );
    // to filter out crowd 'armor'
    unsigned int nRowSize = 0;
    for( auto it = armorInfos.begin(); it != armorInfos.end(); ++it )
        if( (*it)->GetType() != eProtectionType_Crowd )
            ++nRowSize;

    int nRow = 0;
    for( auto it = ammo.categories_.begin(); it != ammo.categories_.end(); ++it )
    {
        ADN_Resources_Data::AmmoCategoryInfo& ammoCategory = *static_cast< ADN_Resources_Data::AmmoCategoryInfo* >( *it );
        if( !ammoCategory.bDirect_.GetData() )
            continue;
        pTable->setNumRows( nRow + nRowSize );
        pTable->AddBoldGridRow( nRow );
        pTable->AddItem( nRow, 0, nRowSize, 1 , *it, ammoCategory.strName_.GetData().c_str() );
        int nSubRow = 0;
        for( auto it2 = ammoCategory.attritions_.begin(); it2 != ammoCategory.attritions_.end(); ++it2 )
        {
            if( ( *it2 )->GetCrossedElement()->nType_ == eProtectionType_Crowd )
                continue;
            if( nSubRow > 0 )
                pTable->AddItem( nRow + nSubRow, 0, *it, &ammoCategory.strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
            pTable->AddItem( nRow + nSubRow, 1, *it, &( *it2 )->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
            pTable->AddItem( nRow + nSubRow, 2, *it, &( *it2 )->rRepairNoEvac_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
            pTable->AddItem( nRow + nSubRow, 3, *it, &( *it2 )->rRepairWithEvac_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
            pTable->AddItem( nRow + nSubRow, 4, *it, &( *it2 )->rDestroy_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
            ++nSubRow;
        }
        nRow += nRowSize;
    }
    pTable->Sort();
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::RegisterTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "PKs" ), boost::bind( &ADN_Resources_GUI::CreatePKTable, this ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::ExportPKs
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::ExportPKs( ADN_HtmlBuilder& builder, ADN_Resources_Data::AmmoCategoryInfo& infos )
{
    if( !infos.bDirect_.GetData() )
        return;
    assert( vListViews_[ sword::dotation_type_ammunition ] != 0 );
    vListViews_[ sword::dotation_type_ammunition ]->SetCurrentItem( &infos );
    builder.Section( tr( "PKs" ) );
    if( pAttritionTables_[ sword::dotation_type_ammunition ] )
        builder.CreateTableFrom( *pAttritionTables_[ sword::dotation_type_ammunition ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::NetworkUsableActivated
// Created: ABR 2012-02-29
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::NetworkUsableActivated( int state )
{
    assert( pTabWidget_ );
    int currentIndex = pTabWidget_->currentIndex();
    ADN_Resources_Data::T_CategoryInfos_Vector& networkUsableVector = data_.GetNetworkUsableResources();
    assert( vListViews_[ currentIndex ] && vNetworkUsablecheckboxes_[ currentIndex ] );
    ADN_Resources_Data::CategoryInfo* current = static_cast< ADN_Resources_Data::CategoryInfo* >( vListViews_[ currentIndex ]->GetCurrentData() );
    if( !current )
        return;

    bool founded = false;
    for( auto it = networkUsableVector.begin(); it != networkUsableVector.end() && !founded; ++it )
        if( ( *it )->nId_.GetData() == current->nId_.GetData() )
            founded = true;

    if( state == Qt::Checked && !founded )
    {
        networkUsableVector.AddItem( current );
    }
    else if( state == Qt::Unchecked && founded )
    {
        ADN_ResourceNetworks_Data::T_ResourceNetworkInfosVector& resourceNetworks = ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos();
        QString associatedResourceNetwork;
        for( auto it = resourceNetworks.begin(); it != resourceNetworks.end(); ++it )
        {
            ADN_ResourceNetworks_Data::ResourceNetworkInfos* networkInfo = *it;
            if( networkInfo )
            {
                ADN_Resources_Data::CategoryInfo* equipmentInfo = networkInfo->ptrCategory_.GetData();
                if( equipmentInfo && equipmentInfo->nId_.GetData() == current->nId_.GetData() )
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
                vNetworkUsablecheckboxes_[ currentIndex ]->setCheckState( Qt::Checked );
                return;
            }
        }
        networkUsableVector.RemItem( current );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_GUI::OnCurrentTabChanged
// Created: ABR 2012-11-14
// -----------------------------------------------------------------------------
void ADN_Resources_GUI::OnCurrentTabChanged( int tab )
{
    currentTab_ = static_cast< sword::DotationType >( tab );
}
