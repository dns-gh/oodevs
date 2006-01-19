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

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_KnowledgeBlackBoard;

// =============================================================================
/** @class  DEC_KnowledgeSource_ABC
    @brief  A knowledge source is able to 
                - publish or modify knowledges of the black board
                - access these knowledges
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KnowledgeSource_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             DEC_KnowledgeSource_ABC( DEC_KnowledgeBlackBoard& blackBoard, uint nPriority );
             DEC_KnowledgeSource_ABC();
    virtual ~DEC_KnowledgeSource_ABC();
    //@}

    //! @name CheckPoints
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

protected:
    //! @name Member data
    //@{
    DEC_KnowledgeBlackBoard* pBlackBoard_;
    uint                     nPriority_;
    //@}
};

#include "DEC_KnowledgeSource_ABC.inl"

#endif // __DEC_KnowledgeSource_ABC_h_
