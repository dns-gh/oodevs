// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ProfileSelection_h_
#define __ProfileSelection_h_

#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class UserProfile_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

class ProfilesModel;

// =============================================================================
/** @class  ProfileSelection
    @brief  A set of selected profiles
*/
// Created: SBO 2009-06-15
// =============================================================================
class ProfileSelection : public tools::Resolver< const kernel::UserProfile_ABC, QString >
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::UserProfile_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ProfileSelection( kernel::Controllers& controllers );
             ProfileSelection( const ProfileSelection& );
             ProfileSelection( kernel::Controllers& controllers, xml::xistream& xis, const ProfilesModel& model );
    virtual ~ProfileSelection();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileSelection& operator=( const ProfileSelection& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyDeleted( const kernel::UserProfile_ABC& profile );
    void ReadProfile( xml::xistream& xis, const ProfilesModel& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __ProfileSelection_h_
