// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.h $
// $Author: Nld $
// $Modtime: 11/03/05 16:35 $
// $Revision: 5 $
// $Workfile: AGR_MissionAutomate_Skeleton.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateMission_GEN_FaireFranchir_h_
#define __MIL_AutomateMission_GEN_FaireFranchir_h_

#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

//=============================================================================
// Created: 
//=============================================================================
class MIL_AutomateMission_GEN_FaireFranchir : public MIL_AutomateMission_ABC
{
    MT_COPYNOTALLOWED( MIL_AutomateMission_GEN_FaireFranchir )

public:
             MIL_AutomateMission_GEN_FaireFranchir( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    virtual ~MIL_AutomateMission_GEN_FaireFranchir();

    //! @name Init
    //@{
    static void InitializeDIA( const MIL_AutomateMissionType& type );

    virtual ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgAutomateOrder& asnMsg );
    //@}

    //! @name Network 
    //@{
    /*final*/ void Serialize              ( ASN1T_MsgAutomateOrder& asnMsg );
    /*final*/ void CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

private:
    static int nDIASitesDeFranchissementIdx_;
    static int nDIAZoneRegroupementIdx_;

};


#endif // __MIL_AutomateMission_GEN_FaireFranchir_h_
