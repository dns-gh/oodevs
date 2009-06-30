//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionSurfaceAgent_h_
#define __PHY_PerceptionSurfaceAgent_h_

#include "MIL.h"
#include "MT_Tools/MT_Sector.h"

class PHY_SensorTypeAgent;
class PHY_PerceptionLevel;
class PHY_RolePion_Perceiver;
class MIL_Agent_ABC;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class DEC_Knowledge_Agent;

//*****************************************************************************
// Created: DFT 02-02-26
// Last modified: JVT 03-09-01
//*****************************************************************************
class PHY_PerceptionSurfaceAgent
{
public:
     PHY_PerceptionSurfaceAgent();
     PHY_PerceptionSurfaceAgent( const PHY_SensorTypeAgent& sensorType, const MT_Vector2D& vOrigin, MT_Float rHeight );
    ~PHY_PerceptionSurfaceAgent();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception        ( const PHY_RolePion_Perceiver& perceiver, const MT_Vector2D&                 target ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const PHY_RolePion_Perceiver& perceiver, const MIL_Agent_ABC&               target ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const PHY_RolePion_Perceiver& perceiver, const DEC_Knowledge_Agent&         target ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const PHY_RolePion_Perceiver& perceiver, const MIL_PopulationConcentration& target ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const PHY_RolePion_Perceiver& perceiver, const MIL_PopulationFlow&          target, T_PointVector& shape ) const;
          MT_Float             ComputePerceptionAccuracy( const PHY_RolePion_Perceiver& perceiver, const MIL_PopulationFlow& target ) const;

          void                 AddDirection     ( const MT_Vector2D& vDir );
    //@}

    //! @name Accessors
    //@{
    bool IsInitialized();
    //@}

    //! @name Operators
    //@{
    PHY_PerceptionSurfaceAgent& operator= ( const PHY_PerceptionSurfaceAgent& rhs );
    //@}

    //! @name Network
    //@{
    void SendFullState( ASN1T_VisionCone& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Sector >       T_SectorVector;
    typedef T_SectorVector::const_iterator CIT_SectorVector;
    //@}

    //! @name Tools
    //@{
    bool IsInside( const MT_Vector2D& vPoint ) const;
    //@}

private:
    const PHY_SensorTypeAgent* pSensorType_;
          MT_Vector2D          vOrigin_;
          MT_Float             rHeight_;
          T_SectorVector       sectors_;
};

#endif // __PHY_PerceptionSurfaceAgent_h_
