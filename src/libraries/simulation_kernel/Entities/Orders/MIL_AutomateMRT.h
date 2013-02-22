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

#include "tools/Map.h"

class MIL_AgentPion;
class MIL_Fuseau;
class MIL_Mission_ABC;

//=============================================================================
// Created: NLD 2003-04-14
//=============================================================================
class MIL_AutomateMRT : private boost::noncopyable
{
public:
             MIL_AutomateMRT();
    virtual ~MIL_AutomateMRT();

    //! @name Operations
    //@{
          void        SetMissionForPion( MIL_AgentPion& pion, const boost::shared_ptr< MIL_Mission_ABC > mission );
          void        SetFuseauForPion ( MIL_AgentPion& pion, MIL_Fuseau& fuseau );
    const MIL_Fuseau* GetFuseauForPion ( MIL_AgentPion& pion ) const;

    void Activate();
    void Cancel  ();

    bool IsActivated() const;
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Map< MIL_AgentPion*, boost::shared_ptr< MIL_Mission_ABC > > T_MissionPionMap;
    typedef std::map< MIL_AgentPion*, boost::shared_ptr< MIL_Fuseau > > T_FuseauPionMap;
    //@}

private:
    bool             bActivated_;
    T_MissionPionMap missionsPion_;
    T_FuseauPionMap  fuseauxPion_;
};

#endif // __MIL_AutomateMRT_h_
