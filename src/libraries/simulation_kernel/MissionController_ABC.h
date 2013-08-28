// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MissionController_ABC_h_
#define __MissionController_ABC_h_

#include "tools/Resolver.h"
#include <boost/shared_ptr.hpp>

class MIL_Mission_ABC;
class MIL_AgentPion;
class AgentFactory_ABC;
class PopulationFactory_ABC;

// =============================================================================
/** @class  MissionController_ABC
    @brief  Mission controller declaration
*/
// Created: LGY 2011-06-14
// =============================================================================
class MissionController_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionController_ABC() {}
    virtual ~MissionController_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( tools::Resolver< MIL_AgentPion >& resolver, PopulationFactory_ABC& populationFactory ) = 0;
    virtual void SendFullState() = 0;
    virtual void Start( boost::shared_ptr< MIL_Mission_ABC > mission ) = 0;
    virtual void Stop( boost::shared_ptr< MIL_Mission_ABC > mission ) = 0;
    virtual uint32_t AcquireId() = 0;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

#endif // __MissionController_ABC_h_
