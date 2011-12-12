// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileFactory_ABC_h_
#define __ProfileFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

class UserProfile;

// =============================================================================
/** @class  ProfileFactory_ABC
    @brief  ProfileFactory_ABC
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfileFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileFactory_ABC() {}
    virtual ~ProfileFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual UserProfile* Create( xml::xistream& xis ) const = 0;
    virtual UserProfile* Create( const QString& name ) const = 0;
    //@}
};

#endif // __ProfileFactory_ABC_h_
