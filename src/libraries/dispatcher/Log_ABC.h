// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_Log_ABC_h
#define dispatcher_Log_ABC_h

#include <boost/noncopyable.hpp>

namespace dispatcher
{
// =============================================================================
/** @class  Log_ABC
    @brief  Log declaration
*/
// Created: MCO 2013-10-01
// =============================================================================
class Log_ABC : boost::noncopyable
{
public:
    Log_ABC( bool active )
        : active_( active )
    {}
    virtual ~Log_ABC()
    {}

    template< typename Serializable >
    void Write( const Serializable& s )
    {
        if( active_ )
            DoWrite( boost::lexical_cast< std::string >( s ) );
    }

private:
    virtual void DoWrite( const std::string& line ) = 0;

private:
    const bool active_;
};

}

#endif // dispatcher_Log_ABC_h
