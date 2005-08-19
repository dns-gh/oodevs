//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ArmeListViewItem.cpp $
// $Author: Nld $
// $Modtime: 29/12/04 15:46 $
// $Revision: 7 $
// $Workfile: MOS_ArmeListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_ArmeListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ArmeListViewItem.inl"
#endif

#include "MOS_Agent.h"
#include "MOS_AutomateListViewItem.h"
#include "MOS_AutomateCheckListItem.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_AgentListView.h"
#include "MOS_KnowledgeGroupListViewItem.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"

//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ArmeListViewItem::MOS_ArmeListViewItem( QListView& listView, QListViewItem& itemAfter, const std::string& strField )
    : MOS_ListViewItem_ABC( eItemTypeArme, listView, itemAfter, strField )
{
    setRenameEnabled( 0, false );
    setOpen( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ArmeListViewItem::MOS_ArmeListViewItem( QListViewItem& item, const std::string& strField )
    : MOS_ListViewItem_ABC( eItemTypeArme, item, strField )
{
    setRenameEnabled( 0, false );
    setOpen( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ArmeListViewItem::MOS_ArmeListViewItem( QListView& listView, const std::string& strField )
    : MOS_ListViewItem_ABC( eItemTypeArme, listView, strField )
{
    setRenameEnabled( 0, false );
    setOpen( true );
}


//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_ArmeListViewItem::~MOS_ArmeListViewItem()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::AddAutomate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem& MOS_ArmeListViewItem::CreateAutomate( MOS_Agent& agent, MOS_AgentListView* pListview, bool bCanCheck )
{
    //
    MOS_Gtia& gtia = agent.GetGtia();
    MOS_KnowledgeGroupListViewItem*& pKnowledgeGroupItem = knowledgeGroups_[ &gtia ];
    if( !pKnowledgeGroupItem )
    {
        std::stringstream strTmp;
        strTmp << "Army " << agent.GetTeam().GetName() << " - KG " << gtia.GetID();
        pKnowledgeGroupItem = new MOS_KnowledgeGroupListViewItem( gtia, *this, strTmp.str() );
    }

    QListViewItem* pAutomateArme = 0;
    if ( pListview->GetType() == eListViewSelector )
    {
        QCheckListItem::Type nCheckable = QCheckListItem::Controller;
        if ( bCanCheck )
            nCheckable = QCheckListItem::CheckBox;

        if ( automateVector_.size() )
            pAutomateArme = new MOS_AutomateCheckListItem( agent, *pKnowledgeGroupItem, **(automateVector_.rbegin()), nCheckable );
        else
            pAutomateArme = new MOS_AutomateCheckListItem( agent, *pKnowledgeGroupItem, nCheckable );
    }
    else
    {
        if ( automateVector_.size() )
            pAutomateArme = new MOS_AutomateListViewItem( agent, *pKnowledgeGroupItem, **(automateVector_.rbegin()) );
        else
            pAutomateArme = new MOS_AutomateListViewItem( agent, *pKnowledgeGroupItem );
    }


    RegisterAutomate( *pAutomateArme, pListview );
    return ( *pAutomateArme );
}


//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::DeleteAutomate
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_ArmeListViewItem::DeleteAutomate( MOS_Agent& agent )
{
    QListViewItem* pAutomate = GetAutomate( agent );
    assert( pAutomate );
    UnregisterAutomate( *pAutomate );
    delete pAutomate;
}
//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::GetAutomateBefore
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
QListViewItem* MOS_ArmeListViewItem::GetAutomateBefore( const QListViewItem& automate ) const
{
    CIT_AutomatePtrVector itAutomate = std::find( automateVector_.begin(), automateVector_.end(), &automate );
    assert( itAutomate != automateVector_.end() );
    if ( *itAutomate == *automateVector_.begin() )
    {
        return 0;
    }
    --itAutomate;
    return *itAutomate;
}
//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_ArmeListViewItem::SetName( const std::string sName )
{
    setText(0, sName.c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::RegisterAutomate
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------

void MOS_ArmeListViewItem::RegisterAutomate( QListViewItem& automate, MOS_AgentListView* pListview, QListViewItem* pAutomateAfter )
{
    if ( pListview->GetType() == eListViewSelector )
    {
        if ( pAutomateAfter )
        {
            MOS_AgentCheckListItem_ABC* pAgentAfter = (MOS_AgentCheckListItem_ABC*)pAutomateAfter;            
            for ( IT_AutomatePtrVector itAutomate = automateVector_.begin() ; itAutomate != automateVector_.end() ; ++itAutomate )
            {
                MOS_AgentCheckListItem_ABC* pCurAgent = (MOS_AgentCheckListItem_ABC*)(*itAutomate);
                if ( &pCurAgent->GetAgent() == &pAgentAfter->GetAgent() )
                    break;
            }
            assert( itAutomate != automateVector_.end() );
            if ( *itAutomate != *automateVector_.rbegin() )
            {
                ++itAutomate;
                automateVector_.insert( itAutomate, &automate );
            }
            else
                automateVector_.push_back( &automate );
        }
        else
            automateVector_.push_back( &automate );
    }
    else
    {
        if ( pAutomateAfter )
        {
            MOS_AgentListViewItem_ABC* pAgentAfter = (MOS_AgentListViewItem_ABC*)pAutomateAfter;            
            for ( IT_AutomatePtrVector itAutomate = automateVector_.begin() ; itAutomate != automateVector_.end() ; ++itAutomate )
            {
                MOS_AgentListViewItem_ABC* pCurAgent = (MOS_AgentListViewItem_ABC*)(*itAutomate);
                if ( &pCurAgent->GetAgent() == &pAgentAfter->GetAgent() )
                    break;
            }
            assert( itAutomate != automateVector_.end() );
            if ( *itAutomate != *automateVector_.rbegin() )
            {
                ++itAutomate;
                automateVector_.insert( itAutomate, &automate );
            }
            else
                automateVector_.push_back( &automate );
        }
        else
            automateVector_.push_back( &automate );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ArmeListViewItem::GetAutomate
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------

QListViewItem* MOS_ArmeListViewItem::GetAutomate( const MOS_Agent& agent )
{
    MOS_AgentListView* pListview = (MOS_AgentListView*)parent();
    if ( pListview->GetType() == eListViewSelector )
    {
        for ( IT_AutomatePtrVector itAutomate = automateVector_.begin() ; itAutomate != automateVector_.end() ; ++itAutomate )
        {
            MOS_AgentCheckListItem_ABC* pCurAgent = (MOS_AgentCheckListItem_ABC*)(*itAutomate);
            if ( &pCurAgent->GetAgent() == &agent  )
                break;
        }
        if ( itAutomate != automateVector_.end() )
            return (*itAutomate);
    }
    else
    {
        for ( IT_AutomatePtrVector itAutomate = automateVector_.begin() ; itAutomate != automateVector_.end() ; ++itAutomate )
        {
            MOS_AgentListViewItem_ABC* pCurAgent = (MOS_AgentListViewItem_ABC*)(*itAutomate);
            if ( &pCurAgent->GetAgent() == &agent  )
                break;
        }
        if ( itAutomate != automateVector_.end() )
            return (*itAutomate);

    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: std::MOS_ArmeListViewItem::GetName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
const std::string MOS_ArmeListViewItem::GetName() const
{
    return text(0);
}