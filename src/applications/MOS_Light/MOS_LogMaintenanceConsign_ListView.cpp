//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMaintenanceConsign_ListView.cpp $
// $Author: Nld $
// $Modtime: 6/01/05 14:35 $
// $Revision: 1 $
// $Workfile: MOS_LogMaintenanceConsign_ListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_LogMaintenanceConsign_ListView.h"

#include "MOS_LogMaintenanceConsign_ListView_Item.h"
#include "MOS_LogMaintenanceConsign.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView::MOS_LogMaintenanceConsign_ListView
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign_ListView::MOS_LogMaintenanceConsign_ListView( QWidget* pParent, bool bHandledConsigns )
    : QListView        ( pParent, "Consignes maintenance" )
    , bHandledConsigns_( bHandledConsigns )
{
    if( bHandledConsigns )
        addColumn( "Consignes en traitement" );
    else
        addColumn( "Demandes logistiques" );
    setColumnWidthMode( 0, QListView::Maximum );
    setMargin( 5 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );    
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign_ListView::~MOS_LogMaintenanceConsign_ListView()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView::AddConsgin
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogMaintenanceConsign_ListView::AddConsign( MOS_LogMaintenanceConsign& consign )
{
    new MOS_LogMaintenanceConsign_ListView_Item( this, consign );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView::RemoveConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogMaintenanceConsign_ListView::RemoveConsign( MOS_LogMaintenanceConsign& consign )
{
    consign.DeleteAllItems();
}

