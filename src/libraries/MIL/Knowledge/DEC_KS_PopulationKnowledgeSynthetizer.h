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

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_PopulationPerception;
class DEC_Knowledge_Population;

class MIL_KnowledgeGroup;
class MIL_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_PopulationKnowledgeSynthetizer : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_PopulationKnowledgeSynthetizer ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_PopulationKnowledgeSynthetizer( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup );
    virtual ~DEC_KS_PopulationKnowledgeSynthetizer();
    //@}
    
    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    virtual void Clean  ();
    //@}

private:
    //! @name Tools
    //@{
    void                      CleanKnowledgePopulation      ( DEC_Knowledge_Population& knowledge );
    void                      UpdateKnowledgesFromPerception( const DEC_Knowledge_PopulationPerception& perception );
    void                      UpdateKnowledgesFromCollision ( const DEC_Knowledge_PopulationCollision&  collision  );
    DEC_Knowledge_Population& GetKnowledgeToUpdate          ( MIL_Population& populationKnown ) const;
    //@}

private:
    //! @name Members
    //@{
    const MIL_KnowledgeGroup* pKnowledgeGroup_;
    //@}
};

#include "DEC_KS_PopulationKnowledgeSynthetizer.inl"

#endif // __DEC_KS_PopulationKnowledgeSynthetizer_h_
