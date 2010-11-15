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
#include "protocol/protocol.h"
#include "simulation_orders/MIL_ParameterType_UrbanBlock.h"
#include "simulation_orders/MIL_ParameterType_LocationComposite.h"
#include "simulation_kernel/knowledge/DEC_KnowledgeResolver_ABC.h"
#include "simulation_kernel/knowledge/DEC_Knowledge_Urban.h"
#include "UrbanModel.h"
#include <urban/Model.h>

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter( const Common::UrbanObjectId& asn, const DEC_KnowledgeResolver_ABC& resolver )
: pKnowledgeUrbanBlock_( resolver.ResolveKnowledgeUrban( asn.id() ) )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: MGD 2010-01-15
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter( boost::shared_ptr< DEC_Knowledge_Urban > urbanBlock )
: pKnowledgeUrbanBlock_( urbanBlock )
{
    // NOTHING
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
    return dynamic_cast< const MIL_ParameterType_UrbanBlock* >( &type ) != 0
        || dynamic_cast< const MIL_ParameterType_LocationComposite* >( &type ) != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::ToUrbanBlock
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::ToUrbanBlock( Common::UrbanObjectId& asn ) const
{
    asn.set_id( pKnowledgeUrbanBlock_->GetId() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::ToUrbanBlock
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::ToUrbanBlock( boost::shared_ptr< DEC_Knowledge_Urban >& value ) const
{
    value = pKnowledgeUrbanBlock_;
    return true;
}
