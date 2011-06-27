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

#include "AgentFactory_ABC.h"
#include "tools/Resolver.h"

class MIL_IDManager;
class AlgorithmsFactories;
class MissionController_ABC;

// =============================================================================
/** @class  AgentFactory
    @brief  AgentFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class AgentFactory : public AgentFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory( MIL_IDManager& idManager, MissionController_ABC& missionController, unsigned int gcPause, unsigned int gcMult );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition );
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name );
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentFactory( const AgentFactory& );            //!< Copy constructor
    AgentFactory& operator=( const AgentFactory& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void Initialize( MIL_AgentPion& pion, const MT_Vector2D& vPosition );
    //@}

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
    unsigned int gcPause_;
    unsigned int gcMult_;
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
            << factory->gcPause_
            << factory->gcMult_;
}
template< typename Archive >
void load_construct_data( Archive& archive, AgentFactory* factory, const unsigned int /*version*/ )
{
    MIL_IDManager* idManager;
    MissionController_ABC* missionController;
    unsigned int gcPause;
    unsigned int gcMult;
    archive >> idManager
            >> missionController
            >> gcPause
            >> gcMult;
    ::new( factory )AgentFactory( *idManager, *missionController, gcPause, gcMult );
}

#endif // __AgentFactory_h_
