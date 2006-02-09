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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentCreationListView.cpp $
// $Author: Ape $
// $Modtime: 10/12/04 16:19 $
// $Revision: 4 $
// $Workfile: AgentCreationListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentCreationListView.h"


// -----------------------------------------------------------------------------
// Name: AgentCreationListView constructor
/** @param  pParent 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
AgentCreationListView::AgentCreationListView( QWidget* pParent )
    : QListView( pParent )
{
    this->addColumn( tr( "Unités disponibles" ) );
    this->setResizeMode( QListView::LastColumn );
}


// -----------------------------------------------------------------------------
// Name: AgentCreationListView destructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
AgentCreationListView::~AgentCreationListView()
{
}


// -----------------------------------------------------------------------------
// Name: AgentCreationListView::FillList
/** @param  agentTypes 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
void AgentCreationListView::FillList( const AgentCreationPanel::T_AgentTypeVector& agentTypes )
{
    this->clear();
    for( AgentCreationPanel::CIT_AgentTypeVector it = agentTypes.begin(); it != agentTypes.end(); ++it )
    {
        QListViewItem* pItem = new MT_ValuedListViewItem< AgentCreationPanel::AgentType* >( *it, this, (*it)->strName_.c_str() );
        pItem->setDragEnabled( true );
    }
}


// -----------------------------------------------------------------------------
// Name: AgentCreationListView::ShowInvalid
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
void AgentCreationListView::ShowInvalid()
{
    this->clear();
    QListViewItem* pItem = new QListViewItem( this, tr( "Modèle d'agent non valide" ) );
    pItem->setDragEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: AgentCreationListView::dragObject
/** @return 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
QDragObject* AgentCreationListView::dragObject()
{
    QListViewItem* pItem = selectedItem();
    assert( pItem != 0 );

    AgentCreationPanel::AgentType* pAgentType = ((MT_ValuedListViewItem< AgentCreationPanel::AgentType* >*)pItem)->GetValue();

    QByteArray* pBytes = new QByteArray();
    pBytes->setRawData( (const char*)&pAgentType, sizeof( AgentCreationPanel::AgentType* ) );

    QStoredDrag* data = new QStoredDrag( "pointer/agentType", this );
    data->setEncodedData( *pBytes );

    return data;
}
