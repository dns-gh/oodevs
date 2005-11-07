//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_PionMission_ABC.h $
// $Author: Nld $
// $Modtime: 21/03/05 12:14 $
// $Revision: 6 $
// $Workfile: MIL_PionMission_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_PionMission_ABC_h_
#define __MIL_PionMission_ABC_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Orders/Lima/MIL_Lima.h"
#include "Entities/Orders/MIL_Order_Def.h"

class MIL_PionOrderManager;
class MIL_AgentPion;
class MIL_AutomateMission_ABC;
class MIL_PionMissionType;

//=============================================================================
// Created: NLD 2002-07-15
// Last modified: JVT 02-12-05
//=============================================================================
class MIL_PionMission_ABC : public DIA_Thing
{
    MT_COPYNOTALLOWED( MIL_PionMission_ABC );

public:
    MIL_PionMission_ABC( MIL_AgentPion& pion, const MIL_PionMissionType& type );
    virtual ~MIL_PionMission_ABC();

    // @name Init
    //@{
    virtual ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgPionOrder& asnMsg );
    virtual bool                     Initialize( const MIL_AutomateMission_ABC& parentMission );
    virtual bool                     Initialize( MIL_PionMission_ABC& mission );
    virtual bool                     Initialize();
    virtual void                     Terminate ();
    //@}

    // @name DIA
    //@{
    static void InitializeDIA();
    const char* GetName      () const;
    //@}

    // @name Control
    //@{
    void Prepare();
    void Start  ();
    void Stop   ();
    //@}

    // @name Network
    //@{
    virtual void Serialize              ( ASN1T_MsgPionOrder& asnMsg );
    virtual void CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

    //! @name Accessors
    //@{
    const MIL_PionMissionType& GetType     () const;
          uint                 GetOrderID  () const;
          MIL_AgentPion&       GetPion     () const;
    const MT_Vector2D&         GetDirDanger() const;
    const MIL_Fuseau&          GetFuseau   () const;
    const T_LimaFlagedPtrMap&  GetLimas    () const;
    //@}

    //! @name Tools  (used by MIL_AutomateMRT) $$$ A changer
    //@{
    void SetFuseau( const MIL_Fuseau& fuseau );
    //@}

    //! @name Limas flags
    //@{
    bool GetLimaFlag( const MIL_Lima& lima ) const;
    bool SetLimaFlag( const MIL_Lima& lima, bool bFlag );
    //@}

private:
    // @name Init tools
    //@{
    const MIL_Lima*          GetLima           ( MIL_Lima::E_LimaFunctions nLimaFuncType ) const;
    ASN1T_EnumOrderErrorCode InitializeLimits  ( const ASN1T_MsgPionOrder& asnMsg );
    ASN1T_EnumOrderErrorCode InitializeLimas   ( const ASN1T_MsgPionOrder& asnMsg );
    ASN1T_EnumOrderErrorCode InitializeMission ( const ASN1T_MsgPionOrder& asnMsg );
    //@}   

protected:
    MIL_AgentPion& pion_;

private:
    const MIL_PionMissionType& type_;
    bool                       bDIABehaviorActivated_;
    uint                       nOrderID_;

    // Mission parameters
    MT_Vector2D        vDirDanger_;
    T_LimaFlagedPtrMap limaMap_;   
    const MIL_Limit*   pLeftLimit_;
    const MIL_Limit*   pRightLimit_;
    MIL_Fuseau         fuseau_;

private:
    static int nDIAMissionType_;
    static int nDIADirectionDangerIdx_;
};

#include "MIL_PionMission_ABC.inl"

#endif // __MIL_PionMission_ABC_h_
