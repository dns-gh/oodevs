//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Categories_GUI.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:56 $
// $Revision: 14 $
// $Workfile: ADN_Urban_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Urban_GUI.h"

#include "moc_ADN_Urban_GUI.cpp"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Urban_Data.h"
#include "ADN_ListView_Urban_Type.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Tr.h"
#include "ADN_TimeField.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qpushbutton.h>
#include <qwhatsthis.h>


//-----------------------------------------------------------------------------
// Name: ADN_Urban_GUI constructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_GUI::ADN_Urban_GUI( ADN_Urban_Data& data )
: ADN_GUI_ABC       ( "ADN_Urban_GUI" )
, data_             ( data )
, pListMaterial_    ( 0 )
, pListFacade_      ( 0 )
, pListRoofShape_   ( 0 )
{
    //NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_GUI destructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_GUI::~ADN_Urban_GUI()
{
    //NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_GUI::Build
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
void ADN_Urban_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;
    ADN_EditLine_ABC* pEdit = 0;  

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "Urban main widget" );

    // horizontal layout
    QHBoxLayout* pLayout = new QHBoxLayout( pMainWidget_ );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    QVBox* pBox = new QVBox( pMainWidget_ );

    ///////////////////
    // Materials
    QGroupBox* pGroupMaterials = new QVGroupBox( tr( "Materials" ), pBox );
    QHBox* pGroupMaterial = new QHBox(pGroupMaterials);  

    // materials list
    T_ConnectorVector    vMaterialInfosConnectors(eNbrUrbanGuiElements,(ADN_Connector_ABC*)0 );
    pListMaterial_=new ADN_ListView_Urban_Type(pGroupMaterial);
    static_cast<ADN_Connector_Vector_ABC*>( &pListMaterial_->GetConnector() )->Connect( &data_.GetMaterialsInfos() );
    
    // material
    pGroupMaterials = new QVGroupBox( tr( "Material" ),pGroupMaterials);
    pGroupMaterials->setInsideMargin(20);
    pGroupMaterials->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroupMaterials);
    vMaterialInfosConnectors[eUrbanName]=&pEdit->GetConnector();

    ///////////////////
    // Facades
    QGroupBox* pGroupFacades = new QVGroupBox( tr( "Facades" ), pBox );
    QHBox* pGroupFacade = new QHBox(pGroupFacades);  

    // facades list
    T_ConnectorVector    vFacadeInfosConnectors(eNbrUrbanGuiElements,(ADN_Connector_ABC*)0 );
    pListFacade_=new ADN_ListView_Urban_Type(pGroupFacade);
    static_cast<ADN_Connector_Vector_ABC*>( &pListFacade_->GetConnector() )->Connect( &data_.GetFacadesInfos() );

    // facade
    pGroupFacades = new QVGroupBox( tr( "Facade" ),pGroupFacades);
    pGroupFacades->setInsideMargin(20);
    pGroupFacades->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroupFacades);
    vFacadeInfosConnectors[eUrbanName]=&pEdit->GetConnector();

    ///////////////////
    // RoofShapes
    QGroupBox* pGroupRoofShapes = new QVGroupBox( tr( "RoofShapes" ), pBox );
    QHBox* pGroupRoofShape = new QHBox(pGroupRoofShapes);  

    // roofshapes list
    T_ConnectorVector    vRoofShapeInfosConnectors(eNbrUrbanGuiElements,(ADN_Connector_ABC*)0 );
    pListRoofShape_=new ADN_ListView_Urban_Type(pGroupRoofShape);
    static_cast<ADN_Connector_Vector_ABC*>( &pListRoofShape_->GetConnector() )->Connect( &data_.GetRoofShapesInfos() );

    // roofsape
    pGroupRoofShapes = new QVGroupBox( tr( "RoofShape" ),pGroupRoofShapes);
    pGroupRoofShapes->setInsideMargin(20);
    pGroupRoofShapes->setInsideSpacing(20);
    pEdit = new ADN_EditLine_String(pGroupRoofShapes);
    vRoofShapeInfosConnectors[eUrbanName]=&pEdit->GetConnector();

    // set auto connectors
    pListMaterial_->SetItemConnectors( vMaterialInfosConnectors );
    pListFacade_  ->SetItemConnectors( vFacadeInfosConnectors );
    pListRoofShape_->SetItemConnectors( vRoofShapeInfosConnectors );

}