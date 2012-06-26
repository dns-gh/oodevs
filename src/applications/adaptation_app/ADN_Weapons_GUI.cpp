// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_GUI.h"
#include "moc_ADN_Weapons_GUI.cpp"

#include "ADN_GuiBuilder.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Weapons_ListView.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_EditLine.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Equipement_Data.h"
#include "ADN_GoToButton.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_Weapons_PhSizeListView.h"
#include "ADN_Weapons_PhTable.h"
#include "ADN_MainWindow.h"
#include "ADN_GuiBuilder.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_TableItem_TimeField.h"
#include "ADN_TimeField.h"
#include "ADN_Graph.h"
#include "ADN_GraphData.h"
#include "ADN_GraphValue.h"
#include "ADN_Connector_Graph_ABC.h"
#include "ADN_SearchListView.h"
#include "GQ_PlotAxis.h"
#include "GQ_PlotCaption.h"
#include "ENT/ENT_Tr.h"

//-----------------------------------------------------------------------------
// Internal Canvas connector
//-----------------------------------------------------------------------------
class ADN_GC_Ph : public ADN_Connector_Vector_ABC
                , private boost::noncopyable
{

public:
    explicit ADN_GC_Ph( ADN_GraphData& graphData )
        : ADN_Connector_Vector_ABC()
        , graphData_ ( graphData )
    {}

    virtual ~ADN_GC_Ph()
    {}

    bool AddItemPrivate( void* pItem, bool /*bCreateCommand*/ = false )
    {
        if( pItem == 0 )
            return false;

        ADN_Weapons_Data::PhInfos* pPhInfos = static_cast<ADN_Weapons_Data::PhInfos*>( pItem );
        new ADN_GraphValue( graphData_, pPhInfos, pPhInfos->nModifiedDistance_ , pPhInfos->rModifiedPerc_ );
        return true;
    }

    bool RemItemPrivate( void* pItem, bool /*bCreateCommand*/ = false )
    {
        graphData_.DeleteData( pItem );
        return true;
    }

    void ConnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
    {
        ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pDataConnector );

        pDataConnector->Initialize( *this );
        connect( pDataConnector, SIGNAL(ItemAdded(void*)),      this, SLOT(AddItemNoEmit(void*)) );
        connect( pDataConnector, SIGNAL(ItemRemoved(void*)),    this, SLOT(RemItemNoEmit(void*)) );
    }

    void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
    {
        ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pDataConnector );

        disconnect( pDataConnector, SIGNAL(ItemAdded(void*)),      this, SLOT(AddItemNoEmit(void*)) );
        disconnect( pDataConnector, SIGNAL(ItemRemoved(void*)),    this, SLOT(RemItemNoEmit(void*)) );
    }

private:
    ADN_GraphData& graphData_;
};

class ADN_GC_PhSize : public ADN_Connector_Graph_ABC
{

public:
    explicit ADN_GC_PhSize( ADN_Graph& graph )
        : ADN_Connector_Graph_ABC( graph )
    {}

    virtual ~ADN_GC_PhSize()
    {}

    bool AddItemPrivate( void* pItem, bool /*bCreateCommand*/ = false )
    {
        if( pItem == 0 )
            return false;

        ADN_Weapons_Data::PhSizeInfos* pPhSizeInfos = static_cast<ADN_Weapons_Data::PhSizeInfos*>(pItem);

        ADN_GraphData* pNewData = new ADN_GraphData( (uint)pItem, graph_ );
        QColor color;
        color.setHsv( nNbrDatas_ * 100, 255, 255 );
        GQ_PlotData::E_PointShapeType nPointShape = (GQ_PlotData::E_PointShapeType)(nNbrDatas_ % (GQ_PlotData::eUserShape-1) + 1);
        pNewData->SetPointShape( nPointShape );
        pNewData->SetPointPen( QPen( color ) );
        pNewData->SetLinePen( QPen( color ) );

        pNewData->SetName( pPhSizeInfos->ptrSize_.GetData()->GetData().c_str() );

        graph_.RegisterPlotData( *pNewData );
        nNbrDatas_++;

        ADN_GC_Ph* pConnector = new ADN_GC_Ph( *pNewData );
        pNewData->SetConnector( *pConnector );
        pConnector->Connect( & pPhSizeInfos->vPhs_ );
        return true;
    }

    bool RemItemPrivate( void* pItem, bool /*bCreateCommand*/ = false )
    {
        GQ_PlotData* pPlotData = graph_.FindPlotData( (uint)pItem );
        if( pPlotData == 0 )
            return false;
        // Unregister and destroy it.
        graph_.UnregisterPlotData( *pPlotData, true );
        return true;
    }

    void ClearPrivate( bool /*bInConnection*/ = false )
    {
        graph_.UnregisterAllPlotData( true );
        nNbrDatas_ = 0;
    }

    virtual ADN_GraphValue* CreateValue( void* /*pObj*/ ) { return 0; }

private:
    uint nNbrDatas_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI constructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Weapons_GUI::ADN_Weapons_GUI( ADN_Weapons_Data& data )
    : ADN_GUI_ABC( "ADN_Weapons_GUI" )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI destructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Weapons_GUI::~ADN_Weapons_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::Build
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField<ADN_EditLine_String>( pInfoHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.SetEnabled( false );
    {
        ADN_GoToButton* goToButton = new ADN_GoToButton( ::eLaunchers );
        goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector<ADN_Launchers_Data::LauncherInfos> >( pInfoHolder, tr( "Launcher" ), vInfosConnectors[eLauncher], 0, eNone, goToButton ) );
        builder.SetEnabled( false );
    }
    {
        ADN_GoToButton* goToButton = new ADN_GoToButton( ::eEquipement, 0 );
        goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector<ADN_Equipement_Data::AmmoCategoryInfo> >( pInfoHolder, tr( "Ammo" ), vInfosConnectors[eAmmo], 0, eNone, goToButton ) );
        builder.SetEnabled( false );
    }
    builder.AddField<ADN_EditLine_Int>( pInfoHolder, tr( "Rounds per burst" ), vInfosConnectors[eRoundsPerBurst], 0, eGreaterZero );
    ADN_TimeField* burst = builder.AddField<ADN_TimeField>( pInfoHolder, tr( "Burst duration" ), vInfosConnectors[eBurstDuration], 0, eGreaterZero );
    burst->SetMinimumValueInSecond( 1 );
    builder.AddField<ADN_EditLine_Int>( pInfoHolder, tr( "Rounds per reload" ), vInfosConnectors[eRoundsPerReload], 0, eGreaterZero );
    ADN_TimeField* reload = builder.AddField<ADN_TimeField>( pInfoHolder, tr( "Reload duration" ), vInfosConnectors[eReloadDuration], 0, eGreaterZero );
    reload->SetMinimumValueInSecond( 1 );

    // Direct group
    ADN_GroupBox2* pDirectGroup = new ADN_GroupBox2( tr( "Direct fire" ) );
    pDirectGroup->GetConnector().bAutoHide_ = true;
    pDirectGroup->setCheckable( false );
    vInfosConnectors[eDirect] = &pDirectGroup->GetConnector();

    ADN_Graph* pGraph = new ADN_Graph(/* pDirectGroup */);
    pGraph->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    pGraph->setMinimumSize( 400, 200 );
    pGraph->YAxis().SetAxisCaption( tr( "Ph (%)" ) );
    pGraph->YAxis().SetAxisRange( 0, 100, true );
    pGraph->YAxis().ShowTicks( 50 );
    pGraph->YAxis().ShowSubTicks( 10 );
    pGraph->YAxis().ShowTicksValue( true );
    pGraph->XAxis().SetAxisCaption( tr( "Distance (m)" ) );
    pGraph->XAxis().ShowTicks( 50 );
    pGraph->XAxis().ShowSubTicks( 10 );
    pGraph->XAxis().ShowTicksValue( true );

    ADN_GC_PhSize* pGraphConnector = new ADN_GC_PhSize( *pGraph );
    vInfosConnectors[ePhsGraph] = pGraphConnector;

    ADN_Weapons_PhSizeListView* pPhSizeListView = new ADN_Weapons_PhSizeListView( *pGraph/*, pDirectGroup*/ );
    vInfosConnectors[ePhs] = &pPhSizeListView->GetConnector();
    T_ConnectorVector vPhConnectors( eNbrPhSizeGuiElements, (ADN_Connector_ABC*)0 );

    ADN_Weapons_PhTable* pPhTable = new ADN_Weapons_PhTable( /*pDirectGroup*/ );
    vPhConnectors[ePhVector] = & pPhTable->GetConnector();

    pPhSizeListView->SetItemConnectors( vPhConnectors );

    ADN_GroupBox2* pSimulation = new ADN_GroupBox2( tr( "Simulation" ) );
    vInfosConnectors[eSimulation] = &pSimulation->GetConnector();
    connect( pSimulation, SIGNAL( toggled( bool ) ), this, SLOT( ModifiersChanged( bool ) ) );

    QWidget* pModifiersHolder = builder.AddFieldHolder( pSimulation );
    // $$$$ LDC Code review: Should be extracted in its own method
    pModifiersHolder->layout()->setAlignment( Qt::AlignTop );
    ADN_ComboBox* pFirePostureCombo = builder.AddEnumField< E_UnitPosture >( pModifiersHolder, tr( "Fire posture" ), vInfosConnectors[eFirePosture], ENT_Tr::ConvertFromUnitPosture );
    connect( pFirePostureCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pTargetPostureCombo = builder.AddEnumField< E_UnitPosture >( pModifiersHolder, tr( "Target posture" ), vInfosConnectors[eTargetPosture], ENT_Tr::ConvertFromUnitPosture );
    connect( pTargetPostureCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pExperienceCombo = builder.AddEnumField< E_UnitExperience >( pModifiersHolder, tr( "Experience" ), vInfosConnectors[eExperience], ENT_Tr::ConvertFromUnitExperience );
    connect( pExperienceCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pTirednessCombo = builder.AddEnumField< E_UnitTiredness >( pModifiersHolder, tr( "Tiredness" ), vInfosConnectors[eTiredness], ENT_Tr::ConvertFromUnitTiredness );
    connect( pTirednessCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pStressCombo = builder.AddEnumField< E_UnitStress >( pModifiersHolder, tr( "Stress" ), vInfosConnectors[eStress], ENT_Tr::ConvertFromUnitStress );
    connect( pStressCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );

    // Indirect group
    ADN_GroupBox* pIndirectGroup = new ADN_GroupBox( 3, Qt::Horizontal, tr( "Indirect fire" ) );
    pIndirectGroup->GetConnector().bAutoHide_ = true;
    pIndirectGroup->setCheckable( false );
    vInfosConnectors[eIndirect] = &pIndirectGroup->GetConnector();

    builder.AddField<ADN_EditLine_Double>( pIndirectGroup, tr( "Average speed" ), vInfosConnectors[eAverageSpeed], tr( "km/h" ), eGreaterEqualZero );
    builder.AddField<ADN_EditLine_Double>( pIndirectGroup, tr( "Min range" ), vInfosConnectors[eMinRange], tr( "m" ), eGreaterEqualZero );
    builder.AddField<ADN_EditLine_Double>( pIndirectGroup, tr( "Max range" ), vInfosConnectors[eMaxRange], tr( "m" ), eGreaterEqualZero );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Simulation layout
    QHBoxLayout* pSimulationLayout = new QHBoxLayout( pSimulation );
    pSimulationLayout->addWidget( pModifiersHolder );

    // Direct layout
    QGridLayout* pDirectLayout = new QGridLayout( pDirectGroup, 2, 3 );
    pDirectLayout->setMargin( 10 );
    pDirectLayout->setSpacing( 10 );
    pDirectLayout->addWidget( pPhSizeListView, 0, 0 );
    pDirectLayout->addWidget( pPhTable, 0, 1 );
    pDirectLayout->addWidget( pSimulation, 0, 2 );
    pDirectLayout->addMultiCellWidget( pGraph, 1, 1, 0, 2 );
    pDirectLayout->setColStretch( 0, 1 );
    pDirectLayout->setColStretch( 1, 2 );
    pDirectLayout->setColStretch( 2, 2 );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pDirectGroup );
    pContentLayout->addWidget( pIndirectGroup);

    // List view
    ADN_SearchListView< ADN_Weapons_ListView >* pSearchListView = new ADN_SearchListView< ADN_Weapons_ListView >( data_.GetWeaponInfos(), vInfosConnectors );
    connect( pSearchListView->GetListView(), SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    connect( this, SIGNAL( ApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ), pSearchListView, SLOT( OnApplyFilterList( const ADN_NavigationInfos::UsedBy& ) ) );
    pListView_ = pSearchListView->GetListView();

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
    pMainWidget_->setObjectName( strClassName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::CreateWeaponsTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
ADN_Table* ADN_Weapons_GUI::CreateWeaponsTable()
{
    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );
    ADN_Weapons_Data::T_WeaponInfosVector& weapons = data_.weapons_;

    // Setup the header.
    pTable->setNumCols( 5 );
    pTable->horizontalHeader()->setLabel( 0, tr( "Weapon system" ) );
    pTable->horizontalHeader()->setLabel( 1, tr( "Rnd per burst" ) );
    pTable->horizontalHeader()->setLabel( 2, tr( "Burst duration (s)" ) );
    pTable->horizontalHeader()->setLabel( 3, tr( "Rnd per reload" ) );
    pTable->horizontalHeader()->setLabel( 4, tr( "Reload duration (s)" ) );
    pTable->horizontalHeader()->show();

    pTable->setNumRows( static_cast< int >( weapons.size() + 1 ) );
    builder.AddTableCell( pTable, 0, 0, tr( "Weapon system" ) );
    builder.AddTableCell( pTable, 0, 1, tr( "Rnd per burst" ) );
    builder.AddTableCell( pTable, 0, 2, tr( "Burst duration (s)" ) );
    builder.AddTableCell( pTable, 0, 3, tr( "Rnd per reload" ) );
    builder.AddTableCell( pTable, 0, 4, tr( "Reload duration (s)" ) );
    pTable->hideRow( 0 );
    pTable->AddBoldGridRow( 1 );

    int nRow = 1;
    for( ADN_Weapons_Data::IT_WeaponInfosVector it = weapons.begin(); it != weapons.end(); ++it, ++nRow )
    {
        ADN_Weapons_Data::WeaponInfos& weapon = **it;

        builder.AddTableCell<ADN_TableItem_String>( pTable, &weapon, nRow, 0, weapon.strName_, eNone, Q3TableItem::Never );
        builder.AddTableCell<ADN_TableItem_Int>(    pTable, &weapon, nRow, 1, weapon.nRoundsPerBurst_, eGreaterEqualZero );
        builder.AddTableCell<ADN_TableItem_TimeField>( pTable, &weapon, nRow, 2, weapon.burstDuration_ );
        builder.AddTableCell<ADN_TableItem_Int>(    pTable, &weapon, nRow, 3, weapon.nRoundsPerReload_, eGreaterZero );
        builder.AddTableCell<ADN_TableItem_TimeField>( pTable, &weapon, nRow, 4, weapon.reloadDuration_ );
    }

    pTable->AdjustColumns( 20 );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::CreatePHTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
ADN_Table* ADN_Weapons_GUI::CreatePHTable()
{
    // Compute the existing distances
    std::set<int> distancesSet;
    ADN_Weapons_Data::T_WeaponInfosVector& weapons = data_.weapons_;
    for( ADN_Weapons_Data::IT_WeaponInfosVector it = weapons.begin(); it != weapons.end(); ++it )
    {
        ADN_Weapons_Data::T_PhSizeInfosVector& phsSizeInfos = (*it)->phs_;
        for( ADN_Weapons_Data::IT_PhSizeInfosVector it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2 )
        {
            ADN_Weapons_Data::T_PhInfosVector& phs = (*it2)->vPhs_;
            for( ADN_Weapons_Data::IT_PhInfosVector it3 = phs.begin(); it3 != phs.end(); ++it3 )
            {
                distancesSet.insert( (*it3)->nDistance_.GetData() );
            }
        }
    }

    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );

    // Setup the header.
    pTable->setNumCols( static_cast< int >( distancesSet.size() + 2 ) );
    pTable->horizontalHeader()->setLabel( 0, tr( "Weapon system" ) );
    pTable->horizontalHeader()->setLabel( 1, tr( "Target size" ) );
    int n = 2;
    for( std::set<int>::iterator it = distancesSet.begin(); it != distancesSet.end(); ++it, ++n )
        pTable->horizontalHeader()->setLabel( n, QString::number( *it ) );
    pTable->horizontalHeader()->show();

    pTable->setNumRows( 1 );
    builder.AddTableCell( pTable, 0, 0, tr( "Weapon system" ) );
    builder.AddTableCell( pTable, 0, 1, tr( "Target size" ) );
    n = 2;
    for( std::set<int>::iterator it = distancesSet.begin(); it != distancesSet.end(); ++it, ++n )
        builder.AddTableCell( pTable, 0, n, QString::number( *it ) );
    pTable->hideRow( 0 );
    pTable->setSorting( false );

    // Fill the table.
    int nRow = 1;
    for( ADN_Weapons_Data::IT_WeaponInfosVector it = weapons.begin(); it != weapons.end(); ++it )
    {
        if( ! (*it)->bDirect_.GetData() )
            continue;

        ADN_Weapons_Data::T_PhSizeInfosVector& phsSizeInfos = (*it)->phs_;
        pTable->setNumRows( pTable->numRows() + static_cast< int >( phsSizeInfos.size() ) );
        pTable->AddBoldGridRow( nRow );

        builder.AddTableCell<ADN_TableItem_String>( pTable, *it, nRow, 0, static_cast< int >( phsSizeInfos.size() ), 1, (*it)->strName_, eNone, Q3TableItem::Never );

        int nSubRow = 0;
        for( ADN_Weapons_Data::IT_PhSizeInfosVector it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2, ++nSubRow )
        {
            builder.AddTableCell<ADN_TableItem_String>( pTable, *it, nRow + nSubRow, 1, * (*it2)->ptrSize_.GetData(), eNone, Q3TableItem::Never );

            // Make sure empty cells are non-editable.
            ADN_Weapons_Data::T_PhInfosVector& phs = (*it2)->vPhs_;
            for( int nn = 2; nn < 2 + (int)distancesSet.size(); ++nn )
            {
                Q3TableItem* pNonEditableDummy = new Q3TableItem( pTable, Q3TableItem::Never );
                pTable->setItem( nRow + nSubRow, nn, pNonEditableDummy );
            }

            for( ADN_Weapons_Data::IT_PhInfosVector it3 = phs.begin(); it3 != phs.end(); ++it3 )
            {
                std::set<int>::iterator itFound = distancesSet.find( (*it3)->nDistance_.GetData() );
                int nIndex = static_cast< int >( std::distance( distancesSet.begin(), itFound ) );
                ADN_TableItem_Double* pItem = builder.AddTableCell<ADN_TableItem_Double>( pTable, *it, nRow + nSubRow, 2 + nIndex, (*it3)->rPerc_, ePercentage );
                pItem->SetUseColor( true );
            }
        }
        nRow += static_cast< int >( phsSizeInfos.size() );
    }

    pTable->AdjustColumns( 50 );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::RegisterTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "Weapon systems" ), new ADN_Callback<ADN_Table*,ADN_Weapons_GUI>( this, & ADN_Weapons_GUI::CreateWeaponsTable ) );
    mainWindow.AddTable( tr( "PHs" ), new ADN_Callback<ADN_Table*,ADN_Weapons_GUI>( this, &ADN_Weapons_GUI::CreatePHTable ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    QString strLocalPath = strPath + tr( "WeaponSystems/" );
    ADN_Tools::CreatePathToFile( strLocalPath.ascii() );
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Weapon Systems" ) );
    indexBuilder.BeginList();

    ADN_Weapons_Data::T_WeaponInfosVector& weapons = data_.weapons_;
    int n = 0;
    for( ADN_Weapons_Data::IT_WeaponInfosVector it = weapons.begin(); it != weapons.end(); ++it, ++n )
    {
        ADN_Weapons_Data::WeaponInfos& weapon = **it;
        QString strFileName = tr( "WeaponSystem_%1.htm" ).arg( n );
        QString strLink = "<a href=\"" + strFileName + "\">" + weapon.strName_.GetData().c_str() + "</a>";
        indexBuilder.ListItem( strLink );

        ADN_HtmlBuilder builder;

        builder.BeginHtml( weapon.strName_.GetData().c_str() );
        builder.Section( "Properties" );
        builder.BeginList();
        builder.ListItem( tr( "Launcher" ), weapon.ptrLauncher_.GetData()->strName_.GetData().c_str() );
        builder.ListItem( tr( "Ammo" ), weapon.ptrAmmunition_.GetData()->strName_.GetData().c_str() );
        builder.ListItem( tr( "Rounds per burst" ), weapon.nRoundsPerBurst_.GetData() );
        builder.ListItem( tr( "Burst duration" ), weapon.burstDuration_.GetData().c_str() );
        builder.ListItem( tr( "Rounds per reload" ), weapon.nRoundsPerReload_.GetData() );
        builder.ListItem( tr( "Reload duration" ), weapon.reloadDuration_.GetData().c_str() );
        builder.EndList();

        // Compute the existing distances
        std::set<int> distancesSet;
        ADN_Weapons_Data::T_PhSizeInfosVector& phsSizeInfos = weapon.phs_;
        for( ADN_Weapons_Data::IT_PhSizeInfosVector it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2 )
        {
            ADN_Weapons_Data::T_PhInfosVector& phs = (*it2)->vPhs_;
            for( ADN_Weapons_Data::IT_PhInfosVector it3 = phs.begin(); it3 != phs.end(); ++it3 )
            {
                distancesSet.insert( (*it3)->nDistance_.GetData() );
            }
        }

        if( weapon.bDirect_.GetData() && ! phsSizeInfos.empty() && ! distancesSet.empty() )
        {
            builder.Section( "PHs" );
            builder.BeginTable( static_cast< int >( phsSizeInfos.size() + 1 ), static_cast< int >( distancesSet.size() + 1 ) );

            int nRow = 1;
            for( ADN_Weapons_Data::IT_PhSizeInfosVector it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2, ++nRow )
            {
                builder.TableItem( nRow, 0, (*it2)->ptrSize_.GetData()->GetData().c_str() );
                ADN_Weapons_Data::T_PhInfosVector& phs = (*it2)->vPhs_;
                for( ADN_Weapons_Data::IT_PhInfosVector it3 = phs.begin(); it3 != phs.end(); ++it3 )
                {
                    std::set<int>::iterator itFound = distancesSet.find( (*it3)->nDistance_.GetData() );
                    int nIndex = static_cast< int >( std::distance( distancesSet.begin(), itFound ) );
                    builder.TableItem( nRow, nIndex + 1, (*it3)->rPerc_.GetData() );
                    builder.TableItem( 0, nIndex + 1, QString( "%1m").arg( (*it3)->nDistance_.GetData() ).toStdString().c_str() );
                }
            }
            builder.EndTable();
        }

        ADN_Workspace::GetWorkspace().GetLaunchers().GetGui().ExportPHModifiers( builder, *(weapon.ptrLauncher_.GetData()) );
        ADN_Workspace::GetWorkspace().GetEquipements().GetGui().ExportPKs( builder, *(weapon.ptrAmmunition_.GetData()) );

        if( weapon.bIndirect_.GetData() )
        {
            builder.Section( "Indirect fire properties" );
            builder.BeginList();
            builder.ListItem( tr( "Average speed" ), weapon.rAverageSpeed_.GetData(), tr( "km/h" ) );
            builder.ListItem( tr( "Min range" ), weapon.rMinRange_.GetData(), tr( "m" ) );
            builder.ListItem( tr( "Max range" ), weapon.rMaxRange_.GetData(), tr( "m" ) );
            builder.EndList();
        }
        builder.EndHtml();
        builder.WriteToFile( strLocalPath + strFileName );
    }
    indexBuilder.EndList();
    indexBuilder.EndHtml();
    indexBuilder.WriteToFile( strLocalPath + "index.htm" );

    QString strText = "<a href=\"" + tr( "WeaponSystems/" ) + "index.htm\">" + tr( "Weapon Systems" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::UpdateModifiers
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::UpdateModifiers()
{
    ADN_Weapons_Data::WeaponInfos* pInfos = ( ADN_Weapons_Data::WeaponInfos* )pListView_->GetCurrentData();
    if( pInfos == 0 )
        return;

    double phModifier = 1.;
    double distanceModifier = 1.;

    if( pInfos->bSimulation_.GetData() != 0 )
    {
        int firePosture = pInfos->nFirePosture_.GetData();
        int targetPosture = pInfos->nTargetPosture_.GetData();

        distanceModifier = pInfos->ptrLauncher_.GetData()->tabModifs_.Get( firePosture, targetPosture ).GetData();
        distanceModifier /= 100;

        E_UnitExperience experience = pInfos->nExperience_.GetData();
        E_UnitTiredness tiredness = pInfos->nTiredness_.GetData();
        E_UnitStress stress = pInfos->nStress_.GetData();

        ADN_HumanFactors_Data& humanFactors = ADN_Workspace::GetWorkspace().GetHumanFactors().GetData();

        switch( experience )
        {
        case eUnitExperience_Conscrit:
            phModifier = humanFactors.newbieModifiers_.rPHModifier_.GetData();
            break;
        case eUnitExperience_Experimente:
            phModifier = humanFactors.xpModifiers_.rPHModifier_.GetData();
            break;
        case eUnitExperience_Veteran:
            phModifier = humanFactors.veteranModifiers_.rPHModifier_.GetData();
            break;
        default:
            break;
        }

        switch( tiredness )
        {
        case eUnitTiredness_Epuise:
            phModifier *= humanFactors.exhaustedModifiers_.rPHModifier_.GetData();
            break;
        case eUnitTiredness_Fatigue:
            phModifier *= humanFactors.tirednessModifiers_.rPHModifier_.GetData();
            break;
        case eUnitTiredness_Normal:
            phModifier *= humanFactors.normalModifiers_.rPHModifier_.GetData();
            break;
        default:
            break;
        }

        switch( stress )
        {
        case eUnitStress_Calm:
            phModifier *= humanFactors.calmModifiers_.rPHModifier_.GetData();
            break;
        case eUnitStress_Worried:
            phModifier *= humanFactors.worriedModifiers_.rPHModifier_.GetData();
            break;
        case eUnitStress_Stressed:
            phModifier *= humanFactors.stressedModifiers_.rPHModifier_.GetData();
            break;
        default:
            break;
        }
    }

    for( ADN_Weapons_Data::IT_PhSizeInfosVector phSizeInfosIt = pInfos->phs_.begin(); phSizeInfosIt != pInfos->phs_.end(); ++phSizeInfosIt )
    {
        for( ADN_Weapons_Data::IT_PhInfosVector phInfosIt = ( *phSizeInfosIt )->vPhs_.begin(); phInfosIt != ( *phSizeInfosIt )->vPhs_.end(); ++phInfosIt )
        {
            ADN_Weapons_Data::PhInfos* ph = *phInfosIt;
            ph->SetPhModifiers( distanceModifier, phModifier );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::ModifiersChanged
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::ModifiersChanged( bool )
{
    UpdateModifiers();
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::ModifiersChanged
// Created: JSR 2010-04-27
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::ModifiersChanged( int )
{
    UpdateModifiers();
}
