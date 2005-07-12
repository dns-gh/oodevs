// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Sharing.h $
// $Author: Nld $
// $Modtime: 20/04/05 19:13 $
// $Revision: 3 $
// $Workfile: DEC_KS_Sharing.h $
//
// *****************************************************************************

#ifndef __DEC_KS_Sharing_h_
#define __DEC_KS_Sharing_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_Knowledge_Agent;
class MIL_Agent_ABC;
class MIL_KnowledgeGroup;

// =============================================================================
// @class  DEC_KS_Sharing
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_Sharing : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_Sharing ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_Sharing( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup );
    virtual ~DEC_KS_Sharing();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();

    void ShareFromSource( const MIL_KnowledgeGroup& source, uint nShareTimeStep );
    void ShareFromSource( const MIL_KnowledgeGroup& source, uint nShareTimeStep, const MT_Vector2D& vSharedCircleCenter, MT_Float rSharedCircleRadius );
    //@}

public:
    //! @name Types
    //@{
    struct sShareSource
    {
        sShareSource();
        sShareSource( const MIL_KnowledgeGroup& shareSource );
        sShareSource( const MIL_KnowledgeGroup& shareSource, const MT_Vector2D& vSharedCircleCenter, MT_Float rSharedCircleRadius );

        const MIL_KnowledgeGroup* pShareSource_;
              MT_Vector2D         vSharedCircleCenter_;
              MT_Float            rSharedCircleRadius_;
    };

    typedef std::multimap< uint /*nShareTimeStep*/, sShareSource > T_ShareSourceMMap;
    typedef T_ShareSourceMMap::iterator                            IT_ShareSourceMMap;
    //@}
    
private:
    const MIL_KnowledgeGroup* pKnowledgeGroup_;
          T_ShareSourceMMap   shareSources_;
};

#include "DEC_KS_Sharing.inl"

#endif // __DEC_KS_Sharing_h_
