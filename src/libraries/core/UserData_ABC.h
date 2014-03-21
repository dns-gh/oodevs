// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_USER_DATA_ABC_H
#define CORE_USER_DATA_ABC_H

#include <boost/noncopyable.hpp>

namespace core
{
// =============================================================================
/** @class  UserData_ABC
    @brief  User data definition
*/
// Created: SLI 2012-01-02
// =============================================================================
class UserData_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UserData_ABC() {}
    virtual ~UserData_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void* Get() const = 0;

    template< typename Archive >
    void serialize( Archive&, const unsigned int )
    {}
    //@}
};

}

#endif // CORE_USER_DATA_ABC_H
