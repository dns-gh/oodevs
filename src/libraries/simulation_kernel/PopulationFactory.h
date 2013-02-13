// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PopulationFactory_h_
#define __PopulationFactory_h_

#include "PopulationFactory_ABC.h"
#include "MIL_AgentServer.h"

class MissionController_ABC;

// =============================================================================
/** @class  PopulationFactory
    @brief  PopulationFactory
*/
// Created: MGD 2009-10-24
// =============================================================================
class PopulationFactory : public PopulationFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFactory( MissionController_ABC& missionController, unsigned int gcPause, unsigned int gcMult, const MIL_Config& config );
    virtual ~PopulationFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Population& Create( xml::xistream& xis, MIL_Army_ABC& army );
    virtual MIL_Population& Create( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& army, MIL_UrbanObject_ABC* pUrbanObject, unsigned int context );
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
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PopulationFactory* factory, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PopulationFactory* factory, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    MissionController_ABC& missionController_;
    const MIL_Config& config_;
    const unsigned int gcPause_;
    const unsigned int gcMult_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PopulationFactory )

template< typename Archive >
void save_construct_data( Archive& archive, const PopulationFactory* factory, const unsigned int /*version*/ )
{
    const MissionController_ABC* const missionController = &factory->missionController_;
    archive << missionController
            << factory->gcPause_
            << factory->gcMult_;
}
template< typename Archive >
void load_construct_data( Archive& archive, PopulationFactory* factory, const unsigned int /*version*/ )
{
    MissionController_ABC* missionController;
    unsigned int gcPause;
    unsigned int gcMult;
    archive >> missionController
            >> gcPause
            >> gcMult;
    ::new( factory )PopulationFactory( *missionController, gcPause, gcMult, MIL_AgentServer::GetWorkspace().GetConfig() );
}

#endif // __PopulationFactory_h_
