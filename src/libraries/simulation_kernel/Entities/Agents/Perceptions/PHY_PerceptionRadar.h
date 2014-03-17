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
    bool IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const;
    //@}

    //! @name Execution
    //@{
    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual void FinalizePerception();
    //@}

private:
    //! @name
    //@{
    typedef std::vector< PHY_PerceptionRadarData::T_ZoneSet > T_RadarZonesVector;
    typedef std::vector< bool >                               T_RadarOnUnitPositionVector;
    typedef std::map< const PHY_RadarType*, PHY_PerceptionRadarData > T_RadarDataMap;
    typedef std::map< int, const TER_Localisation* >          T_RadarIdMap;
    //@}

private:
    T_RadarZonesVector          radarZones_;
    T_RadarOnUnitPositionVector radarOnUnitPosition_;
    T_RadarDataMap              radarData_;
    T_RadarIdMap                radarId_;
};

#endif // __PHY_PerceptionRadar_h_
