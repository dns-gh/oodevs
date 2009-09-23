// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadar.h $
// $Author: Nld $
// $Modtime: 3/05/05 11:02 $
// $Revision: 5 $
// $Workfile: PHY_PerceptionRadar.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRadar_h_
#define __PHY_PerceptionRadar_h_

#include "PHY_Perception_ABC.h"
#include "PHY_PerceptionRadarData.h"

class PHY_RadarClass;
class TER_Localisation;

namespace detection
{
    class DetectionComputerFactory_ABC;
}
// =============================================================================
// @class  PHY_PerceptionRadar
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRadar : public PHY_Perception_ABC
{

public:
    explicit PHY_PerceptionRadar( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionRadar();

    //! @name Add/Remove Points
    //@{
    int  EnableRadar( const PHY_RadarClass& radarClass, const TER_Localisation& );
    void DisableRadar( const PHY_RadarClass& radarClass, int id );

    void EnableRadar ( const PHY_RadarClass& radarClass );
    void DisableRadar( const PHY_RadarClass& radarClass );
    //@}

    //! @name Tools
    //@{
    bool HasRadarToHandle  () const;
    bool IsUsingActiveRadar() const;
    //@}

    //! @name Execution
    //@{
    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents, const detection::DetectionComputerFactory_ABC& detectionComputer );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;
    //@}

private:
    //! @name 
    //@{
    typedef std::vector< PHY_PerceptionRadarData::T_ZoneSet >   T_RadarZonesVector;
    typedef T_RadarZonesVector::const_iterator                  CIT_RadarZonesVector;
    typedef std::vector< bool >                                 T_RadarOnUnitPositionVector;
    typedef T_RadarOnUnitPositionVector::const_iterator         CIT_RadarOnUnitPositionVector;

    typedef std::map< const PHY_RadarType*, PHY_PerceptionRadarData > T_RadarDataMap;
    typedef T_RadarDataMap::iterator                                  IT_RadarDataMap;
    
    typedef std::map< int, const TER_Localisation* > T_RadarIdMap;
    typedef T_RadarIdMap::iterator                   IT_RadarIdMap;
    //@}

private:
    T_RadarZonesVector          radarZones_;
    T_RadarOnUnitPositionVector radarOnUnitPosition_;
    T_RadarDataMap              radarData_;
    T_RadarIdMap                radarId_;
};

#endif // __PHY_PerceptionRadar_h_
