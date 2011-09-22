// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_RoofShapes.h"
#include "ADN_Tr.h"
#include "ADN_Urban_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Urban_GUI.h"
#include "ADN_GuiTools.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_RoofShapes constructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_ListView_RoofShapes::ADN_ListView_RoofShapes( QWidget* pParent, const char* szName, Qt::WFlags f )
    :  ADN_ListView( pParent, szName, f )
{
    addColumn( tools::translate( "ADN_ListView_RoofShapes", szName ) );
    setSorting( -1, true );
    setResizeMode( Q3ListView::AllColumns );
    pConnector_ = new ADN_Connector_ListView< ADN_Urban_Data::RoofShapeInfos >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_RoofShapes destructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_ListView_RoofShapes::~ADN_ListView_RoofShapes()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_RoofShapes::ConnectItem
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_ListView_RoofShapes::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Urban_Data::RoofShapeInfos* pInfos = ( ADN_Urban_Data::RoofShapeInfos* )pCurData_;

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanGuiElements );

    vItemConnectors_[ ADN_Urban_GUI::eUrbanName ]->Connect( &pInfos->strName_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_RoofShapes::OnContextMenu
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_ListView_RoofShapes::OnContextMenu( const QPoint& point )
{
    Q3PopupMenu popuMenu( this );
    popuMenu.insertItem( tools::translate( "ADN_ListView_Urban_Type", "New" ), 0 );
    popuMenu.insertItem( tools::translate( "ADN_ListView_Urban_Type", "Delete" ), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );
    int nResult = popuMenu.exec( point );
    switch ( nResult )
    {
    case 0:
        {
            ADN_Urban_Data::RoofShapeInfos* pNewInfo = new ADN_Urban_Data::RoofShapeInfos();
            ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
            pCList->AddItem( pNewInfo );
            int pos = FindNdx( pNewInfo );
            while( pos != 0 )
            {
                pCList->SwapItem( pos - 1, pos );
                --pos;
            }
            setCurrentItem( FindItem( pNewInfo ) );
            break;
        }
    case 1:
        {
            ADN_Urban_Data::RoofShapeInfos* pCurSize=(ADN_Urban_Data::RoofShapeInfos*)pCurData_;
            if( pCurSize )
            {
                if( pCurSize->IsMultiRef() && ! ADN_GuiTools::MultiRefWarning() )
                    return;
                static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurSize);
            }
            break;
        }
    default:
        break;
    }
}
