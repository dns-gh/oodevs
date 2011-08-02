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
#include <Qt3Support/q3frame.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3vgroupbox.h>
#include <Qt3Support/q3hgroupbox.h>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3grid.h>
#include <Qt3Support/q3whatsthis.h>
#include <QtGui/qtabwidget.h>
//Added by qt3to4:
#include <Qt3Support/q3gridlayout.h>

#define HAL_FILTER "Script files (*.hal)"

//-----------------------------------------------------------------------------
// Name: ADN_Models_GUI constructor
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Models_GUI::ADN_Models_GUI( ADN_Models_Data& data )
    : ADN_GUI_ABC( "ADN_Models_GUI" )
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

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );
    QTabWidget* pTabWidget = new QTabWidget( pMainWidget_ );

    pTabWidget->addTab( BuildPage( pGroupPawn_, pTabWidget, ADN_Models_Data::ModelInfos::ePawn       ), tr( "Unit models"       ) );
    pTabWidget->addTab( BuildPage( pGroupAutomat_, pTabWidget, ADN_Models_Data::ModelInfos::eAutomat    ), tr( "Automata models"   ) );
    pTabWidget->addTab( BuildPage( pGroupPopulation_, pTabWidget, ADN_Models_Data::ModelInfos::ePopulation ), tr( "Crowds models" ) );

    Q3GridLayout* pMainLayout = new Q3GridLayout( pMainWidget_, 1, 1, 10, 10 );
    pMainLayout->addWidget( pTabWidget, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_GUI::BuildPage
// Created: APE 2005-02-09
// -----------------------------------------------------------------------------
QWidget* ADN_Models_GUI::BuildPage( Q3VGroupBox*& pGroup, QWidget* pParent, ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType )
{
    ADN_GuiBuilder builder;

    // Create the main widget
    QWidget* pMainWidget = new QWidget( pParent );

    // Model listview
    T_ConnectorVector vInfosConnectors( eNbrGuiElements,(ADN_Connector_ABC*)0 );
    ADN_ListView_Models* pListModels = new ADN_ListView_Models( eEntityType, pMainWidget);
    if( eEntityType == ADN_Models_Data::ModelInfos::eAutomat )
        pListModels->GetConnector().Connect( &data_.GetAutomataModelsInfos() );
    else if( eEntityType == ADN_Models_Data::ModelInfos::ePawn )
        pListModels->GetConnector().Connect( &data_.GetUnitModelsInfos() );
    else
        pListModels->GetConnector().Connect( &data_.GetPopulationModelsInfos() );

    // Model data
    pGroup = new Q3VGroupBox( tr( "Model"), pMainWidget );

    QWidget* pParamHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pParamHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField<ADN_EditLine_String>( pParamHolder, tr( "DIA type" ), vInfosConnectors[eDiaType] );
    builder.AddFileField( pParamHolder, tr( "File" ), vInfosConnectors[eFile] );
    builder.AddField<ADN_CheckBox>( pParamHolder, tr( "Masalife" ), vInfosConnectors[eMasalife] );

    // Missions
    Q3GroupBox* pMissionsGroup = new Q3HGroupBox( tr( "Missions" ), pGroup );
    ADN_ListView_Missions* pListMissions = new ADN_ListView_Missions( eEntityType, pListModels, pMissionsGroup );
    vInfosConnectors[eMissions] = &pListMissions->GetConnector();

    ADN_ListView_Orders* pListOrders = new ADN_ListView_Orders( true, pMissionsGroup );
    T_ConnectorVector vMissionConnector( eNbrMissionGuiElements, (ADN_Connector_ABC*)0 );
    vMissionConnector[eOrders] = &pListOrders->GetConnector();

    Q3GroupBox* pFragOdersGroup = new Q3HGroupBox( tr( "FragOrders" ), pGroup );
    ADN_ListView_Orders* pListFragOrders = new ADN_ListView_Orders( false , pFragOdersGroup );
    vInfosConnectors[eFragOrders] = &pListFragOrders->GetConnector();

    pListMissions->SetItemConnectors( vMissionConnector );

    // Connect the gui to the data.
    pListModels->SetItemConnectors(vInfosConnectors);

    // Layout
    Q3BoxLayout* pLayout = new Q3HBoxLayout( pMainWidget, 5, 5 );
    pLayout->addWidget( pListModels, 1 );
    pLayout->addWidget( pGroup, 5 );

    return pMainWidget;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_GUI::Enable
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
void ADN_Models_GUI::Enable( bool enable )
{
    pGroupPawn_->setEnabled( enable );
    pGroupAutomat_->setEnabled( enable );
    pGroupPopulation_->setEnabled( enable );
}
