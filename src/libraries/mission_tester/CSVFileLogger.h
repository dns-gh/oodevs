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
    explicit CSVFileLogger( const std::string& filename );
    virtual ~CSVFileLogger();
    //@}

    //! @name Operations
    //@{
    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void MissionAcknowledged( const sword::Tasker& tasker );
    virtual void FragOrderAcknowledged( const sword::Tasker& tasker );
    virtual void MissionErrorAck( const sword::Tasker& tasker ) const;
    virtual void FragOrderErrorAck( const sword::Tasker& tasker ) const;
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter ) const;
    virtual void ConnectionSucceeded( const std::string& endpoint ) const;
    virtual void AuthenticationSucceeded( const std::string& profile ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Created( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const std::string& orderType );
    //@}
};
}

#endif // __Logger_h_
