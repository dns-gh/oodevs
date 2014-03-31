// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionFactory_h_
#define __MissionFactory_h_

#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Mission;
    class FragOrder;
    class MissionType;
    class FragOrderType;

// =============================================================================
/** @class  MissionFactory
    @brief  MissionFactory
*/
// Created: AGE 2006-03-14
// =============================================================================
class MissionFactory : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionFactory( const tools::Resolver_ABC< MissionType, std::string >& unitMissions,
                             const tools::Resolver_ABC< MissionType, std::string >& automatMissions,
                             const tools::Resolver_ABC< MissionType, std::string >& populationMissions,
                             const tools::Resolver_ABC< FragOrderType, std::string >& fragOrders );
    virtual ~MissionFactory();
    //@}

    //! @name Operations
    //@{
    virtual Mission* CreateAgentMission( const std::string& name ) const;
    virtual Mission* CreateAutomatMission( const std::string& name ) const;
    virtual Mission* CreatePopulationMission( const std::string& name ) const;
    virtual FragOrder* CreateFragOrder( const std::string& name ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< MissionType, std::string >& unitMissions_;
    const tools::Resolver_ABC< MissionType, std::string >& automatMissions_;
    const tools::Resolver_ABC< MissionType, std::string >& populationMissions_;
    const tools::Resolver_ABC< FragOrderType, std::string >& fragOrders_;
    //@}
};

}

#endif // __MissionFactory_h_
