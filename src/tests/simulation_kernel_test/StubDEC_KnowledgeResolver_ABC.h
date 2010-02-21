#ifndef __StubDEC_KnowledgeResolver_ABC_h_
#define __StubDEC_KnowledgeResolver_ABC_h_

#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
namespace Common
{
    class MsgUnitKnowledge;
    class MsgObjectKnowledge;
    class MsgPopulationKnowledge;
}

class StubDEC_KnowledgeResolver_ABC : public DEC_KnowledgeResolver_ABC
{
public:
    StubDEC_KnowledgeResolver_ABC() {}
    virtual ~StubDEC_KnowledgeResolver_ABC() {}

    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const Common::MsgUnitKnowledge&  asn ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const{ throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const Common::MsgObjectKnowledge& asn ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const{ throw; }

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const Common::MsgPopulationKnowledge& asn ) const{ throw; }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       unsigned int                       nID ) const{ throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban( const Common::MsgUrbanKnowledge& asn ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Urban > ResolveKnowledgeUrban(       uint                   nID ) const{ throw; }
};

#endif // __StubDEC_KnowledgeResolver_ABC_h_
