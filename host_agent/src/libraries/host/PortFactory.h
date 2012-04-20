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
    virtual std::auto_ptr< Port_ABC > Create();
    virtual std::auto_ptr< Port_ABC > Create( int port );
    //@}

private:
    //! @name Private typedefs
    //@{
    typedef std::set< int > T_Ports;
    //@}

    //! @name Private methods
    //@{
    std::auto_ptr< Port_ABC > Acquire( int port );
    void Release( int port );
    //@}

    //! @name Private members
    //@{
    const std::auto_ptr< boost::shared_mutex > access_;
    const int period_;
    const int min_;
    const int max_;
    T_Ports ports_;
    //@}
};

}

#endif // PORT_FACTORY_H
