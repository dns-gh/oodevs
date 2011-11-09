// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommunicationListView_h_
#define __CommunicationListView_h_

#include "clients_gui/ChangeSuperior_ABC.h"
#include "clients_gui/HierarchyListView.h"
#include "clients_kernel/CommunicationHierarchies.h"

namespace gui
{
    class ChangeSuperiorDialog;
}

namespace kernel
{
    class Automat_ABC;
    class KnowledgeGroup_ABC; // LTO
    class Team_ABC; // LTO
}

class ModelBuilder;

// =============================================================================
/** @class  CommunicationListView
    @brief  CommunicationListView
*/
// Created: SBO 2006-09-25
// =============================================================================
class CommunicationListView : public gui::HierarchyListView< kernel::CommunicationHierarchies >
                            , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC > // LTO
                            , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC > // LTO
                            , public gui::ChangeSuperior_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             CommunicationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ModelBuilder& modelBuilder );
    virtual ~CommunicationListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual void DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeKnowledgeGroup();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void hideEvent( QHideEvent* event );
    virtual void NotifyUpdated( const kernel::Entity_ABC& );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual bool Drop( const kernel::Entity_ABC& draggedEntity, const kernel::Entity_ABC& target );
    // LTO begin
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& knowledgegroup, kernel::ContextMenu& menu );
    // LTO end
    virtual void keyPressEvent( QKeyEvent* event );
   //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    ModelBuilder& modelBuilder_;
    gui::ChangeSuperiorDialog* changeSuperiorDialog_;
    //@}
};

#endif // __CommunicationListView_h_
