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

// =============================================================================
/** @class  UserProfileUnitControls
    @brief  User profile unit controls
*/
// Created: LGY 2011-09-13
// =============================================================================
class UserProfileUnitControls : public gui::HierarchyListView< ProfileHierarchies_ABC >
                              , public UserProfileControls_ABC
                              , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileUnitControls( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                      gui::EntitySymbols& icons, ControlsChecker_ABC& checker, Model& model );
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

    bool ApplyHideFilter( gui::ValuedListItem* item ) const;
    bool ApplyShowFilter( gui::ValuedListItem* item ) const;
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
