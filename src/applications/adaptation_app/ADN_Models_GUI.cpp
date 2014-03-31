//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Models_GUI.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:26 $
// $Revision: 11 $
// $Workfile: ADN_Models_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Models_GUI.h"
#include "moc_ADN_Models_GUI.cpp"
#include "ADN_Models_Data.h"
#include "ADN_FileChooser.h"
#include "ADN_CommonGfx.h"
#include "ADN_ListView_Models.h"
#include "ADN_ListView_Missions.h"
#include "ADN_ListView_Orders.h"
#include "ADN_GuiBuilder.h"
#include <boost/lexical_cast.hpp>

//-----------------------------------------------------------------------------
// Name: ADN_Models_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Models_GUI::ADN_Models_GUI( ADN_Models_Data& data )
    : ADN_Tabbed_GUI_ABC( eModels )
    , data_( data )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_GUI destructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Models_GUI::~ADN_Models_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Models_GUI::Build
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
void ADN_Models_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    // Tab management
    pTabWidget_ = new QTabWidget( pMainWidget_ );
    pTabWidget_->addTab( BuildPage( eEntityType_Pawn, data_.GetModels( eEntityType_Pawn ) ), tr( "Unit models" ) );
    pTabWidget_->addTab( BuildPage( eEntityType_Automat, data_.GetModels( eEntityType_Automat ) ), tr( "Automata models" ) );
    pTabWidget_->addTab( BuildPage( eEntityType_Population, data_.GetModels( eEntityType_Population ) ), tr( "Crowds models" ) );

    // Main widget
    pMainWidget_ = new QWidget();
    pMainWidget_->setObjectName( strClassName_ );
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_GUI::BuildPage
// Created: APE 2005-02-09
// -----------------------------------------------------------------------------
QWidget* ADN_Models_GUI::BuildPage( E_EntityType eEntityType, ADN_Models_Data::T_ModelInfos_Vector& model )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( std::string( ADN_Tr::ConvertFromEntityType( eEntityType, ENT_Tr::eToSim ) + "-tab" ).c_str() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements,static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    builder.AddLocalizedField( model, pInfoHolder, "name", tr( "Name" ), vInfosConnectors[ eName ] );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, "dia-type", tr( "DIA type" ), vInfosConnectors[ eDiaType ] );
    DIAFileChooser_[ eEntityType ] = builder.AddFileField( pInfoHolder, "file", tr( "File" ), vInfosConnectors[ eFile ] );
    DIAFileChooser_[ eEntityType ]->SetRestrictions( data_.GetSourcePaths() );
    builder.AddField< ADN_CheckBox >( pInfoHolder, "masalife", tr( "DIA5" ), vInfosConnectors[ eMasalife ] );

    // Missions
    Q3GroupBox* pMissionsGroup = new Q3HGroupBox( tr( "Missions" ) );
    ADN_ListView_Missions* pListMissions = builder.AddWidget< ADN_ListView_Missions >( "mission-list", eEntityType, pMissionsGroup );
    vInfosConnectors[ eMissions ] = &pListMissions->GetConnector();
    pListMissions->SetGoToOnDoubleClick( ::eMissions, static_cast< int >( eEntityType ) );

    ADN_ListView_Orders* pListOrders = builder.AddWidget< ADN_ListView_Orders >( "orders-list", true, pMissionsGroup );
    T_ConnectorVector vMissionConnector( eNbrMissionGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    vMissionConnector[ eOrders ] = &pListOrders->GetConnector();
    pListOrders->SetGoToOnDoubleClick( ::eMissions, 3 ); // Frag orders tabulation

    // Frag order
    Q3GroupBox* pFragOdersGroup = new Q3HGroupBox( QString::fromStdString( ENT_Tr::ConvertFromMissionType( eMissionType_FragOrder ) ) );
    ADN_ListView_Orders* pListFragOrders = builder.AddWidget< ADN_ListView_Orders >( "fragorders-list", false , pFragOdersGroup );
    vInfosConnectors[ eFragOrders ] = &pListFragOrders->GetConnector();
    pListFragOrders->SetGoToOnDoubleClick( ::eMissions, 3 ); // Frag orders tabulation

    // Connect
    pListMissions->SetItemConnectors( vMissionConnector );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* content = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( content );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pMissionsGroup );
    pContentLayout->addWidget( pFragOdersGroup );

    // List view
    QWidget* pSearchListView = builder.AddSearchListView< ADN_ListView_Models >( this, eEntityType, model, vInfosConnectors, static_cast< int >( eEntityType ) );

    // Main page
    builder.PopSubName(); //eEntityType-tab
    return CreateScrollArea( builder.GetName(), *content, pSearchListView );
}
