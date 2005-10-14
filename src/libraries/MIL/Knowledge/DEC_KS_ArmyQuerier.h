// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ArmyQuerier.h $
// $Author: Nld $
// $Modtime: 9/05/05 14:45 $
// $Revision: 6 $
// $Workfile: DEC_KS_ArmyQuerier.h $
//
// *****************************************************************************

#ifndef __DEC_KS_ArmyQuerier_h_
#define __DEC_KS_ArmyQuerier_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"
#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class PHY_UnitViewable_ABC;
class MIL_RealObject_ABC;
class MIL_Army;
class MIL_RealObjectTypeFilter;

// =============================================================================
/** @class  DEC_KS_ArmyQuerier
    @brief  This knowledge source (KS) is an 'OUT', which is used as a black board
            interrogator from the outside
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_ArmyQuerier :  public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_ArmyQuerier ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_ArmyQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_Army& army );
    virtual ~DEC_KS_ArmyQuerier();
    //@}

    //! @name Queries / Operations
    //@{
    bool                  IsKnown                 ( const MIL_RealObject_ABC& oject ) const;
    DEC_Knowledge_Object* GetKnowledgeObjectFromID( uint nID ) const;

    void                  GetKnowledgesObject( T_KnowledgeObjectVector& container ) const;
    DEC_Knowledge_Object* GetKnowledgeObject ( MIL_RealObject_ABC& object ) const;
    DEC_Knowledge_Object* GetKnowledgeObject ( const DEC_Knowledge_ObjectCollision& collision ) const;

    void                  GetObjects      ( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter ) const;
    void                  GetObjects      ( T_KnowledgeObjectVector&      container, const MIL_RealObjectTypeFilter& filter ) const;
    void                  GetObjects      ( T_KnowledgeObjectVector&      container ) const;
    DEC_Knowledge_Object* GetClosestObject( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const;
    //@}

private:
    const MIL_Army* pArmy_;
};

#include "DEC_KS_ArmyQuerier.inl"

#endif // __DEC_KS_ArmyQuerier_h_
