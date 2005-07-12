// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRadarASA.h $
// $Author: Age $
// $Modtime: 4/03/05 14:54 $
// $Revision: 3 $
// $Workfile: PHY_PerceptionRadarASA.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionRadarASA_h_
#define __PHY_PerceptionRadarASA_h_

#include "MIL.h"

#include "PHY_Perception_ABC.h"

// =============================================================================
// @class  PHY_PerceptionRadarASA
// Created: JVT 2004-10-21
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_PerceptionRadarASA : public PHY_Perception_ABC
{
    MT_COPYNOTALLOWED( PHY_PerceptionRadarASA )

public:
    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();
    //@}

public:
             PHY_PerceptionRadarASA( PHY_RolePion_Perceiver& perceiver );
    virtual ~PHY_PerceptionRadarASA();

    //! @name Radars management
    //@{
    void EnableRadar     ( uint nType );
    void DisableRadar    ( uint nType );
    bool HasRadarToHandle() const;
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
    enum E_RadarASA
    {
        eHawk   = 0,
        eMistral,
        eRoland,
        eNbrRadarASA
    };

    struct sRadarType
    {
        sRadarType();
        sRadarType( MIL_InputArchive& archive );

        //! @name Operations
        //@{
        void Execute( PHY_RolePion_Perceiver& perceiver ) const;
        //@}

    private:
        //! @name Types
        //@{
        typedef std::vector< bool > T_DetectableConsumptionVector;
        //@}
        
    private:
        T_DetectableConsumptionVector detectableConsumptions_;
        MT_Float                      rRadius_;
        MT_Float                      rMinHeight_;
        MT_Float                      rMaxHeight_;
    };

    typedef std::vector< sRadarType >            T_RadarTypeVector;
    typedef std::vector< bool >                  T_RadarEnabledVector;
    typedef T_RadarEnabledVector::const_iterator CIT_RadarEnabledVector;
    //@}

private:
    T_RadarEnabledVector radarsEnabled_;

private:
    static T_RadarTypeVector radarTypes_;
};

#include "PHY_PerceptionRadarASA.inl"

#endif // __PHY_PerceptionRadarASA_h_
