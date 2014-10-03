// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef UUID_FACTORY_ABC_H
#define UUID_FACTORY_ABC_H

#include <boost/noncopyable.hpp>

namespace boost
{
namespace uuids
{
    struct uuid;
}
}

namespace host
{
    typedef boost::uuids::uuid Uuid;

// =============================================================================
/** @class  UuidFactory_ABC
    @brief  UuidFactory_ABC interface
*/
// Created: BAX 2012-03-19
// =============================================================================
struct UuidFactory_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             UuidFactory_ABC() {}
    virtual ~UuidFactory_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual Uuid Create() const = 0;
    //@}
};
}

#endif // UUID_FACTORY_ABC_H
