// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CSVFileLogger_h_
#define __CSVFileLogger_h_

#include "FileLogger.h"
#include <boost/filesystem/path.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  CSVFileLogger
    @brief  CSVFileLogger
*/
// Created: PHC 2011-04-06
// =============================================================================
class CSVFileLogger : public FileLogger
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CSVFileLogger( const boost::filesystem::path& path );
    virtual ~CSVFileLogger();
    //@}

    //! @name Operations
    //@{
    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void MagicActionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void MissionAcknowledged( const sword::Tasker& tasker );
    virtual void FragOrderAcknowledged( const sword::Tasker& tasker );
    virtual void MissionErrorAck( const sword::Tasker& tasker );
    virtual void FragOrderErrorAck( const sword::Tasker& tasker );
    virtual void MagicActionErrorAck( const sword::Tasker& tasker );
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter );
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void AuthenticationSucceeded( const std::string& profile );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void WriteHeader();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, std::pair< unsigned int, std::string > > T_Missions;
    typedef T_Missions::iterator                                            IT_Missions;
    //@}

private:
    //! @name Member data
    //@{
    T_Missions missions_;
    FileLogger fileLogger_;
    //@}
};

}

#endif // __CSVFileLogger_h_
