// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileFilterObject_h_
#define __ProfileFilterObject_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "gaming/ProfileFilter.h"

// =============================================================================
/** @class  ProfileFilterObject
    @brief  ProfileFilterObject
    // $$$$ AGE 2006-11-29: le nom est a chier...
*/
// Created: AGE 2006-11-29
// =============================================================================
class ProfileFilterObject : public QObject
                          , public ProfileFilter
                          , public kernel::Observer_ABC
                          , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProfileFilterObject( QObject* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& forward );
    virtual ~ProfileFilterObject();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnFilter();
    void OnRemove();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileFilterObject( const ProfileFilterObject& );            //!< Copy constructor
    ProfileFilterObject& operator=( const ProfileFilterObject& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    //@}
};

#endif // __ProfileFilterObject_h_
