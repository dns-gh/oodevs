// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MessageHandler_h
#define plugins_hla_MessageHandler_h

#include "MessageHandler_ABC.h"
#include "MessageObserver_ABC.h"
#include "MessageController.h"
#include <boost/function.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MessageHandler
    @brief  MessageHandler
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category, typename Message >
class MessageHandler : public MessageHandler_ABC< Category >
{
public:
    //! @name Constructors/Destructor
    //@{
    MessageHandler( MessageController< Category >& controller,
                    MessageObserver_ABC< Message >& observer,
                    boost::function< bool( const Category& ) > checker,
                    boost::function< const Message&( const Category& ) > retriever )
        : controller_( controller )
        , observer_  ( observer )
        , checker_   ( checker )
        , retriever_ ( retriever )
    {
        controller_.Register( *this );
    }
    virtual ~MessageHandler()
    {
        controller_.Unregister( *this );
    }
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const Category& message )
    {
        if( checker_( message ) )
            observer_.Notify( retriever_( message ) );
    }
    //@}

private:
    //! @name Member data
    //@{
    MessageController< Category >& controller_;
    MessageObserver_ABC< Message >& observer_;
    boost::function< bool( const Category& ) > checker_;
    boost::function< const Message&( const Category& ) > retriever_;
    //@}
};

}
}

#endif // plugins_hla_MessageHandler_h
