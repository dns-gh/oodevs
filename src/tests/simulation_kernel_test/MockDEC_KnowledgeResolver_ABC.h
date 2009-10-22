// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockDEC_KnowledgeResolver_ABC_h_
#define __MockDEC_KnowledgeResolver_ABC_h_

#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

// =============================================================================
/** @class  MockDEC_KnowledgeResolver_ABC
    @brief  MockDEC_KnowledgeResolver_ABC
*/
// Created: LDC 2009-06-09
// =============================================================================
class MockDEC_KnowledgeResolver_ABC
    : public DEC_KnowledgeResolver_ABC
    , public mockpp::ChainableMockObject
{

public:
             MockDEC_KnowledgeResolver_ABC()
                : mockpp::ChainableMockObject( "MockDEC_KnowledgeResolver_ABC", 0 )
                , ResolveKnowledgeAgent_mocker( "ResolveKnowledgeAgent", this )
                , ResolveKnowledgeObject_mocker( "ResolveKnowledgeObject", this )
                , ResolveKnowledgePopulation_mocker( "ResolveKnowledgePopulation", this )
             {}
    virtual ~MockDEC_KnowledgeResolver_ABC() {}
    
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const ASN1T_UnitKnowledge&  asn ) const
    { 
        return ResolveKnowledgeAgent_mocker.forward( &asn );
    }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const DIA_Variable_ABC&      dia ) const
    { throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const
    { throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const
    { 
        return ResolveKnowledgeObject_mocker.forward( &asn );
    }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const DIA_Variable_ABC&      dia ) const
    { throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const
    { throw; }

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const
    { 
        return ResolveKnowledgePopulation_mocker.forward( &asn );
    }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const DIA_Variable_ABC&          dia ) const
    { throw; }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       uint                       nID ) const
    { throw; }

    mockpp::ChainableMockMethod< boost::shared_ptr< DEC_Knowledge_Agent >, const ASN1T_UnitKnowledge* > ResolveKnowledgeAgent_mocker;
    mockpp::ChainableMockMethod< boost::shared_ptr< DEC_Knowledge_Object >, const ASN1T_ObjectKnowledge* > ResolveKnowledgeObject_mocker;
    mockpp::ChainableMockMethod< DEC_Knowledge_Population*, const ASN1T_PopulationKnowledge* > ResolveKnowledgePopulation_mocker;
};

#endif // __MockDEC_KnowledgeResolver_ABC_h_
