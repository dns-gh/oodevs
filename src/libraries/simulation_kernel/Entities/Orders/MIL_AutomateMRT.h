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
class MIL_Fuseau;
class MIL_PionMission;

//=============================================================================
// Created: NLD 2003-04-14
//=============================================================================
class MIL_AutomateMRT
{
    MT_COPYNOTALLOWED( MIL_AutomateMRT )
        
public:
     MIL_AutomateMRT();
    ~MIL_AutomateMRT();

    //! @name Operations
    //@{
          void        SetMissionForPion( MIL_AgentPion& pion, MIL_PionMission& mission );
          void        SetFuseauForPion ( MIL_AgentPion& pion, MIL_Fuseau& fuseau );
    const MIL_Fuseau* GetFuseauForPion ( MIL_AgentPion& pion ) const;
    
    void Activate();
    void Cancel  ();

    bool IsActivated() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< MIL_AgentPion*, MIL_PionMission* >  T_MissionPionMap;
    typedef T_MissionPionMap::const_iterator               CIT_MissionPionMap;
    
    typedef std::map< MIL_AgentPion*, MIL_Fuseau* > T_FuseauPionMap;
    typedef T_FuseauPionMap::const_iterator         CIT_FuseauPionMap;
    //@}
   
private:
    bool             bActivated_;
    T_MissionPionMap missionsPion_;
    T_FuseauPionMap  fuseauxPion_;    
};

#   include "MIL_AutomateMRT.inl"

#endif // __MIL_AutomateMRT_h_
