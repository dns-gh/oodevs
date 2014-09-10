// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef RenameInterface_h
#define RenameInterface_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Drawing_ABC;
    class Entity_ABC;
    class Formation_ABC;
    class Ghost_ABC;
    class Inhabitant_ABC;
    class KnowledgeGroup_ABC;
    class Object_ABC;
    class Pathfind_ABC;
    class Population_ABC;
    class TacticalLine_ABC;
    class Team_ABC;
}

namespace gui
{
    class EntityTreeView_ABC;

// =============================================================================
/** @class  RenameInterface
    @brief  RenameInterface
*/
// Created: ABR 2014-09-03
// =============================================================================
class RenameInterface : public QObject
                      , public tools::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Drawing_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Pathfind_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
                      , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RenameInterface( kernel::Controllers& controllers, QObject* parent = 0 );
    virtual ~RenameInterface();
    //@}

    //! @name Operations
    //@{
    void AddEntityTreeView( gui::EntityTreeView_ABC* view );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRename();
    //@}

private:
    //! @name ContextMenuObserver implementations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Drawing_ABC& drawing, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& ghost, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& inhabitant, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& knowledgeGroup, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Object_ABC& objet, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Pathfind_ABC& pathfind, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& population, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& line, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu );
    //@}

    //! @name Helpers
    //@{
    void AddCommonMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > contextMenuEntity_;
    std::vector< gui::EntityTreeView_ABC* > views_;
    gui::EntityTreeView_ABC* contextMenuView_;
    //@}
};

} //! namespace gui

#endif // RenameInterface_h
