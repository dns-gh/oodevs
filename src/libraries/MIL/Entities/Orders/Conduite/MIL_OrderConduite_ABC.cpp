 //*****************************************************************************
//
// $Created: NLD 2003-01-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Conduite/MIL_OrderConduite_ABC.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 4 $
// $Workfile: MIL_OrderConduite_ABC.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_OrderConduite_ABC.h"

#include "MIL_OrderConduiteType.h"
#include "MIL_OrderConduiteRequest_ABC.h"
#include "Decision/DEC_Tools.h"

int MIL_OrderConduite_ABC::nDIATypeIdx_ = 0;
int MIL_OrderConduite_ABC::nDIAHintIdx_ = 0;

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_Ennemi::InitializeDIA
// Created: AGN 03-01-26
//-----------------------------------------------------------------------------
// static
void MIL_OrderConduite_ABC::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "Rep_OrdreConduite" );
    nDIATypeIdx_ = DEC_Tools::InitializeDIAField( "eOrdre_", diaType );
    nDIAHintIdx_ = DEC_Tools::InitializeDIAField( "szHint_", diaType );
}

// =============================================================================
// CTOR / DTOR
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ABC constructor
// Created: NLD 2003-01-08
//-----------------------------------------------------------------------------
MIL_OrderConduite_ABC::MIL_OrderConduite_ABC( const MIL_KnowledgeGroup& knowledgeGroup, const MIL_OrderConduiteType& type )
    : DIA_Representation( "", *DIA_TypeManager::Instance().GetType( type.GetConduiteDIATypeName() ) )
    , knowledgeGroup_   ( knowledgeGroup )
    , type_             ( type )
{
    eventParameters_.SetOwnerShip( true );
    eventParameters_.AddParam( new DIA_Variable_Object( this ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ABC destructor
// Created: NLD 2003-01-08
//-----------------------------------------------------------------------------
MIL_OrderConduite_ABC::~MIL_OrderConduite_ABC()
{
    
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ABC::Initialize
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_OrderConduite_ABC::Initialize( const ASN1T_MsgOrderConduite_order_conduite& /*asnMsg*/ )
{
    GetVariable( nDIATypeIdx_ ).SetValue( (int)type_.GetConduiteID() ); 
    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ABC::Initialize
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_OrderConduite_ABC::Initialize( DIA_Parameters& /*diaParams*/, uint /*nCurDIAParamIdx*/ )
{
    GetVariable( nDIATypeIdx_ ).SetValue( (int)type_.GetConduiteID() );
}

//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ABC::Launch
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_OrderConduite_ABC::Launch( DIA_Engine& engine, const std::string& strHint ) //$$$ BOF
{
    GetVariable( nDIAHintIdx_ ).SetValue( strHint ); 

    // EVT
    eventParameters_.GetParameter( 0 ).SetValue( *this  ); 
    DIA_Variable_ABC* pResult = engine.ExecuteScriptFunction( "EVT_DEC_OrdreConduite", eventParameters_ );
    if( pResult ) 
        delete pResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduite_ABC::IsAnAnswerToRequest
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
bool MIL_OrderConduite_ABC::IsAnAnswerToRequest( const MIL_OrderConduiteRequest_ABC& request ) const
{
    return type_ == request.GetType();    
}
