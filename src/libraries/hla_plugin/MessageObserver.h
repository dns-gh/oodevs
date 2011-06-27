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
#include "MessageDispatcher_ABC.h"
#include "MessageHandler.h"
#include <memory>
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#undef BOOST_TYPEOF_SILENT

#define CONNECT( controller, message, name ) MessageObserver< message >::Connect< BOOST_TYPEOF( controller )::category_type, message >( controller, *this, &BOOST_TYPEOF( controller )::category_type::has_##name, &BOOST_TYPEOF( controller )::category_type::##name )

namespace plugins
{
namespace hla
{
    template< typename Category > class MessageDispatcher_ABC;

// =============================================================================
/** @class  MessageObserver
    @brief  Message observer
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Message >
class MessageObserver : public MessageObserver_ABC< Message >
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageObserver() {}
    virtual ~MessageObserver() {}
    //@}

protected:
    //! @name Operations
    //@{
    template< typename Category, typename Message >
    void Connect( MessageDispatcher_ABC< Category >& controller, MessageObserver_ABC< Message >& observer,
                  typename MessageHandler< Category, Message >::T_Checker checker,
                  typename MessageHandler< Category, Message >::T_Retriever retriever )
    {
        handler_.reset( new MessageHandler< Category, Message >( controller, observer, checker, retriever ) );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Handler_ABC > handler_;
    //@}
};

}
}

#endif // plugins_hla_MessageObserver_h
