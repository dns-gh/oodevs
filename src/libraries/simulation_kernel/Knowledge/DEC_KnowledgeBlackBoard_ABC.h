// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_ABC.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:47 $
// $Revision: 3 $
// $Workfile: DEC_KnowledgeBlackBoard_ABC.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeBlackBoard_ABC_h_
#define __DEC_KnowledgeBlackBoard_ABC_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_KnowledgeSource_ABC;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard_ABC
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard_ABC 
{
    MT_COPYNOTALLOWED( DEC_KnowledgeBlackBoard_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             DEC_KnowledgeBlackBoard_ABC();
    virtual ~DEC_KnowledgeBlackBoard_ABC();
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Main operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Knowledge sources management
    //@{
    void AddToScheduler     ( DEC_KnowledgeSource_ABC& ks );
    void RemoveFromScheduler( DEC_KnowledgeSource_ABC& ks );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   () const = 0;
    virtual void SendChangedState() const = 0;
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

#include "DEC_KnowledgeBlackBoard_ABC.inl"

#endif // __DEC_KnowledgeBlackBoard_ABC_h_
