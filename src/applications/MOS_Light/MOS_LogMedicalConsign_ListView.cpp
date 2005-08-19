//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMedicalConsign_ListView.cpp $
// $Author: Nld $
// $Modtime: 11/01/05 19:44 $
// $Revision: 1 $
// $Workfile: MOS_LogMedicalConsign_ListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_LogMedicalConsign_ListView.h"

#include "MOS_LogMedicalConsign_ListView_Item.h"
#include "MOS_LogMedicalConsign.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView::MOS_LogMedicalConsign_ListView
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign_ListView::MOS_LogMedicalConsign_ListView( QWidget* pParent, bool bHandledConsigns )
    : QListView        ( pParent, "Consignes Santé" )
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
// Name: MOS_LogMedicalConsign_ListView destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign_ListView::~MOS_LogMedicalConsign_ListView()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView::AddConsgin
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogMedicalConsign_ListView::AddConsign( MOS_LogMedicalConsign& consign )
{
    new MOS_LogMedicalConsign_ListView_Item( this, consign );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView::RemoveConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_LogMedicalConsign_ListView::RemoveConsign( MOS_LogMedicalConsign& consign )
{
    consign.DeleteAllItems();
}

