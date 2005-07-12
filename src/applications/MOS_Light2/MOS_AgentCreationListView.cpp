// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCreationListView.cpp $
// $Author: Ape $
// $Modtime: 10/12/04 16:19 $
// $Revision: 4 $
// $Workfile: MOS_AgentCreationListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_AgentCreationListView.h"


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationListView constructor
/** @param  pParent 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_AgentCreationListView::MOS_AgentCreationListView( QWidget* pParent )
    : QListView( pParent )
{
    this->addColumn( tr( "Unités disponibles" ) );
    this->setResizeMode( QListView::LastColumn );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationListView destructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_AgentCreationListView::~MOS_AgentCreationListView()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationListView::FillList
/** @param  agentTypes 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
void MOS_AgentCreationListView::FillList( const MOS_AgentCreationPanel::T_AgentTypeVector& agentTypes )
{
    this->clear();
    for( MOS_AgentCreationPanel::CIT_AgentTypeVector it = agentTypes.begin(); it != agentTypes.end(); ++it )
    {
        QListViewItem* pItem = new MT_ValuedListViewItem< MOS_AgentCreationPanel::MOS_AgentType* >( *it, this, (*it)->strName_.c_str() );
        pItem->setDragEnabled( true );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationListView::ShowInvalid
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
void MOS_AgentCreationListView::ShowInvalid()
{
    this->clear();
    QListViewItem* pItem = new QListViewItem( this, tr( "Modèle d'agent non valide" ) );
    pItem->setDragEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationListView::dragObject
/** @return 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
QDragObject* MOS_AgentCreationListView::dragObject()
{
    QListViewItem* pItem = selectedItem();
    assert( pItem != 0 );

    MOS_AgentCreationPanel::MOS_AgentType* pAgentType = ((MT_ValuedListViewItem< MOS_AgentCreationPanel::MOS_AgentType* >*)pItem)->GetValue();

    QByteArray* pBytes = new QByteArray();
    pBytes->setRawData( (const char*)&pAgentType, sizeof( MOS_AgentCreationPanel::MOS_AgentType* ) );

    QStoredDrag* data = new QStoredDrag( "pointer/agentType", this );
    data->setEncodedData( *pBytes );

    return data;
}
