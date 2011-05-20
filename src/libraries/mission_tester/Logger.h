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
#include <boost/date_time/posix_time/posix_time.hpp>

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
    virtual void MissionCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void FragOrderCreated( const kernel::Entity_ABC& target, const kernel::OrderType& mission );
    virtual void MissionAcknowledged( const sword::Tasker& tasker );
    virtual void FragOrderAcknowledged( const sword::Tasker& tasker );
    virtual void MissionErrorAck( const sword::Tasker& tasker ) const;
    virtual void FragOrderErrorAck( const sword::Tasker& tasker ) const;
    virtual void ConnectionSucceeded( const std::string& endpoint ) const;
    virtual void AuthenticationSucceeded( const std::string& profile ) const;
    virtual void ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const kernel::OrderParameter& parameter ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    void Created( const kernel::Entity_ABC& target, const kernel::OrderType& mission, const std::string& orderType );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, std::pair< unsigned int, std::string > > T_Missions;
    typedef T_Missions::iterator                                            IT_Missions;
    //@}

    //! @name Member data
    //@{
    std::ostream& os_;
    std::auto_ptr< boost::posix_time::time_facet > facet_;
    T_Missions missions_;
    //@}
};
}

#endif // __Logger_h_
