// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeRapFor.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 2 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeRapFor.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeRapFor_h_
#define __DEC_BlackBoard_CanContainKnowledgeRapFor_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

#include "DEC_Knowledge_RapForLocal.h"
#include "DEC_Knowledge_RapForGlobal.h"

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeRapFor
{
    MT_COPYNOTALLOWED( DEC_BlackBoard_CanContainKnowledgeRapFor )

public:
    //! @name Constructors/Destructor
    //@{
     DEC_BlackBoard_CanContainKnowledgeRapFor();
    ~DEC_BlackBoard_CanContainKnowledgeRapFor();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_RapForLocal&  GetKnowledgeRapForLocal ();
    DEC_Knowledge_RapForGlobal& GetKnowledgeRapForGlobal();
    //@}

private:
    //! @name Member data
    //@{
    DEC_Knowledge_RapForLocal  knowledgeRapForLocal_;
    DEC_Knowledge_RapForGlobal knowledgeRapForGlobal_;
    //@}
};

#include "DEC_BlackBoard_CanContainKnowledgeRapFor.inl"

#endif // __DEC_BlackBoard_CanContainKnowledgeRapFor_h_
