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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCreationListView.h $
// $Author: Ape $
// $Modtime: 10/12/04 16:19 $
// $Revision: 2 $
// $Workfile: MOS_AgentCreationListView.h $
//
// *****************************************************************************

#ifndef __MOS_AgentCreationListView_h_
#define __MOS_AgentCreationListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_AgentCreationPanel.h"


// =============================================================================
/** @class  MOS_AgentCreationListView
    @brief  MOS_AgentCreationListView
*/
// Created: APE 2004-09-02
// =============================================================================
class MOS_AgentCreationListView : public QListView
{
    MT_COPYNOTALLOWED( MOS_AgentCreationListView );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_AgentCreationListView( QWidget* pParent );
    ~MOS_AgentCreationListView();
    //@}

    //! @name Operations
    //@{
    void FillList( const MOS_AgentCreationPanel::T_AgentTypeVector& agentTypes );
    void ShowInvalid();
    //@}

private:
    //! @name Drag & drop
    //@{
    QDragObject* dragObject();
    //@}
};

#include "MOS_AgentCreationListView.inl"

#endif // __MOS_AgentCreationListView_h_
