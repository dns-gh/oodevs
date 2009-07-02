// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeSource_ABC.h $
// $Author: Jvt $
// $Modtime: 31/03/05 16:25 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeSource_ABC.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeSource_ABC_h_
#define __DEC_KnowledgeSource_ABC_h_

#include "DEC_Knowledge_Def.h"

class DEC_KnowledgeBlackBoard_ABC;

// =============================================================================
/** @class  DEC_KnowledgeSource_ABC
    @brief  A knowledge source is able to 
                - publish or modify knowledges of the black board
                - access these knowledges
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeSource_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KnowledgeSource_ABC( DEC_KnowledgeBlackBoard_ABC& blackBoard, uint nPriority );
             DEC_KnowledgeSource_ABC();
    virtual ~DEC_KnowledgeSource_ABC();
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    virtual void Clean  ();
    //@}

    //! @name Accessors
    //@{
    uint GetPriority() const;
    //@}

private:
    uint                         nPriority_;
    DEC_KnowledgeBlackBoard_ABC* pBlackBoard_;
};

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeSource_ABC::serialize
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KnowledgeSource_ABC::serialize( Archive& archive, const uint )
{
    archive & nPriority_
            & pBlackBoard_;
}

#endif // __DEC_KnowledgeSource_ABC_h_
