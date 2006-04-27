// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_PopulationVictimeExplosionMines.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 18:41 $
// $Revision: 1 $
// $Workfile: MIL_RC_PopulationVictimeExplosionMines.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_PopulationVictimeExplosionMines.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_PopulationVictimeExplosionMines constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PopulationVictimeExplosionMines::MIL_RC_PopulationVictimeExplosionMines( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PopulationVictimeExplosionMines::~MIL_RC_PopulationVictimeExplosionMines()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_PopulationVictimeExplosionMines destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PopulationVictimeExplosionMines::Send( const MIL_AgentPion& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PopulationVictimeExplosionMines destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PopulationVictimeExplosionMines::Send( const MIL_Automate& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PopulationVictimeExplosionMines::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PopulationVictimeExplosionMines::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PopulationVictimeExplosionMines::Send
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
void MIL_RC_PopulationVictimeExplosionMines::Send( const MIL_Population& sender, E_RcType nType, uint nNbrVictim, uint nNbrWounded ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );

    ASN1T_CR_PopulationVictimeExplosionMines asnData;
    asnData.victimes     = nNbrVictim;
    asnData.dont_blesses = nNbrWounded;

    asnMsg.GetAsnMsg().cr.u.cr_population_victime_explosion_mines = &asnData;
    asnMsg.Send();           
}
