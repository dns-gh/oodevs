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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentStatePanel.h $
// $Author: Nld $
// $Modtime: 8/07/05 15:25 $
// $Revision: 14 $
// $Workfile: AgentStatePanel.h $
//
// *****************************************************************************

#ifndef __AgentStatePanel_h_
#define __AgentStatePanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Attr_Def.h"
#include "InfoPanel_ABC.h"

class Agent;
class ReportListView;
class Display;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class AgentStatePanel : public InfoPanel_ABC
{
    MT_COPYNOTALLOWED( AgentStatePanel );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentStatePanel( QWidget* pParent );
    virtual ~AgentStatePanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
    virtual void OnAgentUpdated  ( Agent& agent );
    //@}

private:
    //! @name Member data
    //@{
    Display* display_;
    //@}
};

#endif // __AgentStatePanel_h_
