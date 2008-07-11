// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Categories_DotationNature.h"

#include <qpopupmenu.h>

#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Categories_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_GuiTools.h"

typedef ADN_Categories_Data::DotationNatureInfos DotationNatureInfos;


//-----------------------------------------------------------------------------
// Internal List View Categories_DotationNature connector to be connected with ADN_ListView_Categories_DotationNature
//-----------------------------------------------------------------------------
class ADN_CLV_Categories_DotationNature
: public ADN_Connector_ListView_ABC
{
public:
    
    ADN_CLV_Categories_DotationNature( ADN_ListView_Categories_DotationNature& list ) 
        : ADN_Connector_ListView_ABC( list ) {}

    virtual ~ADN_CLV_Categories_DotationNature() {}

    ADN_ListViewItem* CreateItem( void * obj )
    {
        ADN_ListViewItem *pItem = new ADN_ListViewItem( &list_, obj, 1 );
        pItem->Connect( 0, static_cast< DotationNatureInfos* >( obj ) );
        return pItem;
    }
    
private:
    ADN_CLV_Categories_DotationNature& operator=( const ADN_CLV_Categories_DotationNature& );
};


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature( QWidget * parent, const char * name, WFlags f )
    : ADN_ListView( parent, name, f )
{
    addColumn( tr( "Resource Natures" ) );
    setSorting( -1, true );
    setResizeMode( QListView::AllColumns );
    pConnector_ = new ADN_CLV_Categories_DotationNature( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::~ADN_ListView_Categories_DotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_DotationNature::~ADN_ListView_Categories_DotationNature()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::ConnectItem
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_DotationNature::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    DotationNatureInfos* pInfos = (DotationNatureInfos*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Categories_GUI::eNbrDotationNatureGuiElements );

    vItemConnectors_[ ADN_Categories_GUI::eDotationNatureName ]->Connect( pInfos, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::OnContextMenu
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_DotationNature::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popuMenu( this );

    popuMenu.insertItem( tr( "New nature"), 0 );
    popuMenu.insertItem( tr( "Delete nature"), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );

    int nResult = popuMenu.exec( pt );
    switch ( nResult )
    {
        case 0:
        {
            DotationNatureInfos* pNewInfo = new DotationNatureInfos();
            pNewInfo->SetDataName( "le nom de la nature de dotation" );

            ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
            pCList->AddItem( pNewInfo );

            // Put the  new item at the top of the list (to be coherent with the application)
            int pos = FindNdx( pNewInfo );
            while( pos != 0 )
            {
                pCList->SwapItem( pos - 1, pos );
                --pos;
            }
            
            // set current item
            setCurrentItem( FindItem( pNewInfo ) );
            break;
        }
        case 1:
        {
            DotationNatureInfos* pCurSize = (DotationNatureInfos*)pCurData_;
            if ( pCurSize )
            {
                if( pCurSize->IsMultiRef() && ! ADN_GuiTools::MultiRefWarning() )
                    return;

                static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurSize );
            }
            break;
        }
        default:
            break;
    }
}
