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
#include <boost/shared_ptr.hpp>
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/identity.hpp>
#undef BOOST_TYPEOF_SILENT
#include <map>

#define CONNECT( sender, receiver, name ) { \
    typedef BOOST_TYPEOF( sender ) T_Sender; \
    (receiver).MessageObserver< boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &T_Sender::category_type::name ) >::type >::type >::type >::\
     Connect< T_Sender::category_type, boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &T_Sender::category_type::name ) >::type >::type >::type >\
     ( sender, (receiver), &T_Sender::category_type::has_##name, &T_Sender::category_type::name ); }

#define DISCONNECT( sender, receiver, name ) { \
    typedef BOOST_TYPEOF( sender ) T_Sender; \
    (receiver).MessageObserver< boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &T_Sender::category_type::name ) >::type >::type >::type >::\
    Disconnect< T_Sender::category_type, boost::remove_const< boost::remove_reference< boost::function_types::result_type< BOOST_TYPEOF( &T_Sender::category_type::name ) >::type >::type >::type >( sender ); }


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
    template< typename Category, typename Message2 >
    void Connect( MessageController_ABC< Category >& controller, MessageObserver_ABC< Message2 >& observer,
                  typename MessageHandler< Category, Message2 >::T_Checker checker,
                  typename MessageHandler< Category, Message2 >::T_Retriever retriever )
    {
        handlers_[ &controller ].reset( new MessageHandler< Category, Message2 >( controller, observer, checker, retriever ) );
    }
    template< typename Category, typename Message2 >
    void Disconnect( MessageController_ABC< Category >& controller )
    {
        handlers_.erase( &controller );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::map< void*, boost::shared_ptr< Handler_ABC > > handlers_;
    //@}
};

}

#endif // tools_MessageObserver_h
