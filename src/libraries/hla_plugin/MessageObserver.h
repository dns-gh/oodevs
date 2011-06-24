// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MessageObserver_h
#define plugins_hla_MessageObserver_h

#include "MessageObserver_ABC.h"
#include "MessageHandler.h"
#include <memory>

#define MESSAGE_OBSERVER( category, message, name, controller ) MessageObserver< category, message >( *this, controller, &##category##::has_##name, &##category##::##name )

namespace plugins
{
namespace hla
{
    template< typename Category > class MessageController;

// =============================================================================
/** @class  MessageObserver
    @brief  Message observer
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category, typename Message >
class MessageObserver : public MessageObserver_ABC< Message >
{
public:
    //! @name Constructors/Destructor
    //@{
    MessageObserver( MessageObserver_ABC< Message >& observer, MessageController< Category >& controller, typename MessageHandler< Category, Message >::T_Checker checker, typename MessageHandler< Category, Message >::T_Retriever retriever )
        : handler_( new MessageHandler< Category, Message >( controller, observer, checker, retriever ) )
    {
        // NOTHING
    }
    virtual ~MessageObserver()
    {
        // NOTHING
    }
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< MessageHandler_ABC< Category > > handler_;
    //@}
};

}
}

#endif // plugins_hla_MessageObserver_h
