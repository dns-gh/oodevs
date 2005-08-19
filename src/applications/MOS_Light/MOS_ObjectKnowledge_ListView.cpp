//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ObjectKnowledge_ListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_ObjectKnowledge_ListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_ObjectKnowledge_ListView.h"
#include "moc_MOS_ObjectKnowledge_ListView.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_ObjectKnowledge_ListView.inl"
#endif

#include <qheader.h>

#include "MOS_ObjectKnowledge.h"
#include "MOS_ObjectKnowledge_ListView_Item.h"

#include "MOS_Team.h"

//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView constructor
// Created: AGN 03-07-21
//-----------------------------------------------------------------------------
MOS_ObjectKnowledge_ListView::MOS_ObjectKnowledge_ListView( QWidget* pParent )
: QListView( pParent, "Objets connus" )
, pTeam_( 0 )
{
    addColumn( "Objets connus" );
    setColumnWidthMode( 0, QListView::Maximum );
    setMargin( 5 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );    
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView destructor
// Created: AGN 03-07-21
//-----------------------------------------------------------------------------
MOS_ObjectKnowledge_ListView::~MOS_ObjectKnowledge_ListView()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView::AddKnowledge
// Created: AGN 03-07-21
//-----------------------------------------------------------------------------
void MOS_ObjectKnowledge_ListView::AddKnowledge( MOS_ObjectKnowledge& object )
{
    new MOS_ObjectKnowledge_ListView_Item( this, object );
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView::RemoveKnowledge
// Created: AGN 03-07-21
//-----------------------------------------------------------------------------
void MOS_ObjectKnowledge_ListView::RemoveKnowledge( MOS_ObjectKnowledge& object )
{
    if( object.GetListViewItem() != 0 )
        delete( object.GetListViewItem() );
}



//-----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge_ListView::SetTeam
// Created: AGN 03-05-27
//-----------------------------------------------------------------------------
void MOS_ObjectKnowledge_ListView::SetTeam( MOS_Team* pTeam )
{
    if( pTeam_ )
        pTeam_->SetObjectKnowledgeListView( 0 );

    pTeam_ = pTeam;

    clear();

    if( pTeam_ )
        pTeam_->SetObjectKnowledgeListView( this );
}
