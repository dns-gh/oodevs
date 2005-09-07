// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_GUI.cpp $
// $Author: Ape $
// $Modtime: 18/04/05 11:38 $
// $Revision: 12 $
// $Workfile: ADN_NBC_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_NBC_GUI.h"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_NBC_Datas.h"
#include "ADN_CommonGfx.h"
#include "ADN_NBC_NbcAgentListView.h"
#include "ADN_GuiBuilder.h"

#include "ADN_Graph.h"
#include "ADN_GraphValue.h"
#include "ADN_GraphData.h"
#include "ADN_Connector_Graph_ABC.h"
#include "ADN_Validator.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qgrid.h>


//-----------------------------------------------------------------------------
// Internal Canvas connector 
//-----------------------------------------------------------------------------
class ADN_GC_Nbc : public ADN_Connector_Graph_ABC
{
    MT_COPYNOTALLOWED( ADN_GC_Nbc )

public:
    enum E_Member
    {
        rNbAlivedHumans_,
        rNbHurtedHumans1_,
        rNbHurtedHumans2_,
        rNbHurtedHumans3_,
        rNbHurtedHumansE_,
        rNbDeadHumans_,
        rLifeTime_,
        rSpreadAngle_,
        eNbrMembers
    };

public:
    ADN_GC_Nbc(ADN_Graph& graph, ADN_GraphData& graphData, E_Member eMember ) 
        : ADN_Connector_Graph_ABC( graph )
        , graphData_             ( graphData )
        , eMember_               ( eMember )
    {}

    virtual ~ADN_GC_Nbc()
    {}

    ADN_GraphValue* CreateValue( void* pObj )
    {
        ADN_NBC_Datas::NbcAgentInfos* pInfo = static_cast<ADN_NBC_Datas::NbcAgentInfos*>(pObj);
        // Create the new graph value (it will auto-add itself to the graphData).
        ADN_GraphValue* pNew = 0;
        switch( eMember_ )
        {
            case rNbAlivedHumans_ : pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rNbAlivedHumans_ ); break;
            case rNbHurtedHumans1_: pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rNbHurtedHumans1_ ); break;
            case rNbHurtedHumans2_: pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rNbHurtedHumans2_ ); break;
            case rNbHurtedHumans3_: pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rNbHurtedHumans3_ ); break;
            case rNbHurtedHumansE_: pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rNbHurtedHumansE_ ); break;
            case rNbDeadHumans_   : pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rNbDeadHumans_ ); break;
            case rLifeTime_       : pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rLifeTime_ ); break;
            case rSpreadAngle_    : pNew = new ADN_GraphValue( graphData_, pObj, pInfo->rSpreadAngle_ ); break;
        }

        return pNew;
    }

private:
    ADN_GraphData& graphData_;
    E_Member eMember_;
};


// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI constructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_GUI::ADN_NBC_GUI( ADN_NBC_Datas& data )
: ADN_GUI_ABC( "ADN_NBC_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI destructor
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
ADN_NBC_GUI::~ADN_NBC_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI::Build
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
void ADN_NBC_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    // NBC agents list
    ADN_NBC_NbcAgentListView* pNBCAgentListView = new ADN_NBC_NbcAgentListView(pMainWidget_);
    pNBCAgentListView->GetConnector().Connect( &data_.GetNbcAgentVector() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // NBC agent data
    QGroupBox* pGroup = new QGroupBox( 1, Qt::Horizontal, tr( "NBC Agent" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vInfosConnectors[eName] );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Lifetime" ), vInfosConnectors[eLifetime], tr( "s" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Spread angle" ), vInfosConnectors[eSpreadAngle], tr( "°" ) );
    builder.SetValidator( new ADN_DoubleValidator( 0.01, 360, 2, this ) );

    QGroupBox* pAttritionsGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Human attritions" ), pGroup );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "Unhurt" ), vInfosConnectors[eNbrOk], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "U1" ), vInfosConnectors[eNbrHurt1], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "U2" ), vInfosConnectors[eNbrHurt2], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "U3" ), vInfosConnectors[eNbrHurt3], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "UE" ), vInfosConnectors[eNbrHurt4], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pAttritionsGroup, tr( "Killed" ), vInfosConnectors[eNbrDead], tr( "%" ), ePercentage );

    /*
    // Consistency graphs
    QGroupBox* pPlotGroup = new QGroupBox( 4, Qt::Horizontal, tr( "Consistency graphs" ), pMainWidget_ );

    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 0, tr( "Unhurt" ), tr( "%" ), 0.0, 100.0 );
    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 1, tr( "U1" ), tr( "%" ), 0.0, 100.0 );
    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 2, tr( "U2" ), tr( "%" ), 0.0, 100.0 );
    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 3, tr( "U3" ), tr( "%" ), 0.0, 100.0 );
    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 4, tr( "UE" ), tr( "%" ), 0.0, 100.0 );
    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 5, tr( "Killed" ), tr( "%" ), 0.0, 100.0 );
    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 6, tr( "Lifetime" ), tr( "s" ), 0.0, 0.0 );
    this->BuildGraph( pPlotGroup, *pNBCAgentListView, 7, tr( "Spread angle" ), tr( "°" ), 0.0, 360.0 );
    */

    pNBCAgentListView->SetItemConnectors( vInfosConnectors );

    // Main vertical layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( pMainWidget_, 10, 10 );

    // Top horizontal layout
    QBoxLayout* pListAndDataLayout = new QHBoxLayout( 0, 10, 10 );
    pListAndDataLayout->addWidget( pNBCAgentListView, 1 );
    pListAndDataLayout->addWidget( pGroup, 4 );

    pMainLayout->addLayout( pListAndDataLayout, 2 );
    //pMainLayout->addWidget( pPlotGroup, 3 );
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_GUI::BuildGraph
// Created: APE 2004-12-22
// -----------------------------------------------------------------------------
QWidget* ADN_NBC_GUI::BuildGraph( QWidget* pParent, ADN_ListView& listView, int nVarId, const QString& strCaption, const QString& strSuffix, double rMin, double rMax )
{
    ADN_Graph* pGraph = new ADN_Graph( pParent );
    pGraph->SetSelectionMode( ADN_Graph::eSingle );
    pGraph->YAxis().ShowAxis( true );
    pGraph->YAxis().ShowTicks( 50 );
    pGraph->YAxis().ShowSubTicks( 10 );
    pGraph->YAxis().ShowTicksValue( true );
    pGraph->YAxis().SetAxisCaption( strSuffix.ascii() );
    pGraph->XAxis().SetAxisCaption( strCaption.ascii() );
    if( rMin == 0.0 && rMax == 0.0 )
        pGraph->YAxis().SetAxisRange( rMin, rMax, false );
    else
        pGraph->YAxis().SetAxisRange( rMin, rMax, true );

    pGraph->XAxis().ShowAxis( true );

    ADN_GraphData* pGraphData = new ADN_GraphData( 0, *pGraph );
    pGraphData->SetBarStyle( QPen(Qt::black), QBrush(Qt::green), true, 0.8, ADN_GraphData::eAlign_Left );
    pGraphData->SetPointPen( Qt::NoPen );
    pGraphData->SetLinePen ( Qt::NoPen );
    pGraph->RegisterPlotData( *pGraphData );

    ADN_GC_Nbc* pConnector = new ADN_GC_Nbc( *pGraph, *pGraphData, (ADN_GC_Nbc::E_Member)nVarId );
    pConnector->Connect( &data_.GetNbcAgentVector() );
    pGraph->Connect( listView );
    pGraph->setMinimumSize( 200, 200 );

    return pGraph;
}
