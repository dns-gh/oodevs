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

#include "simulation_terrain/TER_Agent_ABC.h"
#include <tools/Map.h>
#include <tools/Set.h>

class PHY_RadarType;
class PHY_RoleInterface_Perceiver;
class TER_Localisation;
class MIL_Agent_ABC;

// =============================================================================
// @class  PHY_PerceptionRadarData
// Created: NLD 2005-05-02
// =============================================================================
class PHY_PerceptionRadarData
{
public:
    //! @name Types
    //@{
    typedef tools::Set< const TER_Localisation* > T_ZoneSet;
    //@}

public:
             PHY_PerceptionRadarData();
             PHY_PerceptionRadarData( const PHY_RadarType& radarType, const double sensorHeight );
    virtual ~PHY_PerceptionRadarData();

    //! @name Operations
    //@{
    void Acquire( PHY_RoleInterface_Perceiver& perceiver, const T_ZoneSet& zones, bool bAcquireOnPerceiverPosition );
    void SetMinHeight( double height );
    //@}

private:
    //! @name Tools
    //@{
    void AcquireTargets( PHY_RoleInterface_Perceiver& perceiver, TER_Agent_ABC::T_AgentPtrVector& targets );
    void Update        ( PHY_RoleInterface_Perceiver& perceiver );
    //@}

private:
    //! @name Types
    //@{
    struct sAcquisitionData
    {
        sAcquisitionData();
        unsigned int nFirstTimeStepPerceived_;
        bool bUpdated_;
    };
    //@}
    
private:
    const PHY_RadarType* pRadarType_;
    double sensorHeight_;
    tools::Map< MIL_Agent_ABC*, sAcquisitionData > acquisitionData_;
};

#endif // __PHY_PerceptionRadarData_h_
