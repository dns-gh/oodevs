// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:35 $
// $Revision: 5 $
// $Workfile: AGR_MissionAutomate_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateMission_ASA_MISTRAL_DefendreSite_h_
#define __MIL_AutomateMission_ASA_MISTRAL_DefendreSite_h_

#include "MIL/Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "TER/TER_Localisation.h"

//=============================================================================
// Created: 2005-08-17 - 16:30:43
//=============================================================================
class MIL_AutomateMission_ASA_MISTRAL_DefendreSite : public MIL_AutomateMission_ABC
{
    MT_COPYNOTALLOWED( MIL_AutomateMission_ASA_MISTRAL_DefendreSite )

public:
             MIL_AutomateMission_ASA_MISTRAL_DefendreSite( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    virtual ~MIL_AutomateMission_ASA_MISTRAL_DefendreSite();

    //! @name Init
    //@{
    static void InitializeDIA( const MIL_AutomateMissionType& type );

    virtual ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgAutomateOrder& asnMsg );
    virtual void                     Terminate ();
    //@}

    //! @name Network 
    //@{
    /*final*/ void Serialize              ( ASN1T_MsgAutomateOrder& asnMsg );
    /*final*/ void CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

private:
    TER_Localisation zone_;

private:
    static int nDIAZoneIdx_;
    static int nDIAModeDeploiementIdx_;
    static int nDIAAngleIdx_;

};


#endif // __MIL_AutomateMission_ASA_MISTRAL_DefendreSite_h_
