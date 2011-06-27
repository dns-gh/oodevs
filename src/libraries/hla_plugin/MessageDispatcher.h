// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MessageDispatcher_h
#define plugins_hla_MessageDispatcher_h

#include "MessageDispatcher_ABC.h"
#include "MessageHandler_ABC.h"
#include <vector>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MessageDispatcher
    @brief  Message dispatcher
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
class MessageDispatcher : public MessageDispatcher_ABC< Category >
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageDispatcher() {}
    virtual ~MessageDispatcher() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( MessageHandler_ABC< Category >& handler )
    {
        handlers_.push_back( &handler );
    }
    virtual void Unregister( MessageHandler_ABC< Category >& handler )
    {
        handlers_.erase( std::remove( handlers_.begin(), handlers_.end(), &handler ) );
    }
    virtual void Dispatch( const Category& message )
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

#endif // plugins_hla_MessageDispatcher_h
