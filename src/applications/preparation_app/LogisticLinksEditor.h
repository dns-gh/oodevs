// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticLinksEditor_h_
#define __LogisticLinksEditor_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Team_ABC;
}

class LinkGenerator;

// =============================================================================
/** @class  LogisticLinksEditor
    @brief  Logistic links editor
*/
// Created: LGY 2011-10-12
// =============================================================================
class LogisticLinksEditor : public QObject
                          , public tools::Observer_ABC
                          , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                          , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                          , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticLinksEditor( QWidget* parent, kernel::Controllers& controllers );
    virtual ~LogisticLinksEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    void Create( const QString& title, const kernel::Entity_ABC& entity,
                 kernel::ContextMenu& menu, QSignalMapper& mapper );
    //@}

private slots:
    //! @name slots
    //@{
    void Generate();
    void GenerateFromAutomat();
    void GenerateFromFormation();
    void Remove();
    void RemoveFromAutomat();
    void RemoveFromFormation();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::unique_ptr< LinkGenerator > pGenerator_;
    std::unique_ptr< QSignalMapper > pAddTeamMapper_;
    std::unique_ptr< QSignalMapper > pAddFormationMapper_;
    std::unique_ptr< QSignalMapper > pAddAutomatMapper_;
    std::unique_ptr< QSignalMapper > pRemoveTeamMapper_;
    std::unique_ptr< QSignalMapper > pRemoveFormationMapper_;
    std::unique_ptr< QSignalMapper > pRemoveAutomatMapper_;
    //@}
};

#endif // __LogisticLinksEditor_h_
