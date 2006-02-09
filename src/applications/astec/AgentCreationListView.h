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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentCreationListView.h $
// $Author: Ape $
// $Modtime: 10/12/04 16:19 $
// $Revision: 2 $
// $Workfile: AgentCreationListView.h $
//
// *****************************************************************************

#ifndef __AgentCreationListView_h_
#define __AgentCreationListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "AgentCreationPanel.h"


// =============================================================================
/** @class  AgentCreationListView
    @brief  AgentCreationListView
*/
// Created: APE 2004-09-02
// =============================================================================
class AgentCreationListView : public QListView
{
    MT_COPYNOTALLOWED( AgentCreationListView );

public:
    //! @name Constructors/Destructor
    //@{
     AgentCreationListView( QWidget* pParent );
    ~AgentCreationListView();
    //@}

    //! @name Operations
    //@{
    void FillList( const AgentCreationPanel::T_AgentTypeVector& agentTypes );
    void ShowInvalid();
    //@}

private:
    //! @name Drag & drop
    //@{
    QDragObject* dragObject();
    //@}
};

#endif // __AgentCreationListView_h_
