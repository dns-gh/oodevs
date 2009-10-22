#ifndef __StubDEC_KnowledgeResolver_ABC_h_
#define __StubDEC_KnowledgeResolver_ABC_h_

#include "Knowledge/DEC_KnowledgeResolver_ABC.h"

class StubDEC_KnowledgeResolver_ABC : public DEC_KnowledgeResolver_ABC
{
public:
    StubDEC_KnowledgeResolver_ABC() {}

    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const ASN1T_UnitKnowledge&  asn ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const DIA_Variable_ABC&      dia ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const{ throw; }

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const DIA_Variable_ABC&      dia ) const{ throw; }
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const{ throw; }

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const{ throw; }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const DIA_Variable_ABC&          dia ) const{ throw; }
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       uint                       nID ) const{ throw; }
};

#endif // __StubDEC_KnowledgeResolver_ABC_h_
