//*****************************************************************************
//
// $Created: NLD 2003-04-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Automate/MIL_AutomateMRT.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_AutomateMRT.h $
//
//*****************************************************************************

#ifndef __MIL_AutomateMRT_h_
#define __MIL_AutomateMRT_h_

#include "MIL.h"

class MIL_AgentPion;
class MIL_AutomateMission_ABC;
class MIL_Fuseau;
class MIL_PionMission_ABC;

//=============================================================================
// Created: NLD 2003-04-14
//=============================================================================
class MIL_AutomateMRT
{
    MT_COPYNOTALLOWED( MIL_AutomateMRT )
        
public:
     MIL_AutomateMRT();
    ~MIL_AutomateMRT();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize( MIL_AutomateMission_ABC& automateMission );
    void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Preparation */
    //-------------------------------------------------------------------------
    //@{
    bool              SetMissionForPion( MIL_AgentPion& pion, MIL_PionMission_ABC& mission );
    bool              SetFuseauForPion ( MIL_AgentPion& pion, MIL_Fuseau& fuseau );
    const MIL_Fuseau& GetFuseauForPion ( const MIL_AgentPion& pion ) const;

    void Validate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Activation */
    //-------------------------------------------------------------------------
    //@{
    void Activate   ();
    bool IsActivated() const;
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MIL_AgentPion*, MIL_PionMission_ABC* >  T_MissionPionMap;
    typedef T_MissionPionMap::iterator                         IT_MissionPionMap;
    typedef T_MissionPionMap::const_iterator                   CIT_MissionPionMap;
    
    typedef std::map< MIL_AgentPion*, MIL_Fuseau* > T_FuseauPionMap;
    typedef T_FuseauPionMap::iterator                IT_FuseauPionMap;
    typedef T_FuseauPionMap::const_iterator          CIT_FuseauPionMap;
    //@}
   
private:
    MIL_AutomateMission_ABC* pAutomateMission_;
    T_MissionPionMap         missionPionMap_;
    T_FuseauPionMap          fuseauPionMap_;
    bool                     bActivated_;
};

#   include "MIL_AutomateMRT.inl"

#endif // __MIL_AutomateMRT_h_
