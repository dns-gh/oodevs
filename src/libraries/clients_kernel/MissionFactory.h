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

#include "Resolver_ABC.h"

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
class MissionFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionFactory( const Resolver_ABC< MissionType, QString >& unitMissions, const Resolver_ABC< MissionType, QString >& automatMissions
                           , const Resolver_ABC< MissionType, QString >& populationMissions, const Resolver_ABC< FragOrderType, QString >& fragOrders );
    virtual ~MissionFactory();
    //@}

    //! @name Operations
    //@{
    virtual Mission*   CreateAgentMission( const std::string& name );
    virtual Mission*   CreateAutomatMission( const std::string& name );
    virtual Mission*   CreatePopulationMission( const std::string& name );
    virtual FragOrder* CreateFragOrder( const std::string& name );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionFactory( const MissionFactory& );            //!< Copy constructor
    MissionFactory& operator=( const MissionFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    Mission* AddFragOrders( Mission* mission );
    //@}

private:
    //! @name Member data
    //@{
    const Resolver_ABC< MissionType, QString >& unitMissions_;
    const Resolver_ABC< MissionType, QString >& automatMissions_;
    const Resolver_ABC< MissionType, QString >& populationMissions_;
    const Resolver_ABC< FragOrderType, QString >& fragOrders_;
    //@}
};

}

#endif // __MissionFactory_h_
