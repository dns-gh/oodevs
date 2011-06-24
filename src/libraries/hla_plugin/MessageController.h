// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MessageController_h
#define plugins_hla_MessageController_h

#include "MessageHandler_ABC.h"
#include <boost/noncopyable.hpp>
#include <vector>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MessageController
    @brief  Message controller
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
class MessageController : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageController() {}
    virtual ~MessageController() {}
    //@}

    //! @name Operations
    //@{
    void Register( MessageHandler_ABC< Category >& handler )
    {
        handlers_.push_back( &handler );
    }
    void Unregister( MessageHandler_ABC< Category >& handler )
    {
        handlers_.erase( std::remove( handlers_.begin(), handlers_.end(), &handler ) );
    }
    void Notify( const Category& message )
    {
        for( T_Handlers::iterator it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Notify( message );
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MessageHandler_ABC< Category >* > T_Handlers;
    //@}

private:
    //! @name Member data
    //@{
    T_Handlers handlers_;
    //@}
};

}
}

#endif // plugins_hla_MessageController_h
