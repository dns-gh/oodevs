// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:56 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:34 $
// $Revision: 5 $
// $Workfile: AGR_MissionPion_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_PionMission_GEN_RealiserPostesTir_h_
#define __MIL_PionMission_GEN_RealiserPostesTir_h_

#include "MIL/Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "TER/TER_Localisation.h"

//*****************************************************************************
// Created: 2005-08-19 - 11:30:56
//*****************************************************************************
class MIL_PionMission_GEN_RealiserPostesTir: public MIL_PionMission_ABC
{
    MT_COPYNOTALLOWED( MIL_PionMission_GEN_RealiserPostesTir )

public:

             MIL_PionMission_GEN_RealiserPostesTir( MIL_AgentPion& pion, const MIL_PionMissionType& type );
    virtual ~MIL_PionMission_GEN_RealiserPostesTir();

    //! @name Init
    //@{
    static void InitializeDIA( const MIL_PionMissionType& type );
    /*final*/ ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgPionOrder& asnMsg );
    /*final*/ bool                     Initialize( const MIL_AutomateMission_ABC& parentMission );
    /*final*/ bool                     Initialize( MIL_PionMission_ABC& mission );
    /*final*/ void                     Terminate();
    //@}

    //! @name Network 
    //@{
    /*final*/ void Serialize              ( ASN1T_MsgPionOrder& asnMsg );
    /*final*/ void CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

private:
    static int nDIAPosObjetIdx_;
    static int nDIAOidObjetPlanifieIdx_;


private:
    MT_Vector2D posObjet_;

};


#endif // __MIL_PionMission_GEN_RealiserPostesTir_h_
