// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-07 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_PhSizeListView.cpp $
// $Author: Ape $
// $Modtime: 2/03/05 17:27 $
// $Revision: 3 $
// $Workfile: ADN_Weapons_PhSizeListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_PhSizeListView.h"

#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Weapons_GUI.h"
#include "GQ_PlotData.h"
#include "GQ_Plot.h"

typedef ADN_Weapons_Data::PhSizeInfos PhSizeInfos;

//-----------------------------------------------------------------------------
// Internal List View Objects connector to be connected with ADN_ListView_Munitions_PhSizes
//-----------------------------------------------------------------------------
class ADN_CLV_PhSizes : public ADN_Connector_ListView_ABC
                      , private boost::noncopyable
{

public:
    ADN_CLV_PhSizes( ADN_Weapons_PhSizeListView& list, GQ_Plot& plot )
    : ADN_Connector_ListView_ABC( list, "ADN_CLV_PhSizes" )
    , plot_ ( plot )
    {}

    virtual ~ADN_CLV_PhSizes()
    {}

    ADN_ListViewItem* CreateItem( void* pObj )
    {
        // Retrieve the display icon for this item from the plot.
        GQ_PlotData* pData = plot_.FindPlotData( (uint)pObj );
        assert( pData != 0 );
        QPixmap icon;
        pData->DrawCaption( icon );

        // Create a new list item.
        ADN_ListViewItem *pItem = new ADN_ListViewItem( &list_, pObj, 1 );
        pItem->setPixmap( 0, icon );


        // Connect it with the size info.
        pItem->Connect( 0, static_cast<PhSizeInfos*>(pObj)->ptrSize_.GetData() );

        return pItem;
    }

private:
    GQ_Plot& plot_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhSizeListView constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhSizeListView::ADN_Weapons_PhSizeListView( GQ_Plot& plot, QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Target size" ) );
    setResizeMode( Q3ListView::AllColumns );
    setSorting( -1 );

    this->setMaximumHeight( 300 );

    // Connector creation.
    pConnector_ = new ADN_CLV_PhSizes( *this, plot );
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhSizeListView destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhSizeListView::~ADN_Weapons_PhSizeListView()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhSizeListView::ConnectItem
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
void ADN_Weapons_PhSizeListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    PhSizeInfos* pInfos = (PhSizeInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Weapons_GUI::eNbrPhSizeGuiElements );

    vItemConnectors_[ADN_Weapons_GUI::ePhVector]->Connect( &pInfos->vPhs_, bConnect );
}

