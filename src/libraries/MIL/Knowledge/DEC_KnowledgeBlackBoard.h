// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:47 $
// $Revision: 3 $
// $Workfile: DEC_KnowledgeBlackBoard.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeBlackBoard_h_
#define __DEC_KnowledgeBlackBoard_h_

#include "MIL.h"

#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectCollision.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_BlackBoard_CanContainKnowledgeRapFor.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"

class DEC_KnowledgeSource_ABC;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard : public DEC_BlackBoard_CanContainKnowledgeAgentPerception
                              , public DEC_BlackBoard_CanContainKnowledgeAgent
                              , public DEC_BlackBoard_CanContainKnowledgeObjectCollision
                              , public DEC_BlackBoard_CanContainKnowledgeObjectPerception
                              , public DEC_BlackBoard_CanContainKnowledgeObject
                              , public DEC_BlackBoard_CanContainKnowledgeRapFor
                              , public DEC_BlackBoard_CanContainKnowledgePopulationPerception
                              , public DEC_BlackBoard_CanContainKnowledgePopulation
{
    MT_COPYNOTALLOWED( DEC_KnowledgeBlackBoard )

public:
    //! @name Constructors/Destructor
    //@{
              DEC_KnowledgeBlackBoard();
     virtual ~DEC_KnowledgeBlackBoard();
    //@}

    //! @name Initialization
    //@{
    void Initialize();
    void Terminate ();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Main operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Knowledge sources management
    //@{
    void AddToScheduler     ( DEC_KnowledgeSource_ABC& ks, bool bHasPriority = false );
    void RemoveFromScheduler( DEC_KnowledgeSource_ABC& ks );
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< DEC_KnowledgeSource_ABC* >     T_KnowledgeSourceList;
    typedef T_KnowledgeSourceList::iterator           IT_KnowledgeSourceList;
    typedef T_KnowledgeSourceList::const_iterator     CIT_KnowledgeSourceList;
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeSourceList talkingKnowledgeSources_;
    //@}
};

#include "DEC_KnowledgeBlackBoard.inl"

#endif // __DEC_KnowledgeBlackBoard_h_
