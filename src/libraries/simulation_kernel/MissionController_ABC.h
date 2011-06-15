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

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class MIL_Mission_ABC;
class MIL_AgentPion;
class AgentFactory_ABC;

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
    virtual void Initialize( AgentFactory_ABC& factory ) = 0;
    virtual void Start( boost::shared_ptr< MIL_Mission_ABC > mission ) = 0;
    virtual void Stop( boost::shared_ptr< MIL_Mission_ABC > mission ) = 0;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int )
    {
        // NOTHING
    }
    //@}
};

#endif // __MissionController_ABC_h_
