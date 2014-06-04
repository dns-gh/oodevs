// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __TacticalTreeView_h_
#define __TacticalTreeView_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/TacticalTreeView.h"
#include "ENT/ENT_Enums.h"

namespace gui
{
    class ChangeSuperiorDialog;
}

namespace kernel
{
    class AgentTypes;
}

class Model;

// =============================================================================
/** @class  TacticalTreeView
    @brief  TacticalTreeView
*/
// Created: JSR 2012-09-07
// =============================================================================
class TacticalTreeView : public gui::TacticalTreeView
                       , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TacticalTreeView( const QString& objectName,
                               kernel::Controllers& controllers,
                               const kernel::Profile_ABC& profile,
                               gui::ModelObserver_ABC& modelObserver,
                               const gui::EntitySymbols& symbols,
                               Model& model,
                               const kernel::AgentTypes& agentTypes,
                               gui::ChangeSuperiorDialog& changeSuperiorDialog,
                               QWidget* parent = 0 );
    virtual ~TacticalTreeView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void ContextMenuRequested( const QPoint& where );
    virtual void Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target);
    virtual void Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::AgentType& item, kernel::Entity_ABC& target );
    virtual void Drop( const kernel::AutomatType& item, kernel::Entity_ABC& target );

    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& agent, kernel::ContextMenu& menu );

    virtual void keyPressEvent( QKeyEvent* event );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateTeam();
    void OnCreateFormation( int level );
    void OnChangeLevel( int levelId );
    void Engage();
    void Disengage();
    void ChangeAutomatType();
    //@}

private:
    //! @name Helpers
    //@{
    void AddFormationMenu( kernel::ContextMenu& menu, E_NatureLevel root );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    const kernel::AgentTypes& agentTypes_;
    //@}
};

#endif // __TacticalTreeView_h_
