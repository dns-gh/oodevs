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

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "ActionObserver_ABC.h"

class Agent;
class ReportListView;
class Display;
class Controller;
class ActionController;
class Attributes;
class Contaminations;
class HumanFactors;
class Reinforcements;
class LogisticLinks;
class Transports;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class AgentStatePanel : public QScrollView
                      , public Observer_ABC
                      , public ElementObserver_ABC< Attributes >
                      , public ElementObserver_ABC< Contaminations >
                      , public ElementObserver_ABC< HumanFactors >
                      , public ElementObserver_ABC< Reinforcements >
                      , public ElementObserver_ABC< LogisticLinks >
                      , public ElementObserver_ABC< Transports >
                      , public ActionObserver_ABC
{
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
             AgentStatePanel( QWidget* pParent, Controller& controller, ActionController& actionController );
    virtual ~AgentStatePanel();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentStatePanel( const AgentStatePanel& );
    AgentStatePanel& operator=( const AgentStatePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void insertChild ( QObject* pObj );
    virtual QLayout* layout();

    virtual void NotifySelected( const Agent& agent );
    virtual void NotifyUpdated( const Attributes& attributes );
    virtual void NotifyUpdated( const Contaminations& attributes );
    virtual void NotifyUpdated( const HumanFactors& attributes );
    virtual void NotifyUpdated( const Reinforcements& attributes );
    virtual void NotifyUpdated( const LogisticLinks& attributes );
    virtual void NotifyUpdated( const Transports& attributes );

    template< typename Extension >
    bool ShouldUpdate( const Extension& extension );
    //@}

private:
    //! @name Member data
    //@{
    ActionController& actionController_;
    QVBox*   pBox_;
    Display* display_;
    const Agent* selected_;
    //@}
};

#endif // __AgentStatePanel_h_
