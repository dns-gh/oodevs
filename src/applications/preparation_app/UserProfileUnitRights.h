// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileUnitRights_h_
#define __UserProfileUnitRights_h_

#include "clients_gui/HierarchyTreeView.h"
#include "clients_gui/DummyModelObserver.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "UserProfileRights_ABC.h"

namespace
{
    typedef gui::HierarchyTreeView< kernel::TacticalHierarchies > T_Parent;
}

// =============================================================================
/** @class  UserProfileUnitRights
    @brief  UserProfileUnitRights
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileUnitRights : public ::T_Parent
                            , public UserProfileRights_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    UserProfileUnitRights( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& icons, const QString& name );
    virtual ~UserProfileUnitRights();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* GetWidget();
    virtual void Display( UserProfile& profile );
    virtual void AdditionalUpdateItem( QStandardItem& entityItem, const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( const QModelIndex& index );
    virtual void OnSelect( const QItemSelection& /*selected*/, const QItemSelection& /* deselected */ );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    gui::DummyModelObserver observer_;
    //@}
};

#endif // __UserProfileUnitRights_h_
