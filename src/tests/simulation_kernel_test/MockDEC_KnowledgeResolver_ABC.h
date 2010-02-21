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
#include "protocol/protocol.h"

// =============================================================================
/** @class  MockDEC_KnowledgeResolver_ABC
    @brief  MockDEC_KnowledgeResolver_ABC
*/
// Created: LDC 2009-06-09
// =============================================================================
class MockDEC_KnowledgeResolver_ABC : public DEC_KnowledgeResolver_ABC
                                    , public mockpp::ChainableMockObject
{

public:
             MockDEC_KnowledgeResolver_ABC()
                : mockpp::ChainableMockObject( "MockDEC_KnowledgeResolver_ABC", 0 )
                , ResolveKnowledgeAgent_mocker( "ResolveKnowledgeAgent", this )
                , ResolveKnowledgeObject_mocker( "ResolveKnowledgeObject", this )
                , ResolveKnowledgePopulation_mocker( "ResolveKnowledgePopulation", this )
                , ResolveKnowledgeUrban_mocker( "ResolveKnowledgeUrban", this )
             {}
    virtual ~MockDEC_KnowledgeResolver_ABC() {}
    
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const Common::MsgUnitKnowledge&  asn ) const
    { 
        return ResolveKnowledgeAgent_mocker.forward( &asn );
    }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const
    { throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::MsgObjectKnowledge& asn ) const
    { 
        return ResolveKnowledgeObject_mocker.forward( &asn );
    }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const
    { throw; }

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& asn ) const
    { 
        return ResolveKnowledgePopulation_mocker.forward( &asn );
    }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       unsigned int                       nID ) const
    { throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban ( const Common::MsgUrbanKnowledge&  asn ) const
    { 
        return ResolveKnowledgeUrban_mocker.forward( &asn );
    }
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban (       uint                   nID ) const
    { throw; }

    mockpp::ChainableMockMethod< boost::shared_ptr< DEC_Knowledge_Agent >, const Common::MsgUnitKnowledge* > ResolveKnowledgeAgent_mocker;
    mockpp::ChainableMockMethod< boost::shared_ptr< DEC_Knowledge_Object >, const Common::MsgObjectKnowledge* > ResolveKnowledgeObject_mocker;
    mockpp::ChainableMockMethod< DEC_Knowledge_Population*, const Common::MsgPopulationKnowledge* > ResolveKnowledgePopulation_mocker;
    mockpp::ChainableMockMethod< boost::shared_ptr< DEC_Knowledge_Urban >, const Common::MsgUrbanKnowledge* > ResolveKnowledgeUrban_mocker;
};

#endif // __MockDEC_KnowledgeResolver_ABC_h_
