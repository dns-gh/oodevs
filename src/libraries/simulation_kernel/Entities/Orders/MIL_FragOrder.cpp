// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_FragOrder.h"
#include "MIL_FragOrderType.h"
#include "MIL_OrderContext.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Decision_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::InitializeDIA
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
// static
void MIL_FragOrder::InitializeDIA()
{
    /*const DIA_TypeDef& diaType = */
    DEC_Tools::GetDIAType( "Rep_OrdreConduite" );
//    nDIATypeIdx_ = DEC_Tools::InitializeDIAField( "eOrdre_", diaType );
//    nDIAHintIdx_ = DEC_Tools::InitializeDIAField( "szHint_", diaType );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, DEC_Decision_ABC& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MsgFragOrder& asn )
    : DIA_TypedObject ( type.GetDIAType() )
    , type_           ( type )
    , diaEngine_      ( engine )
    , eventParameters_()
{
    eventParameters_.SetOwnerShip( true );
    eventParameters_.AddParam( new DIA_Variable_Object( this ) );
    MIL_OrderContext context;
    type_.Copy( asn.parametres, *this, knowledgeResolver, context );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_FragOrder::MIL_FragOrder( const MIL_FragOrderType& type, DEC_Decision_ABC& engine, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_FragOrder& rhs )
    : DIA_TypedObject ( type.GetDIAType() )
    , type_           ( type )
    , diaEngine_      ( engine )
    , eventParameters_()
{
    eventParameters_.SetOwnerShip( true );
    eventParameters_.AddParam( new DIA_Variable_Object( this ) );
    type_.Copy( rhs, *this, knowledgeResolver );
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_FragOrder::~MIL_FragOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrder::Launch
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
void MIL_FragOrder::Launch()
{
    eventParameters_.GetParameter( 0 ).SetValue( *this ); 
    DIA_Variable_ABC* pResult = diaEngine_.ExecuteFunction( "EVT_DEC_OrdreConduite", eventParameters_ ); //$$$$
    if( pResult ) 
        delete pResult;
}

