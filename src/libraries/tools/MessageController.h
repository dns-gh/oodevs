// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_MessageController_h
#define tools_MessageController_h

#include "MessageController_ABC.h"
#include "MessageHandler_ABC.h"
#include <list>

namespace tools
{
// =============================================================================
/** @class  MessageController
    @brief  Message controller
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
class MessageController : public MessageController_ABC< Category >
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageController() {}
    virtual ~MessageController() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( MessageHandler_ABC< Category >& handler )
    {
        handlers_.push_back( &handler );
    }
    virtual void Unregister( MessageHandler_ABC< Category >& handler )
    {
        handlers_.erase( std::remove( handlers_.begin(), handlers_.end(), &handler ), handlers_.end() );
    }
    virtual void Dispatch( const Category& message, int context = -1 )
    {
        for( T_Handlers::iterator it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Notify( message, context );
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< MessageHandler_ABC< Category >* > T_Handlers;
    //@}

private:
    //! @name Member data
    //@{
    T_Handlers handlers_;
    //@}
};

}

#endif // tools_MessageController_h
