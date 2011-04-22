// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Logger_h_
#define __Logger_h_

#include "Listener_ABC.h"

namespace mission_tester
{
// =============================================================================
/** @class  Logger
    @brief  Logger
*/
// Created: PHC 2011-04-06
// =============================================================================
class Logger : public Listener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Logger( std::ostream& os );
    virtual ~Logger();
    //@}

    //! @name Operations
    //@{
    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const;
    virtual void MissionAcknowledged( const sword::Tasker& tasker ) const;
    virtual void MissionErrorAck( const sword::Tasker& tasker ) const;
    virtual void ConnectionSucceeded( const std::string& endpoint ) const;
    virtual void AuthenticationSucceeded( const std::string& profile ) const;
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::MissionType& mission, const kernel::OrderParameter& parameter ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::ostream& os_;
    //@}
};
}

#endif // __Logger_h_
