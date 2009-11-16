// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanBlockParameter.h"
#include "MIL_AgentServer.h"
#include "simulation_orders/MIL_ParameterType_UrbanBlock.h"
#include "Urban/UrbanModel.h"
#include <urban/Model.h>
#include <urban/BlockModel.h>
#include <urban/Block.h>
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter( const ASN1T_UrbanBlock& asn, MIL_AgentServer& agentServer )
: pUrbanBlock_( 0 )
{
    urban::Block* pBlock = agentServer.GetUrbanModel().FindUrbanBlock( asn );
    if( pBlock )
        pUrbanBlock_ = pBlock; 
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter destructor
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::~MIL_UrbanBlockParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::IsOfType
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast< const MIL_ParameterType_UrbanBlock* >( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::ToUrbanBlock
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::ToUrbanBlock( ASN1T_UrbanBlock& asn ) const
{
    if( pUrbanBlock_ )
    {
        asn = pUrbanBlock_->GetId();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::ToUrbanBlock
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::ToUrbanBlock( urban::Block*& value ) const
{
    if( pUrbanBlock_ )
    {
        value = pUrbanBlock_;
        return true;
    }
    return false;
}