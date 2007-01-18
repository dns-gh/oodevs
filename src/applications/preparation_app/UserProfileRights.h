// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileRights_h_
#define __UserProfileRights_h_

#include "clients_gui/HierarchyListView.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "UserProfileRights_ABC.h"

namespace
{
    typedef gui::HierarchyListView< kernel::TacticalHierarchies > T_Parent;
}

// =============================================================================
/** @class  UserProfileRights
    @brief  UserProfileRights
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileRights : public ::T_Parent
                        , public UserProfileRights_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileRights( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::SymbolIcons& icons );
    virtual ~UserProfileRights();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( QListViewItem*, const QPoint&, int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileRights( const UserProfileRights& );            //!< Copy constructor
    UserProfileRights& operator=( const UserProfileRights& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    virtual void showEvent( QShowEvent* event );
    //@}
};

#endif // __UserProfileRights_h_
