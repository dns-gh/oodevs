#ifndef __StubDEC_KnowledgeResolver_ABC_h_
#define __StubDEC_KnowledgeResolver_ABC_h_

#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

namespace sword
{
    class UnitKnowledgeId;
    class ObjectKnowledgeId;
    class CrowdKnowledgeId;
}

class StubDEC_KnowledgeResolver_ABC : public DEC_KnowledgeResolver_ABC
{
public:
             StubDEC_KnowledgeResolver_ABC() {}
    virtual ~StubDEC_KnowledgeResolver_ABC() {}

    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const sword::UnitKnowledgeId&  /*asn*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const MIL_Agent_ABC& /*agent*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( unsigned int /*nID*/ ) const{ throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::ObjectKnowledgeId& /*asn*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& /*object*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int /*nID*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int /*nID*/ ) const{ throw; }

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& /*asn*/ ) const{ throw; }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const MIL_Population& /*population*/ ) const{ throw; }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( unsigned int /*nID*/ ) const{ throw; }
};

#endif // __StubDEC_KnowledgeResolver_ABC_h_
