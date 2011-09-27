// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Categories_LogisticSupplyClass.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Categories_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_GuiTools.h"
#include "ADN_LogisticSupplyClass.h"
#include "ADN_Tr.h"

//-----------------------------------------------------------------------------
// Internal List View Categories_LogisticSupplyClass connector to be connected with ADN_ListView_Categories_LogisticSupplyClass
//-----------------------------------------------------------------------------
class ADN_CLV_Categories_LogisticSupplyClass
: public ADN_Connector_ListView_ABC
{
public:

    ADN_CLV_Categories_LogisticSupplyClass( ADN_ListView_Categories_LogisticSupplyClass& list )
        : ADN_Connector_ListView_ABC( list ) {}

    virtual ~ADN_CLV_Categories_LogisticSupplyClass() {}

    ADN_ListViewItem* CreateItem( void * obj )
    {
        ADN_ListViewItem *pItem = new ADN_ListViewItem( &list_, obj, 1 );
        pItem->Connect( 0, static_cast< helpers::LogisticSupplyClass* >( obj ) );
        return pItem;
    }

private:
    ADN_CLV_Categories_LogisticSupplyClass& operator=( const ADN_CLV_Categories_LogisticSupplyClass& );
};


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::ADN_ListView_Categories_LogisticSupplyClass
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_LogisticSupplyClass::ADN_ListView_Categories_LogisticSupplyClass( QWidget * parent, const char * name, Qt::WFlags f )
    : ADN_ListView( parent, name, f )
{
    addColumn( tools::translate( "ADN_ListView_Categories_LogisticSupplyClass", "Logistic categories" ) );
    setSorting( -1, true );
    setResizeMode( Q3ListView::AllColumns );
    pConnector_ = new ADN_CLV_Categories_LogisticSupplyClass( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::~ADN_ListView_Categories_LogisticSupplyClass
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_LogisticSupplyClass::~ADN_ListView_Categories_LogisticSupplyClass()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::ConnectItem
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_LogisticSupplyClass::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    helpers::LogisticSupplyClass* pInfos = (helpers::LogisticSupplyClass*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Categories_GUI::eNbrLogisticSupplyClassGuiElements );

    vItemConnectors_[ ADN_Categories_GUI::eLogisticSupplyClassName ]->Connect( pInfos, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::OnContextMenu
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_LogisticSupplyClass::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popuMenu( this );

    popuMenu.insertItem( tools::translate( "ADN_ListView_Categories_LogisticSupplyClass", "New category"), 0 );
    popuMenu.insertItem( tools::translate( "ADN_ListView_Categories_LogisticSupplyClass", "Delete category"), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );

    int nResult = popuMenu.exec( pt );
    switch ( nResult )
    {
        case 0:
        {
            helpers::LogisticSupplyClass* pNewInfo = new helpers::LogisticSupplyClass( "new category", ADN_Categories_Data::GetNewIdentifier() );
            pNewInfo->SetDataName( "new category" );

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
            helpers::LogisticSupplyClass* pCurSize = ( helpers::LogisticSupplyClass* )pCurData_;
            if( pCurSize )
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
