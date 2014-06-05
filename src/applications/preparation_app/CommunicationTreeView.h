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
                            , public tools::ElementObserver_ABC< kernel::KnowledgeGroup_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC > // LTO
                            , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC > // LTO
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             CommunicationTreeView( const QString& objectName,
                                    kernel::Controllers& controllers,
                                    const kernel::Profile_ABC& profile,
                                    gui::ModelObserver_ABC& modelObserver,
                                    const gui::EntitySymbols& symbols,
                                    gui::ChangeSuperiorDialog& changeSuperiorDialog,
                                    QWidget* parent );
    virtual ~CommunicationTreeView();
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
    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyCreated( const kernel::KnowledgeGroup_ABC& kg );
    virtual void NotifyDeleted( const kernel::KnowledgeGroup_ABC& kg );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    // LTO begin
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& knowledgegroup, kernel::ContextMenu& menu );
    // LTO end
    virtual void Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target );

    void UpdateFonts( const kernel::KnowledgeGroup_ABC& kg, bool deleted );
    void UpdateLongName( const kernel::Entity_ABC& entity );
    virtual void keyPressEvent( QKeyEvent* event );

    void EnableKnowledgeGroupEdition( bool optional, gui::ChangeSuperiorDialog& dialog );
    //@}

private:
    //! @name Member data
    //@{
    gui::ChangeSuperiorDialog& changeSuperiorDialog_;
    //@}
};

#endif // __CommunicationTreeView_h_
