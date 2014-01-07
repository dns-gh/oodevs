// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PORT_FACTORY_H
#define PORT_FACTORY_H

#include "PortFactory_ABC.h"

#include <boost/thread/mutex.hpp>
#include <set>

namespace boost
{
    class shared_mutex;
}

namespace host
{
// =============================================================================
/** @class  PortFactory
    @brief  PortFactory interface
*/
// Created: BAX 2012-03-20
// =============================================================================
class PortFactory : public PortFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PortFactory( int period, int min, int max );
    virtual ~PortFactory();
    //@}

    //! @name Methods
    //@{
    virtual Port Create();
    virtual Port Create( int port );
    virtual bool WaitConnected( int port, const std::function< bool() >& alive );
    //@}

private:
    //! @name Private typedefs
    //@{
    typedef std::set< int > T_Ports;
    //@}

    //! @name Private methods
    //@{
    Port Acquire( int port );
    void Release( int port );
    //@}

    //! @name Private members
    //@{
    boost::mutex access_;
    const int period_;
    const int min_;
    const int max_;
    T_Ports ports_;
    //@}
};
}

#endif // PORT_FACTORY_H
