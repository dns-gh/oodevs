// *****************************************************************************

#ifndef __DEC_KS_KnowledgeSynthetizer_h_
#define __DEC_KS_KnowledgeSynthetizer_h_

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Population;
class DEC_Knowledge_Object;

// =============================================================================
/** @class  DEC_KS_KnowledgeSynthetizer
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used :
                - To synthetize the subordinate's perceptions of the agents
                  into knowledges for a GTIA
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_KnowledgeSynthetizer : public DEC_KnowledgeSource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_KnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard );
             DEC_KS_KnowledgeSynthetizer();
    virtual ~DEC_KS_KnowledgeSynthetizer();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk( int currentTimeStep );
    virtual void Clean();
    virtual void CleanDeletedAgentKnowledges();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Tools
    //@{
    void CleanKnowledgeAgent( DEC_Knowledge_Agent& knowledge );
    void CleanKnowledgeDeletedAgent( DEC_Knowledge_Agent& knowledge );
    void CleanKnowledgePopulation( DEC_Knowledge_Population& knowledge );
    void CleanKnowledgeObject( const boost::shared_ptr< DEC_Knowledge_Object >& knowledge );

    //@}

private:
    DEC_KnowledgeBlackBoard_KnowledgeGroup* pBlackBoard_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_KnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KS_AgentKnowledgeSynthetizer::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KS_KnowledgeSynthetizer::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this );
    archive & pBlackBoard_;
}

#endif // __DEC_KS_KnowledgeSynthetizer_h_
