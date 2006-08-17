// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_MaterielRetourDeMaintenance.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 16:06 $
// $Revision: 1 $
// $Workfile: MIL_RC_MaterielRetourDeMaintenance.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_MaterielRetourDeMaintenance.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_MaterielRetourDeMaintenance constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_MaterielRetourDeMaintenance::MIL_RC_MaterielRetourDeMaintenance( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_MaterielRetourDeMaintenance::~MIL_RC_MaterielRetourDeMaintenance()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_MaterielRetourDeMaintenance destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_MaterielRetourDeMaintenance::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& /*diaParams*/ ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.Send();  
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_MaterielRetourDeMaintenance destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_MaterielRetourDeMaintenance::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& /*diaParams*/ ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.Send();
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_MaterielRetourDeMaintenance::Send
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
void MIL_RC_MaterielRetourDeMaintenance::Send( const MIL_AgentPion& sender, E_RcType nType, const PHY_ComposanteType_ABC& type ) const
{
   NET_ASN_MsgCR asnMsg;
   FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
   asnMsg.GetAsnMsg().cr.u.cr_materiel_retour_de_maintenance = type.GetMosID();
   asnMsg.Send();
}
