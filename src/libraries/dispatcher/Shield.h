// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_Shield_h
#define dispatcher_Shield_h

#include <boost/noncopyable.hpp>
#include <memory>

namespace shield
{
    class Server;
    class Listener_ABC;
}

namespace dispatcher
{
    class Config;

// =============================================================================
/** @class  Shield
    @brief  Shield
*/
// Created: MCO 2010-09-30
// =============================================================================
class Shield : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Shield( const Config& config );
    virtual ~Shield();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< shield::Listener_ABC > logger_;
    std::auto_ptr< shield::Server > server_;
    //@}
};

}

#endif // dispatcher_Shield_h
