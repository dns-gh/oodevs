// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentListView.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:42 $
// $Revision: 9 $
// $Workfile: AgentListView.h $
//
// *****************************************************************************

#ifndef __AgentListView_h_
#define __AgentListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ElementObserver_ABC.h"
#include "Observer_ABC.h"

class Team;
class Gtia;
class Agent;
class ActionContext;
class SelectedElement;
class Controller;


// =============================================================================
/** @class  AgentListView
    @brief  Displays the agent list sorted by team, gtia and group.
*/
// Created: APE 2004-03-15
// =============================================================================
class AgentListView : public QListView
                    , public Observer_ABC
                    , public ElementObserver_ABC< Team >
{
//    Q_OBJECT;
    MT_COPYNOTALLOWED( AgentListView );

private:
    enum
    {
        eTeam  = 1000,
        eGtia  = 1001,
        eAgent = 1002
    };
    
public:
    //! @name Constructors/Destructor
    //@{
     AgentListView( QWidget* pParent, Controller& controller );
    ~AgentListView();

    QSize sizeHint() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Team& team );
    virtual void NotifyUpdated( const Team& team );
    virtual void NotifyDeleted( const Team& team );
    //@}

private:
    QPopupMenu* pPopupMenu_;
};

#endif // __AgentListView_h_
