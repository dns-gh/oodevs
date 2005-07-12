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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentResourcesPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 11:38 $
// $Revision: 6 $
// $Workfile: MOS_AgentResourcesPanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentResourcesPanel_h_
#define __MOS_AgentResourcesPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Agent;
class QListView;

typedef std::vector< MT_ListViewItem* >   T_ItemPtrVector;
typedef T_ItemPtrVector::iterator       IT_ItemPtrVector;


// =============================================================================
/** @class  MOS_AgentResourcesPanel
    @brief  MOS_AgentResourcesPanel
    @par    Using example
    @code
    MOS_AgentResourcesPanel;
    @endcode
*/
// Created: APE 2004-03-10
// =============================================================================
class MOS_AgentResourcesPanel : public MOS_InfoPanel_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentResourcesPanel );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_AgentResourcesPanel( QWidget* pParent );
    ~MOS_AgentResourcesPanel();
    //@}

private:
    virtual void OnUpdate();
    void OnAgentUpdated( MOS_Agent& agent );

private:
    QListView* pEquipment_;
    QListView* pResources_;
    QListView* pTroops_;
    QListView* pLends_;

    T_ItemPtrVector equipement_;
    T_ItemPtrVector resources_;
    T_ItemPtrVector troops_;
    T_ItemPtrVector lends_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_AgentResourcesPanel.inl"
#endif

#endif // __MOS_AgentResourcesPanel_h_
