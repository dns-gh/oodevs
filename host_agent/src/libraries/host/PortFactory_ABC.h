// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PORT_FACTORY_ABC_H
#define PORT_FACTORY_ABC_H

#include <boost/noncopyable.hpp>
#include <memory>

namespace host
{
// =============================================================================
/** @class  Port_ABC
    @brief  Port_ABC interface
*/
// Created: BAX 2012-03-20
// =============================================================================
struct Port_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Port_ABC() {}
    virtual ~Port_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual int Get() const = 0;
    //@}
};

// =============================================================================
/** @class  PortFactory_ABC
    @brief  PortFactory_ABC interface
*/
// Created: BAX 2012-03-20
// =============================================================================
struct PortFactory_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             PortFactory_ABC() {}
    virtual ~PortFactory_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual std::auto_ptr< Port_ABC > Create() = 0;
    virtual std::auto_ptr< Port_ABC > Create( int port ) = 0;
    //@}
};

}

#endif // PORT_FACTORY_ABC_H
