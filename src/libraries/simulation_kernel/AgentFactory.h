// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AgentFactory_h_
#define __AgentFactory_h_

#include "MIL.h"
#include "AlgorithmsFactories.h"
#include "MissionController_ABC.h"
#include <boost/noncopyable.hpp>
#include <memory>

class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_Automate;
class MIL_IDManager;
class MissionController_ABC;

namespace sword
{
    class RoleExtender_ABC;
}

// =============================================================================
/** @class  AgentFactory
    @brief  AgentFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class AgentFactory : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory( MIL_IDManager& idManager, MissionController_ABC& missionController, std::auto_ptr< AlgorithmsFactories > algorithmsFactories );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext );
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const AgentFactory* factory, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, AgentFactory* factory, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    MIL_IDManager& idManager_;
    MissionController_ABC& missionController_;
    std::auto_ptr< AlgorithmsFactories > algorithmsFactories_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( AgentFactory )

template< typename Archive >
void save_construct_data( Archive& archive, const AgentFactory* factory, const unsigned int /*version*/ )
{
    const MIL_IDManager* const idManager = &factory->idManager_;
    const MissionController_ABC* const missionController = &factory->missionController_;
    archive << idManager
            << missionController
            << factory->algorithmsFactories_;
}

template< typename Archive >
void load_construct_data( Archive& archive, AgentFactory* factory, const unsigned int /*version*/ )
{
    MIL_IDManager* idManager;
    MissionController_ABC* missionController;
    std::auto_ptr< AlgorithmsFactories > algorithmsFactories;
    archive >> idManager
            >> missionController
            >> algorithmsFactories;
    ::new( factory )AgentFactory( *idManager, *missionController, algorithmsFactories );
}

#endif // __AgentFactory_h_
