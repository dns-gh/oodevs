// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Fire.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: DEC_KS_Fire.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::NotifyHitByDirectFire
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
inline
void DEC_KS_Fire::NotifyAttackedBy( MIL_AgentPion& attacker )
{
    directFireAttackerSet_.insert( &attacker );
}

