// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_Effect_Fire_ABC.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 3 $
// $Workfile: MIL_Effect_Fire_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_Fire_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"

MIL_IDManager MIL_Effect_Fire_ABC::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Fire_ABC constructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Fire_ABC::MIL_Effect_Fire_ABC( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory )
    : nID_         ( idManager_.GetId() )
    , surface_     ( surface )
    , ammoCategory_( ammoCategory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Fire_ABC destructor
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
MIL_Effect_Fire_ABC::~MIL_Effect_Fire_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Fire_ABC::GetFireEffectId
// Created: JSR 2013-04-24
// -----------------------------------------------------------------------------
unsigned int MIL_Effect_Fire_ABC::GetFireEffectId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_Fire_ABC::GetSurface
// Created: JSR 2014-04-04
// -----------------------------------------------------------------------------
const MT_Ellipse& MIL_Effect_Fire_ABC::GetSurface() const
{
    return surface_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Effect_Fire_ABC::SendMsgStartEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MIL_Effect_Fire_ABC::SendMsgStartEffect( sword::StartFireEffect::EnumFireEffectType fireEffectType ) const
{
    client::StartFireEffect asnMsg;

    asnMsg().mutable_fire_effect()->set_id( nID_ );
    asnMsg().set_type( fireEffectType );
    NET_ASN_Tools::WriteEllipse( surface_, *asnMsg().mutable_location() );

    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

//-----------------------------------------------------------------------------
// Name: MIL_Effect_Fire_ABC::SendMsgStopEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MIL_Effect_Fire_ABC::SendMsgStopEffect() const
{
    client::StopFireEffect asnMsg;
    asnMsg().mutable_fire_effect()->set_id( nID_ );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}
