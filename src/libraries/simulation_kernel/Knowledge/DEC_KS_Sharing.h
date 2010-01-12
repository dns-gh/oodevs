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

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Agent;
class MIL_Agent_ABC;
class MIL_KnowledgeGroup;

// =============================================================================
// @class  DEC_KS_Sharing
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_Sharing : public DEC_KnowledgeSource_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_Sharing( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard );
             DEC_KS_Sharing();
    virtual ~DEC_KS_Sharing();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Clean();
    virtual void Talk   ( int currentTimeStep );

    void ShareFromSource( const MIL_KnowledgeGroup& source, uint nShareTimeStep );
    void ShareFromSource( const MIL_KnowledgeGroup& source, uint nShareTimeStep, const MT_Vector2D& vSharedCircleCenter, MT_Float rSharedCircleRadius );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
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
    DEC_KnowledgeBlackBoard_KnowledgeGroup* pBlackBoard_;
    T_ShareSourceMMap                       shareSources_;
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KS_Sharing::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KS_Sharing::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_;
}

#endif // __DEC_KS_Sharing_h_
