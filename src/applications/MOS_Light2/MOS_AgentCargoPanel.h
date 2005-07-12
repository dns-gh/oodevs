// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCargoPanel.h $
// $Author: Age $
// $Modtime: 5/04/05 17:18 $
// $Revision: 5 $
// $Workfile: MOS_AgentCargoPanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentCargoPanel_h_
#define __MOS_AgentCargoPanel_h_

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
/** @class  MOS_AgentCargoPanel
    @brief  MOS_AgentCargoPanel
    @par    Using example
    @code
    MOS_AgentCargoPanel;
    @endcode
*/
// Created: APE 2004-04-29
// =============================================================================
class MOS_AgentCargoPanel : public MOS_InfoPanel_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentCargoPanel );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_AgentCargoPanel( QWidget* pParent );
    ~MOS_AgentCargoPanel();
    //@}

private:
    virtual void OnUpdate();
    void OnAgentUpdated( MOS_Agent& agent );

private:
    QListView* pTransports_;
    QListView* pEquipment_;
    QListView* pResources_;
    QListView* pTroops_;

    T_ItemPtrVector transports_;
    T_ItemPtrVector equipement_;
    T_ItemPtrVector resources_;
    T_ItemPtrVector troops_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_AgentCargoPanel.inl"
#endif

#endif // __MOS_AgentCargoPanel_h_
