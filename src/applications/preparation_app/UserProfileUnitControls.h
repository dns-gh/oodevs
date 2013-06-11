// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileUnitControls_h_
#define __UserProfileUnitControls_h_

#include "clients_gui/HierarchyListView.h"
#include "preparation/ProfileHierarchies_ABC.h"
#include "UserProfileControls_ABC.h"

namespace gui
{
    class ValuedListItem;
}

class Model;

namespace
{
    typedef gui::HierarchyListView< ProfileHierarchies_ABC > T_Parent;
}

// =============================================================================
/** @class  UserProfileUnitControls
    @brief  User profile unit controls
*/
// Created: LGY 2011-09-13
// =============================================================================
class UserProfileUnitControls : public T_Parent
                              , public UserProfileControls_ABC
                              , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileUnitControls( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                      const gui::EntitySymbols& icons, ControlsChecker_ABC& checker, Model& model );
    virtual ~UserProfileUnitControls();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateFilter();
    virtual void HideAssignedAutomats();
    virtual void ShowAssignedAutomats();
    virtual void RemoveFilter();

    virtual void Display( UserProfile& profile );
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    void Show();

    virtual void showEvent( QShowEvent* event );
    virtual void hideEvent( QHideEvent* event );

    virtual void NotifyCreated( const ProfileHierarchies_ABC& hierarchy );
    virtual void NotifyUpdated( const ProfileHierarchies_ABC& hierarchy );
    virtual void NotifyDeleted( const ProfileHierarchies_ABC& hierarchy );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem*, const QPoint&, int );
    void OnItemExpanded( Q3ListViewItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int );

    void Expand( gui::ValuedListItem* item );
    bool ApplyHideFilter( gui::ValuedListItem* item ) const;
    bool ApplyShowFilter( gui::ValuedListItem* item ) const;

    bool Accept( const ProfileHierarchies_ABC& hierarchy ) const;

    virtual Q3DragObject* dragObject();
    //@}

private:
    //! @name Member Data
    //@{
    Model& model_;
    ControlsChecker_ABC& checker_;
    boost::function< bool ( gui::ValuedListItem* ) > func_;
    //@}
};

#endif // __UserProfileUnitControls_h_
