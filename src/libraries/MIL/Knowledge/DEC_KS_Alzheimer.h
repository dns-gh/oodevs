// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Alzheimer.h $
// $Author: Nld $
// $Modtime: 1/09/04 14:16 $
// $Revision: 1 $
// $Workfile: DEC_KS_Alzheimer.h $
//
// *****************************************************************************

#ifndef __DEC_KS_Alzheimer_h_
#define __DEC_KS_Alzheimer_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;

// =============================================================================
/** @class  DEC_KS_Alzheimer
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used to apply an
            'alzheimer' process on the knowledges
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_Alzheimer : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_Alzheimer ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_Alzheimer( DEC_KnowledgeBlackBoard& blackBoard );
    virtual ~DEC_KS_Alzheimer();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    //@}

private:
    //! @name Tools
    //@{
    void UpdateKnowledgeAgent ( DEC_Knowledge_Agent&  knowledge );
    void UpdateKnowledgeObject( DEC_Knowledge_Object& knowledge );
    //@}
};

#include "DEC_KS_Alzheimer.inl"

#endif // __DEC_KS_Alzheimer_h_
