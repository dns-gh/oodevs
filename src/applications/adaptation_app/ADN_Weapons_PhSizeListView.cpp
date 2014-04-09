// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_PhSizeListView.h"

#include "ADN_Connector_ListView_ABC.h"
#include "ADN_StandardItem.h"
#include "ADN_Weapons_Data_PhInfos.h"
#include "ADN_Weapons_GUI.h"
#include "ADN_Tr.h"
#include "GQ_PlotData.h"
#include "GQ_Plot.h"

typedef ADN_Weapons_Data_PhSizeInfos PhSizeInfos;

//-----------------------------------------------------------------------------
// Internal List View Objects connector to be connected with ADN_ListView_Munitions_PhSizes
//-----------------------------------------------------------------------------
class ADN_CLV_PhSizes : public ADN_Connector_ListView_ABC
                      , private boost::noncopyable
{

public:
    ADN_CLV_PhSizes( ADN_Weapons_PhSizeListView& list, GQ_Plot& plot, const std::map< void*, unsigned int >& userIds )
        : ADN_Connector_ListView_ABC( list )
        , plot_ ( plot )
        , userIds_( userIds )
    {
        // NOTHING
    }

    virtual ~ADN_CLV_PhSizes()
    {
        // NOTHING
    }

    ADN_StandardItem* CreateItem( void* pObj )
    {
        std::map< void*, unsigned int >::const_iterator it = userIds_.find( pObj );
        if( it == userIds_.end() )
        {
            assert( false );
            return 0;
        }
        // Retrieve the display icon for this item from the plot.
        GQ_PlotData* pData = plot_.FindPlotData( it->second );
        assert( pData != 0 );
        QPixmap icon;
        pData->DrawCaption( icon );

        // Create a new list item.
        ADN_StandardItem *pItem = new ADN_StandardItem( pObj );
        pItem->setIcon( icon );

        // Connect it with the size info.
        pItem->Connect( &static_cast< PhSizeInfos* >( pObj )->strName_ );

        return pItem;
    }

private:
    GQ_Plot& plot_;
    const std::map< void*, unsigned int >& userIds_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhSizeListView constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhSizeListView::ADN_Weapons_PhSizeListView( GQ_Plot& plot, const std::map< void*, unsigned int >& userIds )
    : ADN_ListView( 0, "ADNWeaponsPhSizeListView", tools::translate( "ADN_Weapons_PhSizeListView", "Target size" ) )
{
    setMaximumHeight( 300 );
    pConnector_.reset( new ADN_CLV_PhSizes( *this, plot, userIds ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_PhSizeListView destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Weapons_PhSizeListView::~ADN_Weapons_PhSizeListView()
{
    // NOTHING
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

