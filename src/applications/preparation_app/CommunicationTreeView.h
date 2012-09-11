// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __CommunicationTreeView_h_
#define __CommunicationTreeView_h_

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/ChangeSuperior_ABC.h"
#include "clients_gui/HierarchyTreeView.h"

namespace gui
{
    class ChangeSuperiorDialog;
}

// =============================================================================
/** @class  CommunicationTreeView
    @brief  CommunicationTreeView
*/
// Created: JSR 2012-09-11
// =============================================================================
class CommunicationTreeView : public gui::HierarchyTreeView< kernel::CommunicationHierarchies >
                            //, public tools::ElementObserver_ABC< kernel::Entity_ABC >
                            //, public tools::ElementObserver_ABC< kernel::KnowledgeGroup_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC > // LTO
                            , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC > // LTO
                            , public gui::ChangeSuperior_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             CommunicationTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, QWidget* parent );
    virtual ~CommunicationTreeView();
    //@}

    //! @name Filters/Sort
    //@{
    virtual bool LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const;
    //@}

    //! @name Operations
    //@{
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual void DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateCommunication();
    void OnChangeKnowledgeGroup();
    //@}

private:
    //! @name Helpers
    //@{
    //virtual void NotifyUpdated( const kernel::Entity_ABC& );
    //virtual void NotifyDeleted( const kernel::KnowledgeGroup_ABC& );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    // LTO begin
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& knowledgegroup, kernel::ContextMenu& menu );
    // LTO end
    virtual void Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    gui::ChangeSuperiorDialog* changeSuperiorDialog_;
    //@}
};

#endif // __CommunicationTreeView_h_
