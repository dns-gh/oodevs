// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************
#ifndef DAEMON_H__
#define DAEMON_H__

#include <boost/noncopyable.hpp>
#include <functional>
#include <string>
#include <vector>

namespace cpplog
{
    class BaseLogger;
}

namespace runtime
{
    struct Runtime_ABC;
// =============================================================================
/** @class  Daemon
    @brief  Daemon definition
*/
// Created: BAX 2012-06-12
// =============================================================================
class Daemon : public boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
     Daemon( cpplog::BaseLogger& log, const Runtime_ABC& runtime );
    ~Daemon();
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::vector< std::string > T_Args;
    typedef std::function< void( void ) > T_Waiter;
    typedef std::function< int( const T_Waiter& ) > T_Task;
    //@}

    //! @name Methods
    //@{
    bool Register( const std::string& name, const T_Args& args, const std::string& username, const std::string& password );
    bool Unregister( const std::string& name );
    bool Run( const T_Task& task );
    //@}

private:
    //! @name Private members
    //@{
    struct Private;
    const  std::unique_ptr< Private > private_;
    //@}
};
}

#endif//DAEMON_H__
