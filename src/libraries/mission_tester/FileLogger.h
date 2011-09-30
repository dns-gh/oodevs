// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FileLogger_h_
#define __FileLogger_h_

#include "Logger.h"
#include <boost/filesystem/path.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  FileLogger
    @brief  FileLogger
*/
// Created: PHC 2011-04-07
// =============================================================================
class FileLogger : public Logger_ABC
                 , public Listener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileLogger();
             FileLogger( const boost::filesystem::path& path, const std::string& extension = ".log" );
    virtual ~FileLogger();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Write( const std::string& input );

    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void MagicActionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void MissionAcknowledged( const sword::Tasker& tasker );
    virtual void FragOrderAcknowledged( const sword::Tasker& tasker );
    virtual void MissionErrorAck( const sword::Tasker& tasker );
    virtual void FragOrderErrorAck( const sword::Tasker& tasker );
    virtual void MagicActionErrorAck( const sword::Tasker& tasker );
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void AuthenticationSucceeded( const std::string& profile );
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter );
    //@}

protected:
    //! @name helpers
    //@{
    void UpdateLogger();
    virtual void WriteHeader();
    //@}

    //! @name member data
    //@{
    unsigned int lines_;
    unsigned int files_;
    const boost::filesystem::path path_;
    const std::string extension_;
    std::auto_ptr< std::ostream > os_;
    std::auto_ptr< Logger > logger_;
    //@}
};

}

#endif // __FileLogger_h_
