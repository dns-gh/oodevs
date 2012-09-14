// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_SURFACE_AGENT_H
#define SWORD_PERCEPTION_PERCEPTION_SURFACE_AGENT_H

#include "MT_Tools/MT_Sector.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include <map>

namespace sword
{
    class VisionCone;
}

class MIL_PopulationConcentration;
class MIL_PopulationFlow;

namespace sword
{
namespace wrapper
{
    class View;
    class Effect;
}
namespace perception
{
    class PerceptionLevel;
    class SensorTypeAgent;
    class TargetPerceptionVisitor_ABC;

// =============================================================================
/** @class  PerceptionSurfaceAgent
    @brief  Perception surface agent
*/
// Created: SLI 2012-04-12
// =============================================================================
class PerceptionSurfaceAgent
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionSurfaceAgent();
             PerceptionSurfaceAgent( const SensorTypeAgent& sensorType, const MT_Vector2D& vOrigin, double rHeight );
    virtual ~PerceptionSurfaceAgent();
    //@}

    //! @name Operations
    //@{
    void Apply( TargetPerceptionVisitor_ABC& visitor ) const;
    const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const MT_Vector2D& target ) const;
    const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const wrapper::View& target ) const;
    const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const MIL_PopulationConcentration& target ) const;
    const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const MIL_PopulationFlow& target, T_PointVector& shape ) const;
    double ComputePerceptionAccuracy( const wrapper::View& perceiver, const MIL_PopulationFlow& target ) const;
    void AddDirection( const MT_Vector2D& vDir );
    void TransferPerception( std::map< std::size_t, std::pair< unsigned int, double > > urbanPerceptionMap ) const;
    void NotifyCone( wrapper::Effect& effect ) const;

    void FinalizePerception();
    //@}

    //! @name Accessors
    //@{
    bool IsInitialized();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Sector >         T_SectorVector;
    typedef T_SectorVector::const_iterator CIT_SectorVector;

    typedef std::map< std::size_t, unsigned int > T_PerceptionTickMap;
    typedef T_PerceptionTickMap::const_iterator CIT_PerceptionTickMap;
    //@}

    //! @name Tools
    //@{
    bool IsInside( const MT_Vector2D& vPoint ) const;
    const PerceptionLevel& GetLevelWithDelay( const PerceptionLevel& level, std::size_t target ) const;
    //@}

private:
    //! @name Member data
    //@{
    const SensorTypeAgent* pSensorType_;
    MT_Vector2D vOrigin_;
    double rHeight_;
    T_SectorVector sectors_;
    T_PerceptionTickMap perceptionsUnderway_;
    mutable T_PerceptionTickMap perceptionsBuffer_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_SURFACE_AGENT_H
