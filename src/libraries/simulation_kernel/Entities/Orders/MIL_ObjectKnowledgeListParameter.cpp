// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectKnowledgeListParameter.h"
#include "simulation_orders/MIL_ParameterType_ObjectKnowledgeList.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeListParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeListParameter::MIL_ObjectKnowledgeListParameter( const Common::MsgObjectKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    knowledgeObjectList_.reserve( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject = resolver.ResolveKnowledgeObject( asn.elem(i) );
        if( !pKnowledgeObject || !pKnowledgeObject->IsValid() )
            throw std::runtime_error( "Object Knowledge does not exist" );
        knowledgeObjectList_.push_back( pKnowledgeObject );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeListParameter::MIL_ObjectKnowledgeListParameter( const std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& knowledgeObjectList )
    : knowledgeObjectList_( knowledgeObjectList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeListParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeListParameter::~MIL_ObjectKnowledgeListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeListParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_ObjectKnowledgeList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeListParameter::ToObjectKnowledgeList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeListParameter::ToObjectKnowledgeList( Common::MsgObjectKnowledgeList& asn ) const
{
    unsigned int size = knowledgeObjectList_.size();
    for( unsigned int i = 0; i < size; ++i )
        asn.add_elem()->set_oid( knowledgeObjectList_[i]->GetID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeListParameter::ToObjectKnowledgeList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeListParameter::ToObjectKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& value ) const
{
    value.resize( knowledgeObjectList_.size() );
    for( unsigned int i = 0; i < knowledgeObjectList_.size(); ++i )
        value[i] = knowledgeObjectList_[i];
    return true;
}
