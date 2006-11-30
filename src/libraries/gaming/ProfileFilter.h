// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileFilter_h_
#define __ProfileFilter_h_

#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel {
    class Entity_ABC;
    class Controllers;
    class Hierarchies;
    class TacticalHierarchies;
    class CommunicationHierarchies;
}

// =============================================================================
/** @class  ProfileFilter
    @brief  ProfileFilter
*/
// Created: AGE 2006-11-29
// =============================================================================
class ProfileFilter : public kernel::Profile_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ProfileFilter( kernel::Controllers& controllers, const kernel::Profile_ABC& forward );
    virtual ~ProfileFilter();
    //@}

    //! @name Operations
    //@{
    virtual bool IsVisible   ( const kernel::Entity_ABC& entity ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic  ( const kernel::Entity_ABC& entity ) const;

    void SetFilter( const kernel::Entity_ABC& entity );
    void RemoveFilter();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileFilter( const ProfileFilter& );            //!< Copy constructor
    ProfileFilter& operator=( const ProfileFilter& ); //!< Assignement operator
   //@}

    //! @name Helpers
    //@{
    bool IsInHierarchy( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Profile_ABC& forward_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::TacticalHierarchies* tHierarchies_;
    const kernel::CommunicationHierarchies* cHierarchies_;
    //@}
};

#endif // __ProfileFilter_h_
