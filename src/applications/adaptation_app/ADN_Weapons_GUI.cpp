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
#include "ADN_Weapons_Data_WeaponInfos.h"
#include "ADN_Weapons_Data_PhInfos.h"
#include "ADN_Weapons_ListView.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_EditLine.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Resources_Data.h"
#include "ADN_GoToButton.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_Weapons_PhSizeListView.h"
#include "ADN_Weapons_PhTable.h"
#include "ADN_MainWindow.h"
#include "ADN_GuiBuilder.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_Resources_GUI.h"
#include "ADN_TimeField.h"
#include "ADN_Graph.h"
#include "ADN_GraphData.h"
#include "ADN_GraphValue.h"
#include "ADN_Connector_Graph_ABC.h"
#include "GQ_PlotAxis.h"
#include "GQ_PlotCaption.h"
#include "ENT/ENT_Tr.h"

namespace
{
//-----------------------------------------------------------------------------
// Internal Canvas connector
//-----------------------------------------------------------------------------
class ADN_GC_Ph : public ADN_Connector_Vector_ABC
                , private boost::noncopyable
{
public:
    explicit ADN_GC_Ph( ADN_GraphData& graphData )
        : ADN_Connector_Vector_ABC()
        , graphData_( graphData )
    {
        // NOTHING
    }

    virtual ~ADN_GC_Ph()
    {
        // NOTHING
    }

    bool AddItemPrivate( void* pItem )
    {
        if( pItem == 0 )
            return false;

        ADN_Weapons_Data_PhInfos* pPhInfos = static_cast< ADN_Weapons_Data_PhInfos* >( pItem );
        new ADN_GraphValue( graphData_, pPhInfos, pPhInfos->nModifiedDistance_, pPhInfos->rModifiedPerc_ );
        return true;
    }

    bool RemItemPrivate( void* pItem )
    {
        graphData_.DeleteData( pItem );
        ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
        return true;
    }

    void ConnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
    {
        ADN_Connector_ABC::ConnectPrivateSub( static_cast< ADN_Connector_ABC* >( pDataConnector ) );

        pDataConnector->Initialize( *this );
        connect( pDataConnector, SIGNAL( ItemAdded( void* ) ),      this, SLOT( AddItemNoEmit( void* ) ) );
        connect( pDataConnector, SIGNAL( ItemRemoved( void* ) ),    this, SLOT( RemItemNoEmit( void* ) ) );
    }

    void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
    {
        ADN_Connector_ABC::DisconnectPrivateSub( static_cast< ADN_Connector_ABC* >( pDataConnector ) );

        disconnect( pDataConnector, SIGNAL( ItemAdded( void* ) ),      this, SLOT( AddItemNoEmit( void* ) ) );
        disconnect( pDataConnector, SIGNAL( ItemRemoved( void* ) ),    this, SLOT( RemItemNoEmit( void* ) ) );
    }

private:
    ADN_GraphData& graphData_;
};

class ADN_GC_PhSize : public ADN_Connector_Graph_ABC
{
public:
    explicit ADN_GC_PhSize( ADN_Graph& graph )
        : ADN_Connector_Graph_ABC( graph )
        , userId_( 0 )
    {
        // NOTHING
    }

    virtual ~ADN_GC_PhSize()
    {
        // NOTHING
    }

    bool AddItemPrivate( void* pItem )
    {
        if( pItem == 0 )
            return false;

        ADN_Weapons_Data_PhSizeInfos* pPhSizeInfos = static_cast< ADN_Weapons_Data_PhSizeInfos* >( pItem );
        if( !pPhSizeInfos->GetCrossedElement() )
            return false;
        userIds_[ pItem ] = ++userId_;
        ADN_GraphData* pNewData = new ADN_GraphData( userIds_[ pItem ], graph_ );
        QColor color;
        color.setHsv( nNbrDatas_ * 100, 255, 255 );
        GQ_PlotData::E_PointShapeType nPointShape = static_cast< GQ_PlotData::E_PointShapeType >( nNbrDatas_ % ( GQ_PlotData::eUserShape - 1 ) + 1 );
        pNewData->SetPointShape( nPointShape );
        pNewData->SetPointPen( QPen( color ) );
        pNewData->SetLinePen( QPen( color ) );

        pNewData->SetName( pPhSizeInfos->strName_.GetData().c_str() );

        graph_.RegisterPlotData( *pNewData );
        nNbrDatas_++;

        ADN_GC_Ph* pConnector = new ADN_GC_Ph( *pNewData );
        pNewData->SetConnector( *pConnector );
        pConnector->Connect( &pPhSizeInfos->vPhs_ );
        return true;
    }

    bool RemItemPrivate( void* pItem )
    {
        std::map< void*, unsigned int >::iterator itId = userIds_.find( pItem );
        if( itId == userIds_.end() )
        {
            assert( false );
            return false;
        }
        GQ_PlotData* pPlotData = graph_.FindPlotData( itId->second );
        userIds_.erase( itId );
        if( pPlotData == 0 )
            return false;
        // Unregister and destroy it.
        graph_.UnregisterPlotData( *pPlotData, true );
        ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
        return true;
    }

    void ClearPrivate( bool /*bInConnection*/ = false )
    {
        userIds_.clear();
        graph_.UnregisterAllPlotData( true );
        nNbrDatas_ = 0;
    }

    virtual ADN_GraphValue* CreateValue( void* /*pObj*/ ) { return 0; }

    const std::map< void*, unsigned int >& GetUserIds() const
    {
        return userIds_;
    }

private:
    uint nNbrDatas_;
    unsigned int userId_;
    std::map< void*, unsigned int > userIds_;
};

class ADN_Weapon_Table : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Weapon_Table( const QString& objectName, QWidget* pParent = 0 )
        : ADN_Table( objectName, pParent )
    {
        dataModel_.setColumnCount( 5 );
        setSortingEnabled( true );
        setShowGrid( true );
        QStringList horizontalHeaders;
        horizontalHeaders << tools::translate( "ADN_Weapons_GUI", "Weapon system" )
                          << tools::translate( "ADN_Weapons_GUI", "Rnd per burst" )
                          << tools::translate( "ADN_Weapons_GUI", "Burst duration (s)" )
                          << tools::translate( "ADN_Weapons_GUI", "Rnd per reload" )
                          << tools::translate( "ADN_Weapons_GUI", "Reload duration (s)" );
        dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
        horizontalHeader()->setResizeMode( QHeaderView::Stretch );
        horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
        verticalHeader()->setVisible( false );
        delegate_.AddSpinBoxOnColumn( 1 );
        delegate_.AddDelayEditOnColumn( 2 );
        delegate_.AddSpinBoxOnColumn( 3 );
        delegate_.AddDelayEditOnColumn( 4 );
    }
    virtual ~ADN_Weapon_Table() {}
    //@}
};

class ADN_PHs_Table : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_PHs_Table( const QString& objectName, const std::set< int >& distancesSet, QWidget* pParent = 0 )
        : ADN_Table( objectName, pParent )
    {
        dataModel_.setColumnCount( static_cast< int >( distancesSet.size() + 2 ) );
        setSortingEnabled( false );
        setShowGrid( true );
        horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
        horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
        QStringList horizontalHeaders;
        horizontalHeaders << tools::translate( "ADN_Weapons_GUI", "Weapon system" )
                          << tools::translate( "ADN_Weapons_GUI", "Target size" );
        int n = 2;
        for( std::set< int >::const_iterator it = distancesSet.begin(); it != distancesSet.end(); ++it, ++n )
        {
            horizontalHeaders.append( QString::number( *it ) );
            horizontalHeader()->setResizeMode( n, QHeaderView::Fixed );
            horizontalHeader()->resizeSection( n, 64 );
        }
        dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
        verticalHeader()->setVisible( false );
    }
    virtual ~ADN_PHs_Table() {}
    //@}
};
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI constructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Weapons_GUI::ADN_Weapons_GUI( ADN_Weapons_Data& data )
    : ADN_GUI_ABC( eWeapons )
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
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
    builder.SetEnabled( false );
    {
        ADN_GoToButton* goToButton = builder.AddWidget< ADN_GoToButton >( "goto-launchers", ::eLaunchers );
        ADN_ComboBox_Vector* combo = builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "launcher", tr( "Launcher" ), vInfosConnectors[ eLauncher ], 0, eNone, goToButton );
        goToButton->SetLinkedCombo( combo );
        builder.SetEnabled( false );
    }
    {
        ADN_GoToButton* goToButton = builder.AddWidget< ADN_GoToButton >( "goto-ammo", ::eResources, 0 );
        ADN_ComboBox_Vector* combo = builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "ammo", tr( "Ammo" ), vInfosConnectors[eAmmo], 0, eNone, goToButton );
        goToButton->SetLinkedCombo( combo );
        builder.SetEnabled( false );
    }
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, "round-per-burst", tr( "Rounds per burst" ), vInfosConnectors[ eRoundsPerBurst ], 0, eGreaterZero );
    ADN_TimeField* burst = builder.AddField< ADN_TimeField >( pInfoHolder, "burst-duration", tr( "Burst duration" ), vInfosConnectors[ eBurstDuration ], 0, eGreaterZero );
    burst->SetMinimumValueInSecond( 1 );
    builder.AddField< ADN_EditLine_Int >( pInfoHolder, "round-per-reload", tr( "Rounds per reload" ), vInfosConnectors[ eRoundsPerReload ], 0, eGreaterZero );
    ADN_TimeField* reload = builder.AddField< ADN_TimeField >( pInfoHolder, "reload-duration", tr( "Reload duration" ), vInfosConnectors[ eReloadDuration ], 0, eGreaterZero );
    reload->SetMinimumValueInSecond( 1 );

    // Direct group
    ADN_GroupBox2* pDirectGroup = builder.AddWidget< ADN_GroupBox2 >( "direct-fire", tr( "Direct fire" ) );
    pDirectGroup->GetConnector().bAutoHide_ = true;
    pDirectGroup->setCheckable( false );
    vInfosConnectors[ eDirect ] = &pDirectGroup->GetConnector();

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

    graphConnector_.reset( new ADN_GC_PhSize( *pGraph ) );
    vInfosConnectors[ ePhsGraph ] = graphConnector_.get();

    ADN_Weapons_PhSizeListView* pPhSizeListView = builder.AddWidget< ADN_Weapons_PhSizeListView, GQ_Plot&, const std::map< void*, unsigned int >& >( "ph-size", *pGraph, static_cast< ADN_GC_PhSize& >( *graphConnector_ ).GetUserIds() );
    vInfosConnectors[ ePhs ] = &pPhSizeListView->GetConnector();
    T_ConnectorVector vPhConnectors( eNbrPhSizeGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    ADN_Weapons_PhTable* pPhTable = new ADN_Weapons_PhTable( builder.GetChildName( "ph-table" ), vPhConnectors[ ePhVector ] );

    pPhSizeListView->SetItemConnectors( vPhConnectors );

    ADN_GroupBox2* pSimulation = builder.AddWidget< ADN_GroupBox2 >( "simulation", tr( "Simulation" ) );
    vInfosConnectors[ eSimulation ] = &pSimulation->GetConnector();
    connect( pSimulation, SIGNAL( toggled( bool ) ), this, SLOT( ModifiersChanged( bool ) ) );

    QWidget* pModifiersHolder = builder.AddFieldHolder( pSimulation );
    // $$$$ LDC Code review: Should be extracted in its own method
    pModifiersHolder->layout()->setAlignment( Qt::AlignTop );
    ADN_ComboBox* pFirePostureCombo = builder.AddEnumField( pModifiersHolder, "fire-posture", tr( "Fire posture" ), vInfosConnectors[ eFirePosture ] );
    connect( pFirePostureCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pTargetPostureCombo = builder.AddEnumField( pModifiersHolder, "target-posture", tr( "Target posture" ), vInfosConnectors[ eTargetPosture ] );
    connect( pTargetPostureCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pExperienceCombo = builder.AddEnumField( pModifiersHolder, "experience", tr( "Experience" ), vInfosConnectors[ eExperience ] );
    connect( pExperienceCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pTirednessCombo = builder.AddEnumField( pModifiersHolder, "tiredness", tr( "Tiredness" ), vInfosConnectors[ eTiredness ] );
    connect( pTirednessCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );
    ADN_ComboBox* pStressCombo = builder.AddEnumField( pModifiersHolder, "stress", tr( "Stress" ), vInfosConnectors[ eStress ] );
    connect( pStressCombo, SIGNAL( activated( int ) ), this, SLOT( ModifiersChanged( int ) ) );

    // Indirect group
    ADN_GroupBox* pIndirectGroup = builder.AddGroupBox( 0, "indirect-fire", tr( "Indirect fire" ), vInfosConnectors[ eIndirect ], 3 );
    pIndirectGroup->GetConnector().bAutoHide_ = true;
    pIndirectGroup->setCheckable( false );

    builder.AddField< ADN_EditLine_Double >( pIndirectGroup, "average-speed", tr( "Average speed" ), vInfosConnectors[ eAverageSpeed ], tr( "km/h" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroup, "min-range", tr( "Min range" ), vInfosConnectors[ eMinRange ], tr( "m" ), eGreaterEqualZero );
    builder.AddField< ADN_EditLine_Double >( pIndirectGroup, "max-range", tr( "Max range" ), vInfosConnectors[ eMaxRange ], tr( "m" ), eGreaterEqualZero );

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
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Weapons_ListView >( this, data_.GetWeaponInfos(), vInfosConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::CreateWeaponsTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
QWidget* ADN_Weapons_GUI::CreateWeaponsTable()
{
    ADN_Weapon_Table* pTable = new ADN_Weapon_Table( std::string( strClassName_ + "_weapon-systems-consistency-table" ).c_str() );
    auto& weapons = data_.weapons_;

    int nRow = 0;
    for( auto it = weapons.begin(); it != weapons.end(); ++it, ++nRow )
    {
        ADN_Weapons_Data_WeaponInfos& weapon = **it;
        pTable->AddItem( nRow, 0, &weapon, weapon.strName_.GetData().c_str() );
        pTable->AddItem( nRow, 1, &weapon, &weapon.nRoundsPerBurst_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 2, &weapon, &weapon.burstDuration_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 3, &weapon, &weapon.nRoundsPerReload_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
        pTable->AddItem( nRow, 4, &weapon, &weapon.reloadDuration_ , ADN_StandardItem::eDelay, Qt::ItemIsEditable );
    }
    pTable->Sort();
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::CreatePHTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
QWidget* ADN_Weapons_GUI::CreatePHTable()
{
    // Compute the existing distances
    std::set< int > distancesSet;
    auto& weapons = data_.weapons_;
    for( auto it = weapons.begin(); it != weapons.end(); ++it )
    {
        auto& phsSizeInfos = ( *it )->phs_;
        for( auto it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2 )
        {
            auto& phs = ( *it2 )->vPhs_;
            for( auto it3 = phs.begin(); it3 != phs.end(); ++it3 )
                distancesSet.insert( ( *it3 )->nDistance_.GetData() );
        }
    }

    ADN_Table* pTable = new ADN_PHs_Table( std::string( strClassName_ + "_ph-consistency-table" ).c_str(), distancesSet );
    // Fill the table.
    int nRow = 0;
    for( auto it = weapons.begin(); it != weapons.end(); ++it )
    {
        if( !( *it )->bDirect_.GetData() )
            continue;
        auto& phsSizeInfos = ( *it )->phs_;
        pTable->setNumRows( pTable->numRows() + static_cast< int >( phsSizeInfos.size() ) );
        pTable->AddBoldGridRow( nRow );
        pTable->AddItem( nRow, 0, static_cast< int >( phsSizeInfos.size() ), 1, *it, ( *it )->strName_.GetData().c_str() );

        int nSubRow = 0;
        for( auto it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2 )
        {
            if( !(*it2)->GetCrossedElement() )
                continue;
            if( nSubRow > 0 )
                pTable->AddItem( nRow + nSubRow, 0, *it, ( *it )->strName_.GetData().c_str() );
            pTable->AddItem( nRow + nSubRow, 1, *it, &(*it2)->strName_, ADN_StandardItem::eString );
            auto& phs = (*it2)->vPhs_;
            for( auto it3 = phs.begin(); it3 != phs.end(); ++it3 )
            {
                std::set< int >::iterator itFound = distancesSet.find( ( *it3 )->nDistance_.GetData() );
                int nIndex = static_cast< int >( std::distance( distancesSet.begin(), itFound ) );
                int row = nRow + nSubRow;
                int col = 2 + nIndex;
                pTable->GetDelegate().AddDoubleSpinBox( row, row, col, col, 0, 100, 1., 5 );
                pTable->GetDelegate().AddColor( row, row, col, col, 0, 100 );
                pTable->AddItem( row, col, *it, &(*it3)->rPerc_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
            }
            ++nSubRow;
        }
        nRow += static_cast< int >( phsSizeInfos.size() );
    }
    pTable->Sort();
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::RegisterTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "Weapon systems" ), boost::bind( &ADN_Weapons_GUI::CreateWeaponsTable, this ) );
    mainWindow.AddTable( tr( "PHs" ), boost::bind( &ADN_Weapons_GUI::CreatePHTable, this ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const tools::Path& strPath )
{
    tools::Path strLocalPath = strPath / tools::Path::FromUnicode( tr( "WeaponSystems/" ).toStdWString() );
    strLocalPath.CreateDirectories();
    ADN_HtmlBuilder indexBuilder;
    indexBuilder.BeginHtml( tr( "Weapon Systems" ) );
    indexBuilder.BeginList();

    auto& weapons = data_.weapons_;
    int n = 0;
    for( auto it = weapons.begin(); it != weapons.end(); ++it )
    {
        ADN_Weapons_Data_WeaponInfos& weapon = **it;
        if( !weapon.ptrLauncher_.GetData() || !weapon.ptrAmmunition_.GetData() )
            continue;
        tools::Path strFileName = tools::Path::FromUnicode( tr( "WeaponSystem_%1.htm" ).arg( n ).toStdWString() );
        QString strLink = QString( "<a href=\"" ) + strFileName.ToUTF8().c_str() + "\">" + weapon.strName_.GetData().c_str() + "</a>";
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
        auto& phsSizeInfos = weapon.phs_;
        for( auto it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2 )
        {
            auto& phs = (*it2)->vPhs_;
            for( auto it3 = phs.begin(); it3 != phs.end(); ++it3 )
                distancesSet.insert( (*it3)->nDistance_.GetData() );
        }

        if( weapon.bDirect_.GetData() && ! phsSizeInfos.empty() && ! distancesSet.empty() )
        {
            builder.Section( "PHs" );
            builder.BeginTable( static_cast< int >( phsSizeInfos.size() + 1 ), static_cast< int >( distancesSet.size() + 1 ) );

            int nRow = 1;
            for( auto it2 = phsSizeInfos.begin(); it2 != phsSizeInfos.end(); ++it2 )
            {
                if( !( *it2 )->GetCrossedElement() )
                    continue;
                builder.TableItem( nRow, 0, (*it2)->strName_.GetData().c_str() );
                auto& phs = (*it2)->vPhs_;
                for( auto it3 = phs.begin(); it3 != phs.end(); ++it3 )
                {
                    std::set<int>::iterator itFound = distancesSet.find( (*it3)->nDistance_.GetData() );
                    int nIndex = static_cast< int >( std::distance( distancesSet.begin(), itFound ) );
                    builder.TableItem( nRow, nIndex + 1, (*it3)->rPerc_.GetData() );
                    builder.TableItem( 0, nIndex + 1, QString( "%1m").arg( (*it3)->nDistance_.GetData() ).toStdString().c_str() );
                }
                ++nRow;
            }
            builder.EndTable();
        }

        ADN_Workspace::GetWorkspace().GetLaunchers().GetGui().ExportPHModifiers( builder, *(weapon.ptrLauncher_.GetData()) );
        ADN_Workspace::GetWorkspace().GetResources().GetGui().ExportPKs( builder, *(weapon.ptrAmmunition_.GetData()) );

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
        builder.WriteToFile( strLocalPath / strFileName );
        ++n;
    }
    indexBuilder.EndList();
    indexBuilder.EndHtml();
    indexBuilder.WriteToFile( strLocalPath / "index.htm" );

    QString strText = "<a href=\"" + tr( "WeaponSystems/" ) + "index.htm\">" + tr( "Weapon Systems" ) + "</a>";
    mainIndexBuilder.ListItem( strText );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_GUI::UpdateModifiers
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
void ADN_Weapons_GUI::UpdateModifiers()
{
    ADN_Weapons_Data_WeaponInfos* pInfos = static_cast< ADN_Weapons_Data_WeaponInfos* >( pListView_->GetCurrentData() );
    if( pInfos == 0 )
        return;

    double phModifier = 1.;
    double distanceModifier = 1.;

    if( pInfos->bSimulation_.GetData() != 0 && pInfos->ptrLauncher_.GetData() )
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

    for( auto phSizeInfosIt = pInfos->phs_.begin(); phSizeInfosIt != pInfos->phs_.end(); ++phSizeInfosIt )
    {
        for( auto phInfosIt = ( *phSizeInfosIt )->vPhs_.begin(); phInfosIt != ( *phSizeInfosIt )->vPhs_.end(); ++phInfosIt )
        {
            ADN_Weapons_Data_PhInfos* ph = *phInfosIt;
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
