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

#include "DEC_Knowledge_Def.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_KnowledgeResolver_ABC.h"
#include <boost/serialization/list.hpp>

class DEC_KnowledgeSource_ABC;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard_ABC
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard_ABC : public DEC_KnowledgeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KnowledgeBlackBoard_ABC();
    virtual ~DEC_KnowledgeBlackBoard_ABC();
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Main operations
    //@{
    virtual void Update( int currentTimeStep );
    void Clean();
    void Clear();
    virtual void CleanDeletedAgentKnowledges();
    //@}

    //! @name Knowledge sources management
    //@{
    void AddToScheduler( DEC_KnowledgeSource_ABC& ks );
    void RemoveFromScheduler( DEC_KnowledgeSource_ABC& ks );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( unsigned int nCtx ) const = 0;
    virtual void SendChangedState() const = 0;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< DEC_KnowledgeSource_ABC* > T_KnowledgeSourceList;
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeSourceList talkingKnowledgeSources_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_ABC::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KnowledgeBlackBoard_ABC::serialize( Archive& file, const unsigned int )
{
    file & talkingKnowledgeSources_;
}

#endif // __DEC_KnowledgeBlackBoard_ABC_h_
