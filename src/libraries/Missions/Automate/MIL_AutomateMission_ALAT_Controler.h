// *****************************************************************************
//
// $Created: 2005-4-27 - 17:37:45 $
// $Archive: /MVW_v10/Build/SDK/Missions/src/Automate/MIL_AutomateMission_ALAT_Controler.h $
// $Author: Nld $
// $Modtime: 27/04/05 17:37 $
// $Revision: 33 $
// $Workfile: MIL_AutomateMission_ALAT_Controler.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateMission_ALAT_Controler_h_
#define __MIL_AutomateMission_ALAT_Controler_h_

#include "Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "TER_Localisation.h"

//=============================================================================
// Created: 2005-4-27 - 17:37:45
//=============================================================================
class MIL_AutomateMission_ALAT_Controler : public MIL_AutomateMission_ABC
{
    MT_COPYNOTALLOWED( MIL_AutomateMission_ALAT_Controler )

public:
             MIL_AutomateMission_ALAT_Controler( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    virtual ~MIL_AutomateMission_ALAT_Controler();

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
    MT_Vector2D pointRegroupement_;

private:
    static int nDIAZoneIdx_;
    static int nDIAPointRegroupementIdx_;
    static int nDIAPlotsRavitaillementIdx_;
    static int nDIAPorteeActionIdx_;
    static int nDIAAmbianceMissionIdx_;

};


#endif // __MIL_AutomateMission_ALAT_Controler_h_
