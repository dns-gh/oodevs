// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_PopulationKnowledgeSynthetizer.h $
// $Author: Jvt $
// $Modtime: 24/03/05 13:11 $
// $Revision: 5 $
// $Workfile: DEC_KS_PopulationKnowledgeSynthetizer.h $
//
// *****************************************************************************

#ifndef __DEC_KS_PopulationKnowledgeSynthetizer_h_
#define __DEC_KS_PopulationKnowledgeSynthetizer_h_

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_PopulationKnowledgeSynthetizer : public DEC_KnowledgeSource_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_PopulationKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard );
             DEC_KS_PopulationKnowledgeSynthetizer();
    virtual ~DEC_KS_PopulationKnowledgeSynthetizer();
    //@}
    
    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ( int currentTimeStep );
    virtual void Clean  ();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Tools
    //@{
    void                      CleanKnowledgePopulation      ( DEC_Knowledge_Population& knowledge );

    //@}

private:
    DEC_KnowledgeBlackBoard_KnowledgeGroup* pBlackBoard_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_PopulationKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KS_PopulationKnowledgeSynthetizer::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_;
}

#endif // __DEC_KS_PopulationKnowledgeSynthetizer_h_
