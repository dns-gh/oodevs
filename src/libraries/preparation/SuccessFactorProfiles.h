// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorProfiles_h_
#define __SuccessFactorProfiles_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Controllers;
}

namespace xml
{
    class xistream;
    class xostream;
}

class ProfilesModel;
class UserProfile;

// =============================================================================
/** @class  SuccessFactorProfiles
    @brief  SuccessFactorProfiles
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorProfiles : public tools::Resolver< const UserProfile, QString >
                            , public tools::Observer_ABC
                            , public tools::ElementObserver_ABC< UserProfile >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SuccessFactorProfiles( kernel::Controllers& controllers );
             SuccessFactorProfiles( kernel::Controllers& controllers, xml::xistream& xis, const ProfilesModel& model );
    virtual ~SuccessFactorProfiles();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorProfiles( const SuccessFactorProfiles& );            //!< Copy constructor
    SuccessFactorProfiles& operator=( const SuccessFactorProfiles& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );
    void ReadProfile( xml::xistream& xis, const ProfilesModel& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __SuccessFactorProfiles_h_
