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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentStatePanel.h $
// $Author: Nld $
// $Modtime: 8/07/05 15:25 $
// $Revision: 14 $
// $Workfile: MOS_AgentStatePanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentStatePanel_h_
#define __MOS_AgentStatePanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Attr_Def.h"
#include "MOS_InfoPanel_ABC.h"

class MOS_Agent;
class MOS_ReportListView;
class MOS_Display;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class MOS_AgentStatePanel : public MOS_InfoPanel_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentStatePanel );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_AgentStatePanel( QWidget* pParent );
    virtual ~MOS_AgentStatePanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
    virtual void OnAgentUpdated  ( MOS_Agent& agent );
    //@}

private:
    //! @name Member data
    //@{
    MOS_Display* display_;
    //@}
};

#endif // __MOS_AgentStatePanel_h_
