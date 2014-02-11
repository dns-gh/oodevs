// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ContextHandler_ABC_h
#define plugins_hla_ContextHandler_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace simulation
{
    class UnitMagicAction;
    class ObjectMagicAction;
}

namespace plugins
{
namespace hla
{
    template< typename MessageResponse > class ResponseObserver_ABC;

// =============================================================================
/** @class  ContextHandler_ABC
    @brief  Context handler definition
*/
// Created: SLI 2011-09-09
// =============================================================================
template< typename MessageResponse >
class ContextHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ContextHandler_ABC() {}
    virtual ~ContextHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( ResponseObserver_ABC< MessageResponse >& observer ) = 0;
    virtual void Unregister( ResponseObserver_ABC< MessageResponse >& observer ) = 0;
    virtual void Send( simulation::UnitMagicAction& message, const std::string& identifier ) = 0;
    virtual void Send( simulation::ObjectMagicAction& message, const std::string& identifier ) = 0;
    //@}
};

}
}

#endif // plugins_hla_ContextHandler_ABC_h
