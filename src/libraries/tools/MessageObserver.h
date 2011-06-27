// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_MessageObserver_h
#define tools_MessageObserver_h

#include "MessageObserver_ABC.h"
#include "MessageHandler.h"
#include <memory>
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/type_traits.hpp>
#undef BOOST_TYPEOF_SILENT

#define CONNECT( sender, receiver, name ) \
    (receiver).MessageObserver< boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &BOOST_TYPEOF( sender )::category_type::##name ) >::type >::type >::type >::\
     Connect< BOOST_TYPEOF( sender )::category_type, boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &BOOST_TYPEOF( sender )::category_type::##name ) >::type >::type >::type >\
     ( sender, (receiver), &BOOST_TYPEOF( sender )::category_type::has_##name, &BOOST_TYPEOF( sender )::category_type::##name );

#define DISCONNECT( sender, receiver, name ) \
    (receiver).MessageObserver< boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &BOOST_TYPEOF( sender )::category_type::##name ) >::type >::type >::type >::\
     Disconnect< BOOST_TYPEOF( sender )::category_type, boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &BOOST_TYPEOF( sender )::category_type::##name ) >::type >::type >::type >()

namespace tools
{
    template< typename Category > class MessageController_ABC;

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

    //! @name Operations
    //@{
    template< typename Category, typename Message >
    void Connect( MessageController_ABC< Category >& controller, MessageObserver_ABC< Message >& observer,
                  typename MessageHandler< Category, Message >::T_Checker checker,
                  typename MessageHandler< Category, Message >::T_Retriever retriever )
    {
        handler_.reset( new MessageHandler< Category, Message >( controller, observer, checker, retriever ) );
    }
    template< typename Category, typename Message >
    void Disconnect()
    {
        handler_.reset();
    }
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Handler_ABC > handler_;
    //@}
};

}

#endif // tools_MessageObserver_h
