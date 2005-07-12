// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadarDoppler.h $
// $Author: Nld $
// $Modtime: 25/03/05 11:45 $
// $Revision: 4 $
// $Workfile: PHY_PerceptionRadarDoppler.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRadarDoppler_h_
#define __PHY_PerceptionRadarDoppler_h_

#include "PHY_Perception_ABC.h"
#include "TER_Localisation.h"

class MIL_Agent_ABC;

// =============================================================================
// @class  PHY_PerceptionRadarDoppler
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRadarDoppler : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionRadarDoppler )

public:
             PHY_PerceptionRadarDoppler( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionRadarDoppler();

    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();
    //@}

    //! @name Add/Remove Points
    //@{
    void* AddRadar        ( const TER_Localisation& );
    void  RemoveRadar     ( void* );
    bool  HasRadarToHandle() const;
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
    //! @name Types
    //@{
    struct sRadar
    {
        sRadar( const TER_Localisation& localisation );

        void GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& ) const;

    private:
        sRadar& operator = ( const sRadar& );

    private:
        const TER_Localisation localisation_;
    };

    typedef std::vector< sRadar* >        T_RadarVector;
    typedef T_RadarVector::iterator       IT_RadarVector;
    typedef T_RadarVector::const_iterator CIT_RadarVector;

    struct sAgentPerceptionData
    {
        sAgentPerceptionData();
        sAgentPerceptionData( const sAgentPerceptionData& rhs );
        const uint nFirstTimeStepPerceived_;
              bool bUpdated_;

    private:
        sAgentPerceptionData& operator= ( const sAgentPerceptionData& rhs );
    };

    typedef std::map< MIL_Agent_ABC*, sAgentPerceptionData > T_AgentPerceptionMap;
    typedef T_AgentPerceptionMap::iterator                   IT_AgentPerceptionMap;
    typedef T_AgentPerceptionMap::const_iterator             CIT_AgentPerceptionMap;

    typedef std::vector< bool > T_DetectableConsumptionVector;
    //@}

private:
    //! @name Tools
    //@{
    const PHY_PerceptionLevel& ComputeLevel( const MIL_Agent_ABC& agent, uint nFirstTimeStepPerceived ) const;
    //@}

private:
    T_RadarVector        radars_;
    T_AgentPerceptionMap perceivedAgents_;

private:
    static MT_Float                      rRadius_;
    static MT_Float                      rDetectionTime_;
    static MT_Float                      rRecognitionTime_;
    static MT_Float                      rIdentificationTime_;
    static T_DetectableConsumptionVector detectableConsumptions_;
};

#include "PHY_PerceptionRadarDoppler.inl"

#endif // __PHY_PerceptionRadarDoppler_h_
