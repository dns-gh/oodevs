// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Templates.h"
#include "ADN_Tr.h"
#include "ADN_Urban_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Urban_GUI.h"
#include "ADN_GuiTools.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates constructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_ListView_Templates::ADN_ListView_Templates( QWidget* pParent, const char* szName, Qt::WFlags f )
    :  ADN_ListView( pParent, szName, f )
{
    addColumn( tools::translate( "ADN_ListView_UrbanAccommodation_Type", szName ) );
    setSorting( -1, true );
    setResizeMode( Q3ListView::AllColumns );
    pConnector_ = new ADN_Connector_ListView< ADN_Urban_Data::UrbanTemplateInfos >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates destructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_ListView_Templates::~ADN_ListView_Templates()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates::ConnectItem
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
void ADN_ListView_Templates::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Urban_Data::UrbanTemplateInfos* pInfos = ( ADN_Urban_Data::UrbanTemplateInfos* )pCurData_;

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanUsageGuiElements );

    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageColor ]->Connect( &pInfos->color_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageAlpha ]->Connect( &pInfos->alpha_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageHeight ]->Connect( &pInfos->height_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageFloorNumber ]->Connect( &pInfos->floor_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageParkingFloors ]->Connect( &pInfos->parking_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageOccupation ]->Connect( &pInfos->occupation_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageTrafficability ]->Connect( &pInfos->trafficability_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageMaterial ]->Connect( &pInfos->ptrMaterial_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageRoofShape ]->Connect( &pInfos->ptrRoofShape_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanUsageRole ]->Connect( &pInfos->usages_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Templates::OnContextMenu
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
void ADN_ListView_Templates::OnContextMenu( const QPoint& point )
{
    Q3PopupMenu popuMenu( this );

    popuMenu.insertItem( tools::translate( "ADN_ListView_Templates", "New" ), 0 );
    popuMenu.insertItem( tools::translate( "ADN_ListView_Templates", "Delete" ), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );

    int nResult = popuMenu.exec( point );

    switch ( nResult )
    {
    case 0:
        {
            ADN_Urban_Data::UrbanTemplateInfos* pNewInfo = new ADN_Urban_Data::UrbanTemplateInfos();
            ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
            pCList->AddItem( pNewInfo );
            int position = FindNdx( pNewInfo );
            while( position != 0 )
            {
                static_cast< ADN_Connector_Vector_ABC* >( &GetConnector() )->SwapItem( position - 1, position );
                --position;
            }
            setCurrentItem( FindItem( pNewInfo ) );
            break;
        }
    case 1:
        {
            ADN_Urban_Data::UrbanTemplateInfos* pCurrent = (ADN_Urban_Data::UrbanTemplateInfos*) pCurData_;
            if( pCurrent->IsMultiRef() && ! ADN_GuiTools::MultiRefWarning() )
                return;
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrent );
            break;
        }
    default:
        break;
    }
}
