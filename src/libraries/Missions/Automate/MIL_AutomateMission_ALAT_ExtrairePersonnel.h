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

#ifndef __MIL_AutomateMission_ALAT_ExtrairePersonnel_h_
#define __MIL_AutomateMission_ALAT_ExtrairePersonnel_h_

#include "MIL/Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "TER/TER_Localisation.h"

//=============================================================================
// Created: 2005-08-17 - 16:30:43
//=============================================================================
class MIL_AutomateMission_ALAT_ExtrairePersonnel : public MIL_AutomateMission_ABC
{
    MT_COPYNOTALLOWED( MIL_AutomateMission_ALAT_ExtrairePersonnel )

public:
             MIL_AutomateMission_ALAT_ExtrairePersonnel( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    virtual ~MIL_AutomateMission_ALAT_ExtrairePersonnel();

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
    MT_Vector2D pointDebarquement_;
    MT_Vector2D pointEmbarquement_;
    MT_Vector2D pointRegroupement_;

private:
    static int nDIAUnitesAHelitransporterIdx_;
    static int nDIAPointDebarquementIdx_;
    static int nDIAPointEmbarquementIdx_;
    static int nDIAPointRegroupementIdx_;
    static int nDIAPlotsRavitaillementIdx_;
    static int nDIARavitaillementDebutMissionIdx_;
    static int nDIAPorteeActionIdx_;
    static int nDIAAmbianceMissionIdx_;
    static int nDIAAvecMaterielIdx_;

};


#endif // __MIL_AutomateMission_ALAT_ExtrairePersonnel_h_
