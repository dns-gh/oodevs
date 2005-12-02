// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_AutomateQuerier.h $
// $Author: Nld $
// $Modtime: 11/05/05 16:02 $
// $Revision: 5 $
// $Workfile: DEC_KS_AutomateQuerier.h $
//
// *****************************************************************************

#ifndef __DEC_KS_AutomateQuerier_h_
#define __DEC_KS_AutomateQuerier_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"
#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_RapForLocal;
class DEC_Knowledge_RapForGlobal;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class MIL_RealObject_ABC;
class MIL_Automate;
class MIL_RealObjectTypeFilter;

// =============================================================================
/** @class  DEC_KS_AutomateQuerier
    @brief  This knowledge source (KS) is an 'OUT', which is used as a black board
            interrogator from the outside
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_AutomateQuerier : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_AutomateQuerier ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_AutomateQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_Automate& automate );
    virtual ~DEC_KS_AutomateQuerier();
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_RapForGlobal& GetKnowledgeRapForGlobal() const;
    MT_Float                    GetRapForGlobalValue    () const;
    void                        GetObjects              ( T_KnowledgeObjectDiaIDVector&     container, const MIL_RealObjectTypeFilter& filter ) const;
    void                        GetFriendsInZone        ( T_KnowledgeAgentDiaIDVector&      container, const TER_Localisation& zone ) const;
    void                        GetPopulations          ( T_KnowledgePopulationDiaIDVector& container ) const;
    //@}

private:
    const MIL_Automate* pAutomate_;
};

#include "DEC_KS_AutomateQuerier.inl"

#endif // __DEC_KS_AutomateQuerier_h_
