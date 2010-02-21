// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeResolver_Empty.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty constructor
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_KnowledgeResolver_Empty::DEC_KnowledgeResolver_Empty()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty destructor
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_KnowledgeResolver_Empty::~DEC_KnowledgeResolver_Empty()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeResolver_Empty::ResolveKnowledgeAgent( const Common::MsgUnitKnowledge& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeResolver_Empty::ResolveKnowledgeAgent( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeResolver_Empty::ResolveKnowledgeObject( const Common::MsgObjectKnowledge& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeResolver_Empty::ResolveKnowledgeObject( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeResolver_Empty::ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& /*asn*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeResolver_Empty::ResolveKnowledgePopulation( unsigned int /*nID*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgeUrban
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_KnowledgeResolver_Empty::ResolveKnowledgeUrban( const Common::MsgUrbanKnowledge& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Urban >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeResolver_Empty::ResolveKnowledgeUrban
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_KnowledgeResolver_Empty::ResolveKnowledgeUrban( uint /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Urban >();
}
