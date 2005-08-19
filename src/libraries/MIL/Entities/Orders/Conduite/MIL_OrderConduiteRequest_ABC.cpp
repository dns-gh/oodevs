//*****************************************************************************
//
// $Created: NLD 2003-01-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Conduite/MIL_OrderConduiteRequest_ABC.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 16:02 $
// $Revision: 2 $
// $Workfile: MIL_OrderConduiteRequest_ABC.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_OrderConduiteRequest_ABC.h"
#include "MIL_OrderConduiteType.h"
#include "Decision/DEC_Tools.h"


int MIL_OrderConduiteRequest_ABC::nDIATypeIdx_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC::InitializeDIA
// Created: AGN 03-01-26
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduiteRequest_ABC::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_OrdreConduiteDemande" );
    nDIATypeIdx_ = DEC_Tools::InitializeDIAField( "nType_" , diaType );
}


//-----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC constructor
// Created: NLD 2003-01-08
//-----------------------------------------------------------------------------
MIL_OrderConduiteRequest_ABC::MIL_OrderConduiteRequest_ABC( const MIL_OrderConduiteType& type )
    : DIA_Thing( DIA_Thing::ThingType(), *DIA_TypeManager::Instance().GetType( "T_OrdreConduiteDemande" ) )
    , type_    ( type )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC destructor
// Created: NLD 2003-01-08
//-----------------------------------------------------------------------------
MIL_OrderConduiteRequest_ABC::~MIL_OrderConduiteRequest_ABC()
{
    
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC::Initialize
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_OrderConduiteRequest_ABC::Initialize( DIA_Parameters& /*diaParams*/, uint& /*nCurrentDIAParameter*/ )
{
    GetVariable( nDIATypeIdx_ ).SetValue( (int)type_.GetConduiteID() );    
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC::Terminate
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_OrderConduiteRequest_ABC::Terminate()
{
}

//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC::Serialize
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void MIL_OrderConduiteRequest_ABC::Serialize( ASN1T_MsgAttenteOrdreConduite_ordres_conduite_element& asnMsg ) const
{
    asnMsg.t = type_.GetAsnRequestID();
}
