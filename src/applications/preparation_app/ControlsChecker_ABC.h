// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ControlsChecker_ABC_h_
#define __ControlsChecker_ABC_h_

#include <boost/noncopyable.hpp>
#include <map>

class UserProfile;

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  ControlsChecker_ABC
    @brief  Controls checker declaration
*/
// Created: LGY 2011-09-15
// =============================================================================
class ControlsChecker_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ControlsChecker_ABC() {}
    virtual ~ControlsChecker_ABC() {}
    //@}

    //! @name Types
    //@{
    typedef std::map< const UserProfile*, UserProfile* > T_ProfileEditors;
    typedef T_ProfileEditors::iterator                  IT_ProfileEditors;
    typedef T_ProfileEditors::const_iterator           CIT_ProfileEditors;
    //@}

    //! @name Operations
    //@{
    virtual void Display( const T_ProfileEditors& editors ) = 0;
    virtual std::string CheckControl( const UserProfile& profile, const kernel::Entity_ABC& entity ) const = 0;
    virtual void Update( const UserProfile& profile, const kernel::Entity_ABC& entity ) = 0;
    //@}
};

#endif // __ControlsChecker_ABC_h_
