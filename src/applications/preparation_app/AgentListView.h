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
    virtual void Display( const kernel::Team_ABC& team, gui::ValuedListItem* item );
    virtual void Display( const kernel::Agent_ABC& agent, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRename( QListViewItem* item, int col, const QString& text );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentListView( const AgentListView& );            //!< Copy constructor
    AgentListView& operator=( const AgentListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
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
