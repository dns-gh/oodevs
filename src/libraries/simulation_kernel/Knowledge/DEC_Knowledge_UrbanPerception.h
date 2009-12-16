// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Knowledge_UrbanPerception_h_
#define __DEC_Knowledge_UrbanPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "game_asn/Simulation.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "urban/TerrainObject_ABC.h"

class MIL_AgentPion;

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  DEC_Knowledge_UrbanPerception
    @brief  DEC_Knowledge_UrbanPerception
*/
// Created: MGD 2009-11-26
// =============================================================================
class DEC_Knowledge_UrbanPerception : public DEC_Knowledge_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_UrbanPerception( const MIL_AgentPion& agentPerceiving, const urban::TerrainObject_ABC& object );
    virtual ~DEC_Knowledge_UrbanPerception();
    //@}
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update     ( const PHY_PerceptionLevel& perceptionLevel );
    void SetPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel );
    bool Clean      () const;
    //@}

    //! @name Accessor
    unsigned GetId  () const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
    const urban::TerrainObject_ABC& GetUrbanPerceived() const;
    const MIL_AgentPion& GetPerceiver() const;
    bool IsPerceived() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     ();
    void SendStateToNewClient();
    //@}

private:
    //! @name CheckPoints
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_Knowledge_UrbanPerception* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_Knowledge_UrbanPerception* role, const unsigned int /*version*/ );
    //@}

private:
    const unsigned       nID_;
    const MIL_AgentPion& perceiver_;
    const urban::TerrainObject_ABC& object_;

    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
};

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::save_construct_data
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
template< typename Archive >
inline void save_construct_data( Archive& archive, const DEC_Knowledge_UrbanPerception* perception, const unsigned int /*version*/ )
{
    const MIL_AgentPion* const perceiver = &perception->perceiver_;
    const urban::TerrainObject_ABC* const object = &perception->object_;
    archive << perceiver
            << object;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::load_construct_data
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
template< typename Archive >
inline void load_construct_data( Archive& archive, DEC_Knowledge_UrbanPerception* perception, const unsigned int /*version*/ )
{
    MIL_AgentPion* perceiver;
    urban::TerrainObject_ABC* object;
    archive >> perceiver
            >> object;
    ::new( perception )DEC_Knowledge_UrbanPerception( *perceiver, *object );
}


#endif // __DEC_Knowledge_UrbanPerception_h_
