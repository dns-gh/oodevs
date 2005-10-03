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

#ifndef __MIL_AutomateMission_INF_Semparer_h_
#define __MIL_AutomateMission_INF_Semparer_h_

#include "MIL/Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "TER/TER_Localisation.h"

//=============================================================================
// Created: 
//=============================================================================
class MIL_AutomateMission_INF_Semparer : public MIL_AutomateMission_ABC
{
    MT_COPYNOTALLOWED( MIL_AutomateMission_INF_Semparer )

public:
             MIL_AutomateMission_INF_Semparer( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    virtual ~MIL_AutomateMission_INF_Semparer();

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
    TER_Localisation objectif_;

private:
    static int nDIAObjectifIdx_;

};


#endif // __MIL_AutomateMission_INF_Semparer_h_
