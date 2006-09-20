// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentListView_h_
#define __AgentListView_h_

#include "clients_gui/AgentListView.h"

namespace kernel
{
    class ModelLoaded;
}

class AutomatDecisions;
class Model;
class ModelBuilder;

// =============================================================================
/** @class  AgentListView
    @brief  AgentListView
*/
// Created: SBO 2006-08-29
// =============================================================================
class AgentListView : public gui::AgentListView
                    , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                    , public kernel::ElementObserver_ABC< AutomatDecisions >
                    , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AgentListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const Model& model, ModelBuilder& modelBuilder );
    virtual ~AgentListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Hierarchies& hierarchy, gui::ValuedListItem* item );
    virtual void Display( const kernel::Entity_ABC& agent, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRename( QListViewItem* item, int col, const QString& text );
    void Engage();
    void Disengage();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentListView( const AgentListView& );            //!< Copy constructor
    AgentListView& operator=( const AgentListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const AutomatDecisions& );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void keyPressEvent( QKeyEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    const Model& model_;
    ModelBuilder& modelBuilder_;
    //@}
};

#endif // __AgentListView_h_
