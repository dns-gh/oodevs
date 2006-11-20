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
#include "Entities/Orders/MIL_OrderContext.h"

class MIL_AgentPion;
class MIL_AutomateMission_ABC;
class MIL_PionMissionType;

//=============================================================================
// Created: NLD 2002-07-15
// Last modified: JVT 02-12-05
//=============================================================================
class MIL_PionMission_ABC : public DIA_Thing
                          , public MIL_OrderContext
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
    virtual void                     Initialize();
    //@}

    // @name DIA
    //@{
    static void InitializeDIA();
    const char* GetName      () const;
    //@}

    // @name Control
    //@{
    void Start();
    void Stop ();
    //@}

    // @name Network
    //@{
    virtual void Serialize              ( ASN1T_MsgPionOrder& asnMsg );
    virtual void CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg ); // Delete all the ASN members allocated for the serialization
    //@}

    //! @name Accessors
    //@{
    const MIL_PionMissionType& GetType() const;
          MIL_AgentPion&       GetPion() const;
    //@}

protected:
    MIL_AgentPion& pion_;

private:
    const MIL_PionMissionType& type_;
          bool                 bDIABehaviorActivated_;
    
private:
    static int nDIAMissionType_;
    static int nDIADirectionDangerIdx_;
};

#include "MIL_PionMission_ABC.inl"

#endif // __MIL_PionMission_ABC_h_
