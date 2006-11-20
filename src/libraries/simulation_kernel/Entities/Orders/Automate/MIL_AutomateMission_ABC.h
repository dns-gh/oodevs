//*****************************************************************************
//
// $Created: NLD 2003-04-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateMission_ABC.h $
// $Author: Nld $
// $Modtime: 21/03/05 12:14 $
// $Revision: 6 $
// $Workfile: MIL_AutomateMission_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_AutomateMission_ABC_h_
#define __MIL_AutomateMission_ABC_h_

#include "MIL.h"

#include "MIL_AutomateMRT.h"
#include "Network/NET_ASN_Types.h"
#include "Entities/Orders/MIL_OrderContext.h"

class MIL_Automate;
class MIL_AutomateMissionType;

//=============================================================================
// Created: NLD 2003-04-09
//=============================================================================
class MIL_AutomateMission_ABC : public DIA_Thing
                              , public MIL_OrderContext
{
    MT_COPYNOTALLOWED( MIL_AutomateMission_ABC )

public:
    MIL_AutomateMission_ABC( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    virtual ~MIL_AutomateMission_ABC();

    //! @name Init
    //@{
    virtual void                     Initialize();
    virtual ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgAutomateOrder& asnMsg );
    //@}

    //! @name DIA
    //@{
    static void InitializeDIA();
    const char* GetName() const;
    //@}

    //! @name Control
    //@{
    void StartConduite();
    void StopConduite ();
    void StartMRT     ();
    void StopMRT      ();
    //@}

    //! @name Accessors
    //@{
          MIL_AutomateMRT&         GetMRT      ();
    const MIL_AutomateMissionType& GetType     () const;
          MIL_Automate&            GetAutomate () const;  
    //@}

    // @name Network
    //@{
    virtual void Serialize              ( ASN1T_MsgAutomateOrder& asnMsg );
    virtual void CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

protected:
    MIL_Automate& automate_;

private:
    const MIL_AutomateMissionType& type_;
          MIL_AutomateMRT          mrt_;

    bool bDIAMRTBehaviorActivated_;
    bool bDIAConduiteBehaviorActivated_;

public:
    static int nDIAMissionType_;
    static int nDIADirectionDangerIdx_;
    static int nDIAFormationIdx_;
};

#include "MIL_AutomateMission_ABC.inl"

#endif // __MIL_AutomateMission_ABC_h_
