// *****************************************************************************
//
// $Created: 2005-4-27 - 17:37:45 $
// $Archive: /MVW_v10/Build/SDK/Missions/src/Pion/MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition.h $
// $Author: Nld $
// $Modtime: 27/04/05 17:37 $
// $Revision: 16 $
// $Workfile: MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition.h $
//
// *****************************************************************************

#ifndef __MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition_h_
#define __MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition_h_

#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "TER_Localisation.h"

//*****************************************************************************
// Created: 2005-4-27 - 17:37:45
//*****************************************************************************
class MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition: public MIL_PionMission_ABC
{
    MT_COPYNOTALLOWED( MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition )

public:

             MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition( MIL_AgentPion& pion, const MIL_PionMissionType& type );
    virtual ~MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition();

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
    static int nDIAZoneIdx_;
    static int nDIAPointDeDeploiementIdx_;
    static int nDIAModeDeploiementIdx_;


private:
    TER_Localisation zone_;
    MT_Vector2D pointDeDeploiement_;

};


#endif // __MIL_PionMission_ASA_MISTRAL_SurveillerAPartirPosition_h_
