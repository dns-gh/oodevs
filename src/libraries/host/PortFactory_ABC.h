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
#include <functional>

namespace boost
{
    template< typename T > class shared_ptr;
}

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

typedef boost::shared_ptr< const Port_ABC > Port;

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
    virtual Port Create() = 0;
    virtual Port Create( int port ) = 0;
    virtual bool WaitConnected( int port, const std::function< bool() >& alive ) = 0;
    //@}
};
}

#endif // PORT_FACTORY_ABC_H
