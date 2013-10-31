#ifndef __StubDEC_KnowledgeResolver_ABC_h_
#define __StubDEC_KnowledgeResolver_ABC_h_

#include <tools/Exception.h>
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

    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const sword::Id&  /*asn*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const MIL_Agent_ABC& /*agent*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( unsigned int /*nID*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::Id& /*asn*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& /*object*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int /*nID*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int /*nID*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }

    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const sword::Id& /*asn*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const MIL_Population& /*population*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( unsigned int /*nID*/ ) const{ throw MASA_EXCEPTION_NOT_IMPLEMENTED; }
};

#endif // __StubDEC_KnowledgeResolver_ABC_h_
