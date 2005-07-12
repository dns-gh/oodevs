// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionTapping.h $
// $Author: Nld $
// $Modtime: 25/03/05 11:45 $
// $Revision: 4 $
// $Workfile: PHY_PerceptionTapping.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionTapping_h_
#define __PHY_PerceptionTapping_h_

#include "PHY_Perception_ABC.h"
#include "TER_Localisation.h"

// =============================================================================
// @class  PHY_PerceptionTapping
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionTapping : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionTapping )

public:
             PHY_PerceptionTapping( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionTapping();

    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();
    //@}

    //! @name Add/Remove Points
    //@{
    void* AddTapping        ( const TER_Localisation& );
    void  RemoveTapping     ( void* );
    bool  HasTappingToHandle() const;
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
    struct sTapping
    {
        sTapping( const TER_Localisation& localisation );

        void GetAgentsInside( TER_Agent_ABC::T_AgentPtrVector& ) const;

    private:
        sTapping& operator = ( const sTapping& );

    private:
        const TER_Localisation localisation_;
    };

    typedef std::vector< sTapping* >        T_TappingVector;
    typedef T_TappingVector::iterator       IT_TappingVector;
    typedef T_TappingVector::const_iterator CIT_TappingVector;

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
    const PHY_PerceptionLevel& ComputeLevel( const MIL_Agent_ABC& agent, uint nFirstTimeStepPerceived ) const;
    //@}

private:
    T_TappingVector       tappings_;
    T_AgentPerceptionMap  perceivedAgents_;

private:
    static MT_Float rRadius_;

    static MT_Float rNonPcDetectionTime_;
    static MT_Float rNonPcRecognitionTime_;
    static MT_Float rNonPcIdentificationTime_;

    static MT_Float rPcDetectionTime_;
    static MT_Float rPcRecognitionTime_;
    static MT_Float rPcIdentificationTime_;
};

#include "PHY_PerceptionTapping.inl"

#endif // __PHY_PerceptionTapping_h_
