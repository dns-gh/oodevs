// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_RapFor.h $
// $Author: Jvt $
// $Modtime: 24/03/05 13:12 $
// $Revision: 2 $
// $Workfile: DEC_KS_RapFor.h $
//
// *****************************************************************************

#ifndef __DEC_KS_RapFor_h_
#define __DEC_KS_RapFor_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_KnowledgeGroup;

// =============================================================================
/** @class  DEC_KS_RapFor
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used to apply n
            'RapFor' process on the knowledges
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_RapFor : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_RapFor ) 
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_RapFor( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard );
             DEC_KS_RapFor();
    virtual ~DEC_KS_RapFor();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

private:
    DEC_KnowledgeBlackBoard_KnowledgeGroup* pBlackBoard_;
};

#include "DEC_KS_RapFor.inl"

#endif // __DEC_KS_RapFor_h_
