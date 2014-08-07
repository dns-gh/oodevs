 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_Logger_h
#define dispatcher_Logger_h

#include "tools/Log.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>

namespace dispatcher
{
// =============================================================================
/** @class  Logger
    @brief  Logger
*/
// Created: MCO 2011-06-27
// =============================================================================
template< typename T >
class Logger
{
public:
    //! @name Types
    //@{
    typedef std::function< void( const std::string&, const T& ) > T_ConstCallback;
    typedef std::function< void( const std::string&, T& ) >       T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Logger( tools::Log& log, const std::string& prefix, T_Callback callback )
        : log_     ( &log )
        , prefix_  ( prefix )
        , callback_( callback )
    {}
    Logger( tools::Log& log, const std::string& prefix, T_ConstCallback constCallback )
        : log_          ( &log )
        , prefix_       ( prefix )
        , constCallback_( constCallback )
    {}
    //@}

    //! @name Operators
    //@{
    void operator()( const std::string& link, const T& message ) const
    {
        log_->Write( MessageSerializer< T >( prefix_, message ) );
        if( constCallback_ )
            constCallback_( link, message );
    }
    void operator()( const std::string& link, T& message ) const
    {
        log_->Write( MessageSerializer< T >( prefix_, message ) );
        if( callback_ )
            callback_( link, message );
    }
    //@}

private:
    //! @name Types
    //@{
    template< typename T >
    class MessageSerializer : private boost::noncopyable
    {
    public:
        MessageSerializer( const std::string& prefix, const T& message )
            : prefix_ ( prefix )
            , message_( message )
        {}
        friend std::ostream& operator<<( std::ostream& s, const MessageSerializer& ms )
        {
            return s << ms.prefix_ << ms.message_.ShortDebugString();
        }
    private:
        const std::string& prefix_;
        const T& message_;
    };
    //@}

private:
    //! @name Member data
    //@{
    tools::Log* log_;
    std::string prefix_;
    T_Callback callback_;
    T_ConstCallback constCallback_;
    //@}
};

template< typename C, typename T >
std::function< void( const std::string&, const T& ) > MakeConstLogger(
    tools::Log& log, const std::string& prefix,
    C& instance, void (C::*callback)( const std::string&, const T& ) )
{
    return Logger< T >( log, prefix, Logger< T >::T_ConstCallback( boost::bind( callback, &instance, _1, _2 ) ) );
}

template< typename C, typename T >
std::function< void( const std::string&, T& ) > MakeLogger(
    tools::Log& log, const std::string& prefix,
    C& instance, void (C::*callback)( const std::string&, T& ) )
{
    return Logger< T >( log, prefix, Logger< T >::T_Callback( boost::bind( callback, &instance, _1, _2 ) ) );
}

}

#endif // dispatcher_Logger_h
