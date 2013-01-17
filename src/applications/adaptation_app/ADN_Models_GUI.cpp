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
#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_Models_Data.h"
#include "ADN_CommonGfx.h"
#include "ADN_ListView_Models.h"
#include "ADN_FileChooser.h"
#include "ADN_ListView_Missions.h"
#include "ADN_ListView_Orders.h"
#include "ADN_Project_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_SearchListView.h"
#include <boost/lexical_cast.hpp>

//-----------------------------------------------------------------------------
// Name: ADN_Models_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Models_GUI::ADN_Models_GUI( ADN_Models_Data& data )
    : ADN_Tabbed_GUI_ABC( "ADN_Models_GUI" )
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
    pTabWidget_->addTab( BuildPage( eEntityType_Pawn, data_.GetUnitModelsInfos() ), tr( "Unit models" ) );
    pTabWidget_->addTab( BuildPage( eEntityType_Automat, data_.GetAutomataModelsInfos() ), tr( "Automata models" ) );
    pTabWidget_->addTab( BuildPage( eEntityType_Population, data_.GetPopulationModelsInfos() ), tr( "Crowds models" ) );

    // Main widget
    pMainWidget_ = new QWidget();
    pMainWidget_->setObjectName( strClassName_ );
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget_ );}

// -----------------------------------------------------------------------------
// Name: ADN_Models_GUI::BuildPage
// Created: APE 2005-02-09
// -----------------------------------------------------------------------------
QWidget* ADN_Models_GUI::BuildPage( E_EntityType eEntityType, ADN_Models_Data::T_ModelInfos_Vector& model )
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    const QString builderName = strClassName_ + boost::lexical_cast< std::string >( static_cast< int >( eEntityType ) ).c_str();
    ADN_GuiBuilder builder( builderName );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements,static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    ADN_EditLine_ABC* nameField = builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "Name" ), vInfosConnectors[ eName ] );
    nameField->ConnectWithRefValidity( model );
    builder.AddField< ADN_EditLine_String >( pInfoHolder, tr( "DIA type" ), vInfosConnectors[ eDiaType ] );
    builder.AddFileField( pInfoHolder, tr( "File" ), vInfosConnectors[ eFile ] );
    builder.AddField< ADN_CheckBox >( pInfoHolder, tr( "Masalife" ), vInfosConnectors[ eMasalife ] );

    // Missions
    Q3GroupBox* pMissionsGroup = new Q3HGroupBox( tr( "Missions" ) );
    ADN_ListView_Missions* pListMissions = new ADN_ListView_Missions( eEntityType, pMissionsGroup );
    pListMissions->setObjectName( builderName + "_Missions" );
    vInfosConnectors[ eMissions ] = &pListMissions->GetConnector();
    pListMissions->SetGoToOnDoubleClick( ::eMissions, static_cast< int >( eEntityType ) );

    ADN_ListView_Orders* pListOrders = new ADN_ListView_Orders( true, pMissionsGroup );
    pListOrders->setObjectName( builderName + "_Orders" );
    T_ConnectorVector vMissionConnector( eNbrMissionGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );
    vMissionConnector[ eOrders ] = &pListOrders->GetConnector();
    pListOrders->SetGoToOnDoubleClick( ::eMissions, 3 ); // Frag orders tabulation

    // Frag order
    Q3GroupBox* pFragOdersGroup = new Q3HGroupBox( tr( "FragOrders" ) );
    ADN_ListView_Orders* pListFragOrders = new ADN_ListView_Orders( false , pFragOdersGroup );
    pListFragOrders->setObjectName( builderName + "_FragOrders" );
    vInfosConnectors[ eFragOrders ] = &pListFragOrders->GetConnector();
    pListFragOrders->SetGoToOnDoubleClick( ::eMissions, 3 ); // Frag orders tabulation

    // Connect
    pListMissions->SetItemConnectors( vMissionConnector );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    pWidgets_[ eEntityType ] = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pWidgets_[ eEntityType ] );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pMissionsGroup );
    pContentLayout->addWidget( pFragOdersGroup );

    // List view
    ADN_SearchListView< ADN_ListView_Models >* pSearchListView = new ADN_SearchListView< ADN_ListView_Models >( this, eEntityType, model, vInfosConnectors, static_cast< int >( eEntityType ) );
    pSearchListView->GetListView()->setObjectName( builderName + "_List" );
    vListViews_.push_back( pSearchListView->GetListView() );

    // Main page
    return CreateScrollArea( *pWidgets_[ eEntityType ], pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Models_GUI::Enable( bool enable )
{
    for( int i = 0; i < eNbrEntityTypes; ++i )
        pWidgets_[ i ]->setEnabled( enable );
}
