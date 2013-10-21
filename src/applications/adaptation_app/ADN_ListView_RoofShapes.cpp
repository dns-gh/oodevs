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
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_RoofShapes constructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_ListView_RoofShapes::ADN_ListView_RoofShapes( QWidget* pParent )
    :  ADN_ListView( pParent, "RoofShape", tools::translate( "ADN_ListView_RoofShapes", "RoofShape" ) )
{
    setMinimumHeight( 150 );
    pConnector_.reset( new ADN_Connector_ListView< ADN_Urban_Data::RoofShapeInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_RoofShapes destructor
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_ListView_RoofShapes::~ADN_ListView_RoofShapes()
{
    // NOTHING
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
void ADN_ListView_RoofShapes::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Urban_Data::RoofShapeInfos > wizard( tools::translate( "ADN_ListView_RoofShapes", "RoofShapes" ), ADN_Workspace::GetWorkspace().GetUrban().GetData().GetRoofShapesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_RoofShapes::GetToolTipFor
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
std::string ADN_ListView_RoofShapes::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ADN_Urban_Data::RoofShapeInfos* pCastData = static_cast< ADN_Urban_Data::RoofShapeInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( tools::translate( "ADN_ListView_RoofShapes", "Templates" ),
                            ADN_Workspace::GetWorkspace().GetUrban().GetData().GetUrbanTemplateThatUse( *pCastData ) );
}
