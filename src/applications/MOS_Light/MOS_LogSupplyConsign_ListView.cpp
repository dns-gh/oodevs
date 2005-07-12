//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogSupplyConsign_ListView.cpp $
// $Author: Nld $
// $Modtime: 28/01/05 11:01 $
// $Revision: 1 $
// $Workfile: MOS_LogSupplyConsign_ListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_LogSupplyConsign_ListView.h"

#include "MOS_LogSupplyConsign_ListView_Item.h"
#include "MOS_LogSupplyConsign.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView::MOS_LogSupplyConsign_ListView
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign_ListView::MOS_LogSupplyConsign_ListView( QWidget* pParent, bool bHandledConsigns )
    : QListView        ( pParent, "Consignes rav" )
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
// Name: MOS_LogSupplyConsign_ListView destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign_ListView::~MOS_LogSupplyConsign_ListView()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView::AddConsgin
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogSupplyConsign_ListView::AddConsign( MOS_LogSupplyConsign& consign )
{
    new MOS_LogSupplyConsign_ListView_Item( this, consign );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView::RemoveConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogSupplyConsign_ListView::RemoveConsign( MOS_LogSupplyConsign& consign )
{
    consign.DeleteAllItems();
}

