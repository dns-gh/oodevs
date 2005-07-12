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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ArmeListViewItem.h $
// $Author: Nld $
// $Modtime: 25/10/04 15:55 $
// $Revision: 4 $
// $Workfile: MOS_ArmeListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_ArmeListViewItem_h_
#define __MOS_ArmeListViewItem_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_AutomateListViewItem.h"

class QListView;
class QListViewItem;
class MOS_Agent;
class MOS_AgentListView;
class MOS_Gtia;
class MOS_KnowledgeGroupListViewItem;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ArmeListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_ArmeListViewItem );
private:
    typedef std::vector< QListViewItem* >          T_AutomatePtrVector;
    typedef const T_AutomatePtrVector              CT_AutomatePtrVector;
    typedef T_AutomatePtrVector::iterator          IT_AutomatePtrVector;
    typedef T_AutomatePtrVector::const_iterator    CIT_AutomatePtrVector;
    typedef T_AutomatePtrVector::reverse_iterator  RIT_AutomatePtrVector;

public:
    MOS_ArmeListViewItem( QListView& listView,                           const std::string& strField );
    MOS_ArmeListViewItem( QListView& listView, QListViewItem& itemAfter, const std::string& strField );
    MOS_ArmeListViewItem( QListViewItem& item,                           const std::string& strField );
   
    
    virtual ~MOS_ArmeListViewItem();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    QListViewItem*    GetFirstAutomate() const;
    const std::string GetName() const;
    void              SetName( const std::string sName );
    QListViewItem*    GetAutomateBefore( const QListViewItem& Automate ) const;
    QListViewItem*    GetAutomate( const MOS_Agent& agent );
    //@}

    //-------------------------------------------------------------------------
    /** @name Automates*/
    //-------------------------------------------------------------------------
    //@{
    void           RegisterAutomate  ( QListViewItem& automate, MOS_AgentListView* pListview, QListViewItem* pAutomateAfter = 0 );
    void           UnregisterAutomate( const QListViewItem& automate );
    QListViewItem& CreateAutomate    ( MOS_Agent& agent, MOS_AgentListView* pListview, bool bCanCheck = true );
    void           DeleteAutomate    ( MOS_Agent& agent );
    //@}

private:
    typedef std::map< const MOS_Gtia*, MOS_KnowledgeGroupListViewItem* > T_KnowledgeGroupListViewItemMap;
    typedef T_KnowledgeGroupListViewItemMap::const_iterator              CIT_KnowledgeGroupListViewItemMap;

private:
    T_AutomatePtrVector             automateVector_;
    T_KnowledgeGroupListViewItemMap knowledgeGroups_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ArmeListViewItem.inl"
#endif

#endif // __MOS_ArmeListViewItem_h_
