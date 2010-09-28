#ifndef __StubDEC_KnowledgeResolver_ABC_h_
#define __StubDEC_KnowledgeResolver_ABC_h_

#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

namespace Common
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

    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const Common::UnitKnowledgeId&  /*asn*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( unsigned int /*nID*/ ) const{ throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::ObjectKnowledgeId& /*asn*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int /*nID*/ ) const{ throw; }

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::CrowdKnowledgeId& /*asn*/ ) const{ throw; }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( unsigned int /*nID*/ ) const{ throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const Common::UrbanObjectKnowledgeId& /*asn*/ ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( unsigned int /*nID*/ ) const{ throw; }
};

#endif // __StubDEC_KnowledgeResolver_ABC_h_
