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
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Orders/Lima/MIL_Lima.h"
#include "Entities/Orders/MIL_Order_Def.h"

class MIL_Automate;
class MIL_AutomateMissionType;

//=============================================================================
// Created: NLD 2003-04-09
//=============================================================================
class MIL_AutomateMission_ABC : public DIA_Thing
{
    MT_COPYNOTALLOWED( MIL_AutomateMission_ABC )

public:
    MIL_AutomateMission_ABC( MIL_Automate& automate, const MIL_AutomateMissionType& type );
    virtual ~MIL_AutomateMission_ABC();

    //! @name Init
    //@{
    virtual void                     Initialize();
    virtual ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgAutomateOrder& asnMsg );
    virtual void                     Terminate ();
    //@}

    //! @name DIA
    //@{
    static void InitializeDIA();
    const char* GetName() const;
    //@}

    //! @name Control
    //@{
    void Prepare      ();
    void StartConduite();
    void StopConduite ();
    void StartMRT     ();
    void StopMRT      ();
    //@}

    //! @name Accessors
    //@{
          MIL_AutomateMRT&         GetMRT      ();
          uint                     GetOrderID  () const;
    const MIL_AutomateMissionType& GetType     () const;
          MIL_Automate&            GetAutomate () const;  
    const MT_Vector2D&             GetDirDanger() const;
    const MIL_Fuseau&              GetFuseau   () const;
    const T_LimaFlagedPtrMap&      GetLimas    () const;
    //@}

    //! @name Limas flags
    //@{
    bool GetLimaFlag( const MIL_Lima& lima ) const;
    bool SetLimaFlag( const MIL_Lima& lima, bool bFlag );
    //@}

    // @name Network
    //@{
    virtual void Serialize              ( ASN1T_MsgAutomateOrder& asnMsg );
    virtual void CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

private:
    //! @name Init tools
    //@{
    const MIL_Lima*          GetLima          ( MIL_Lima::E_LimaFunctions nLimaFuncType ) const;
    ASN1T_EnumOrderErrorCode InitializeLimits ( const ASN1T_MsgAutomateOrder& asnMsg );
    ASN1T_EnumOrderErrorCode InitializeLimas  ( const ASN1T_MsgAutomateOrder& asnMsg );
    ASN1T_EnumOrderErrorCode InitializeMission( const ASN1T_MsgAutomateOrder& asnMsg );
    //@}

protected:
    MIL_Automate& automate_;

private:
    const MIL_AutomateMissionType& type_;
          MIL_AutomateMRT          mrt_;
          uint                     nOrderID_;
          bool                     bCreatedBySim_;

    // Mission parameters
    MT_Vector2D        vDirDanger_;
    T_LimaFlagedPtrMap limaMap_;   
    const MIL_Limit*   pLeftLimit_;
    const MIL_Limit*   pRightLimit_;
    MIL_Fuseau         fuseau_;

    bool bDIAMRTBehaviorActivated_;
    bool bDIAConduiteBehaviorActivated_;

public:
    static int nDIAMissionType_;
    static int nDIADirectionDangerIdx_;
    static int nDIAFormationIdx_;
};

#include "MIL_AutomateMission_ABC.inl"

#endif // __MIL_AutomateMission_ABC_h_
