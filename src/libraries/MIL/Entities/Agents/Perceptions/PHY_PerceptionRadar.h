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

class TER_Localisation;

// =============================================================================
// @class  PHY_PerceptionRadar
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRadar : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionRadar )

public:
             PHY_PerceptionRadar( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionRadar();

    //! @name Add/Remove Points
    //@{
    void* EnableRadar ( const PHY_RadarClass& radarClass, const TER_Localisation& );
    void  DisableRadar( const PHY_RadarClass& radarClass, void* pID );

    void  EnableRadar ( const PHY_RadarClass& radarClass );
    void  DisableRadar( const PHY_RadarClass& radarClass );
    //@}

    //! @name Tools
    //@{
    bool  HasRadarToHandle  () const;
    bool  IsUsingActiveRadar() const;
    //@}

    //! @name Execution
    //@{
    virtual const PHY_PerceptionLevel& Compute( const MT_Vector2D& vPoint ) const;

    virtual void                       Execute( const TER_Agent_ABC::T_AgentPtrVector& perceivableAgents );
    virtual const PHY_PerceptionLevel& Compute( const MIL_Agent_ABC& agent ) const;
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Agent & knowledge ) const;

    virtual void                       Execute( const TER_DynaObject_ABC::T_DynaObjectVector& perceivableObjects );
    virtual const PHY_PerceptionLevel& Compute( const DEC_Knowledge_Object& knowledge ) const;
    virtual const PHY_PerceptionLevel& Compute( const MIL_RealObject_ABC&   object    ) const;
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
    //@}

private:
    T_RadarZonesVector          radarZones_;
    T_RadarOnUnitPositionVector radarOnUnitPosition_;
    T_RadarDataMap              radarData_;
};

#include "PHY_PerceptionRadar.inl"

#endif // __PHY_PerceptionRadar_h_
