// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileFilterToolbar_h_
#define __ProfileFilterToolbar_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel 
{
    class Controllers;
    class Entity_ABC;
}
class ProfileFilter;

// =============================================================================
/** @class  ProfileFilterToolbar
    @brief  ProfileFilterToolbar
*/
// Created: AGE 2006-12-13
// =============================================================================
class ProfileFilterToolbar : public QToolBar
                           , public kernel::Observer_ABC
                           , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProfileFilterToolbar( QMainWindow* pParent, kernel::Controllers& controllers, ProfileFilter& filter );
    virtual ~ProfileFilterToolbar();
    //@}

    //! @name Operations
    //@{
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSetFilter();
    void OnClearFilter();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileFilterToolbar( const ProfileFilterToolbar& );            //!< Copy constructor
    ProfileFilterToolbar& operator=( const ProfileFilterToolbar& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ProfileFilter& filter_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    QLabel* label_;
    //@}
};

#endif // __ProfileFilterToolbar_h_
