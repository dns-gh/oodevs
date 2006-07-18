// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentResourcesPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 11:38 $
// $Revision: 6 $
// $Workfile: AgentResourcesPanel.h $
//
// *****************************************************************************

#ifndef __AgentResourcesPanel_h_
#define __AgentResourcesPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "InfoPanel_ABC.h"

class Agent;
class QListView;

typedef std::vector< MT_ListViewItem* >   T_ItemPtrVector;
typedef T_ItemPtrVector::iterator       IT_ItemPtrVector;


// =============================================================================
/** @class  AgentResourcesPanel
    @brief  AgentResourcesPanel
    @par    Using example
    @code
    AgentResourcesPanel;
    @endcode
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentResourcesPanel : public InfoPanel_ABC
{
    MT_COPYNOTALLOWED( AgentResourcesPanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentResourcesPanel( QWidget* pParent );
    virtual ~AgentResourcesPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
    virtual void OnAgentUpdated  ( Agent& agent );
    //@}
    
private:
    //! @name Member Data
    //@{
    QListView* pEquipment_;
    QListView* pResources_;
    QListView* pTroops_;
    QListView* pLendings_;
    QListView* pBorrowings_;

    T_ItemPtrVector equipement_;
    T_ItemPtrVector resources_;
    T_ItemPtrVector troops_;
    T_ItemPtrVector lendings_;
    T_ItemPtrVector borrowings_;
    //@}
};

#endif // __AgentResourcesPanel_h_
