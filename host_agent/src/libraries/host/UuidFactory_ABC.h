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
#include <boost/uuid/uuid.hpp>

namespace host
{

// =============================================================================
/** @class  UuidFactory_ABC
    @brief  UuidFactory_ABC interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class UuidFactory_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UuidFactory_ABC() {}
    virtual ~UuidFactory_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual boost::uuids::uuid Create() const = 0;
    //@}
};

}

#endif // UUID_FACTORY_ABC_H
