// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __frontend_ProfileVisitor_ABC_h_
#define __frontend_ProfileVisitor_ABC_h_

#include <boost/noncopyable.hpp>

namespace frontend
{
    class Profile;

// =============================================================================
/** @class  ProfileVisitor_ABC
    @brief  ProfileVisitor_ABC
*/
// Created: SBO 2010-11-22
// =============================================================================
class ProfileVisitor_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ProfileVisitor_ABC() {}
    virtual ~ProfileVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const Profile& profile ) = 0;
    //@}
};

}

#endif // __frontend_ProfileVisitor_ABC_h_
