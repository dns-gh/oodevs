// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_DiffusionListDialog_h_
#define gui_DiffusionListDialog_h_

#include "ModalDialog.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Agent_ABC;
    class ContextMenu;
    class Controllers;
    class Entity_ABC;
    class ExtensionTypes;
    class Formation_ABC;
    class Profile_ABC;
    class Team_ABC;
}

namespace gui
{
    class DiffusionListHierarchy;
    class EntitySymbols;
    class DiffusionListEditor;

// =============================================================================
/** @class  DiffusionListDialog
    @brief  DiffusionListDialog
*/
// Created: ABR 2011-04-29
// =============================================================================
class DiffusionListDialog : public ModalDialog
                          , public tools::Observer_ABC
                          , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DiffusionListDialog( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver< kernel::Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations, const kernel::ExtensionTypes& extensions, const char* name = 0 );
    virtual ~DiffusionListDialog();
    //@}

    //! @name Accessors
    //@{
    void SetCurrentTeam( const kernel::Entity_ABC& team );
    void SetCurrentAgent( const kernel::Entity_ABC& agent );
    void SetContextMenuEntry( bool contextMenuEntry );
    //@}

public slots:
    //! @name Slots
    //@{
    void Show();
    //@}

private:
    //! @name ContextMenuObserver_ABC implementation
    //@{
    virtual void NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu );
    //@}

signals:
    //! @name Signals
    //@{
    void Accepted();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void accept();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ExtensionTypes&               extensions_;
    kernel::Controllers&                        controllers_;
    kernel::SafePointer< kernel::Entity_ABC >   currentTeam_;
    kernel::SafePointer< kernel::Entity_ABC >   currentAgent_;
    DiffusionListEditor*                        editor_;
    bool                                        contextMenuEntry_;
    //@}
};

}

#endif // gui_DiffusionListDialog_h_
