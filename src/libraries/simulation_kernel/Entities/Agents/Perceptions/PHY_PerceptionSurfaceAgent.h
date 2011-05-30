//*****************************************************************************
// Created: DFT 02-02-26
//*****************************************************************************

#ifndef __PHY_PerceptionSurfaceAgent_h_
#define __PHY_PerceptionSurfaceAgent_h_

#include "MIL.h"
#include "MT_Tools/MT_Sector.h"
#include "MT_Tools/MT_Vector2DTypes.h"

namespace sword
{
    class VisionCone;
}

class PHY_SensorTypeAgent;
class PHY_PerceptionLevel;
class PHY_RoleInterface_Perceiver;
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
    //! @name Constructors/Destructor
    //@{
             PHY_PerceptionSurfaceAgent();
             PHY_PerceptionSurfaceAgent( const PHY_SensorTypeAgent& sensorType, const MT_Vector2D& vOrigin, double rHeight );
    virtual ~PHY_PerceptionSurfaceAgent();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MT_Vector2D& target ) const;
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_Agent_ABC& target ) const;
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const DEC_Knowledge_Agent& target ) const;
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_PopulationConcentration& target ) const;
    const PHY_PerceptionLevel& ComputePerception( const PHY_RoleInterface_Perceiver& perceiver, const MIL_PopulationFlow& target, T_PointVector& shape ) const;
    double ComputePerceptionAccuracy( const PHY_RoleInterface_Perceiver& perceiver, const MIL_PopulationFlow& target ) const;
    void AddDirection( const MT_Vector2D& vDir );
    void TransfertPerception( std::map< const void*, std::pair< unsigned int, double > > urbanPerceptionMap ) const;

    virtual void FinalizePerception();
    //@}

    //! @name Accessors
    //@{
    bool IsInitialized();
    const std::string& GetSensorTypeName() const; // LTO
    std::map< const void*, unsigned int > GetTargetsPerception() const;
    //@}

    //! @name Operators
    //@{
    PHY_PerceptionSurfaceAgent& operator= ( const PHY_PerceptionSurfaceAgent& rhs );
    //@}

    //! @name Network
    //@{
    void SendFullState( sword::VisionCone& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Sector >         T_SectorVector;
    typedef T_SectorVector::const_iterator CIT_SectorVector;

    typedef std::map< const void*, unsigned int > T_PerceptionTickMap;
    typedef T_PerceptionTickMap::const_iterator CIT_PerceptionTickMap;
    //@}

    //! @name Tools
    //@{
    bool IsInside( const MT_Vector2D& vPoint ) const;
    const PHY_PerceptionLevel& GetLevelWithDelay( const PHY_PerceptionLevel& level, const void* target ) const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_SensorTypeAgent* pSensorType_;
    MT_Vector2D vOrigin_;
    double rHeight_;
    T_SectorVector sectors_;
    T_PerceptionTickMap perceptionsUnderway_;
    mutable T_PerceptionTickMap perceptionsBuffer_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_PerceptionSurfaceAgent )

#endif // __PHY_PerceptionSurfaceAgent_h_
