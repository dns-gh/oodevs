// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionTappingRadar.h $
// $Author: Nld $
// $Modtime: 25/03/05 11:44 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionTappingRadar.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionTappingRadar_h_
#define __PHY_PerceptionTappingRadar_h_

#include "PHY_Perception_ABC.h"
#include "TER_Localisation.h"

// =============================================================================
// @class  PHY_PerceptionTappingRadar
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionTappingRadar : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionTappingRadar )

public:
             PHY_PerceptionTappingRadar( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionTappingRadar();

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
    //@}

private:
    //! @name Tools
    //@{
    const PHY_PerceptionLevel& ComputeLevel( uint nFirstTimeStepPerceived ) const;
    //@}

private:
    T_RadarVector           radars_;
    T_AgentPerceptionMap    perceivedAgents_;

private:
    static MT_Float rRadius_;
    static MT_Float rDetectionTime_;
    static MT_Float rRecognitionTime_;
    static MT_Float rIdentificationTime_;
};

#include "PHY_PerceptionTappingRadar.inl"

#endif // __PHY_PerceptionTappingRadar_h_
