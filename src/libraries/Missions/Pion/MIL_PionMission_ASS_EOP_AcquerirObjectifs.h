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

#ifndef __MIL_PionMission_ASS_EOP_AcquerirObjectifs_h_
#define __MIL_PionMission_ASS_EOP_AcquerirObjectifs_h_

#include "MIL/Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "TER/TER_Localisation.h"

//*****************************************************************************
// Created: 2005-08-19 - 11:30:56
//*****************************************************************************
class MIL_PionMission_ASS_EOP_AcquerirObjectifs: public MIL_PionMission_ABC
{
    MT_COPYNOTALLOWED( MIL_PionMission_ASS_EOP_AcquerirObjectifs )

public:

             MIL_PionMission_ASS_EOP_AcquerirObjectifs( MIL_AgentPion& pion, const MIL_PionMissionType& type );
    virtual ~MIL_PionMission_ASS_EOP_AcquerirObjectifs();

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
    static int nDIAZoneAObserverIdx_;
    static int nDIAPositionInstallationIdx_;
    static int nDIAPositionDebarquementIdx_;


private:
    TER_Localisation zoneAObserver_;
    MT_Vector2D positionInstallation_;
    MT_Vector2D positionDebarquement_;

};


#endif // __MIL_PionMission_ASS_EOP_AcquerirObjectifs_h_
