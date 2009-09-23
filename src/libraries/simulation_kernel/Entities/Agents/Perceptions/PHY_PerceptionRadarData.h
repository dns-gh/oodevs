// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadarData.h $
// $Author: Nld $
// $Modtime: 2/05/05 17:42 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRadarData.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRadarData_h_
#define __PHY_PerceptionRadarData_h_

class PHY_RadarType;
class PHY_RoleInterface_Perceiver;
class TER_Localisation;
class MIL_Agent_ABC;

namespace detection
{
    class DetectionComputerFactory_ABC;
}

// =============================================================================
// @class  PHY_PerceptionRadarData
// Created: NLD 2005-05-02
// =============================================================================
class PHY_PerceptionRadarData
{
public:
    //! @name Types
    //@{
    typedef std::set< const TER_Localisation* > T_ZoneSet;
    typedef T_ZoneSet::const_iterator           CIT_ZoneSet;
    //@}

public:
     PHY_PerceptionRadarData();
     PHY_PerceptionRadarData( const PHY_RadarType& radarType );
    ~PHY_PerceptionRadarData();

    //! @name Operations
    //@{
    void Acquire( PHY_RoleInterface_Perceiver& perceiver, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition, const detection::DetectionComputerFactory_ABC& detectionComputerFactory );
    //@}

private:
    //! @name Tools
    //@{
    void AcquireTargets( PHY_RoleInterface_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& targets, const detection::DetectionComputerFactory_ABC& detectionComputerFactory );
    void Update        ( PHY_RoleInterface_Perceiver& perceiver );
    //@}

private:
    //! @name Types
    //@{
    struct sAcquisitionData
    {
        sAcquisitionData();
        sAcquisitionData( const sAcquisitionData& rhs );
        const uint nFirstTimeStepPerceived_;
               bool bUpdated_;
    private:
        sAcquisitionData& operator= ( const sAcquisitionData& rhs );
    };

    typedef std::map< MIL_Agent_ABC*, sAcquisitionData > T_AgentAcquisitionMap;
    typedef T_AgentAcquisitionMap::iterator              IT_AgentAcquisitionMap;    
    //@}

private:
    const PHY_RadarType*        pRadarType_;
          T_AgentAcquisitionMap acquisitionData_;
};

#endif // __PHY_PerceptionRadarData_h_
