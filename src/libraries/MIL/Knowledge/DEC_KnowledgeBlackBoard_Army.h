// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_Army.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:47 $
// $Revision: 3 $
// $Workfile: DEC_KnowledgeBlackBoard_Army.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeBlackBoard_Army_h_
#define __DEC_KnowledgeBlackBoard_Army_h_

#include "MIL.h"

#include "DEC_KnowledgeBlackBoard_ABC.h"

class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_KS_ObjectKnowledgeSynthetizer;
class MIL_Army;
class MIL_RealObject_ABC;
class MIL_RealObjectTypeFilter;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard_Army
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard_Army : public DEC_KnowledgeBlackBoard_ABC
{
    MT_COPYNOTALLOWED( DEC_KnowledgeBlackBoard_Army )

public:
    //! @name Constructors/Destructor
    //@{
              DEC_KnowledgeBlackBoard_Army( MIL_Army& army );
              DEC_KnowledgeBlackBoard_Army();
     virtual ~DEC_KnowledgeBlackBoard_Army();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Accessors
    //@{
    MIL_Army&                                 GetArmy                        () const;
    DEC_BlackBoard_CanContainKnowledgeObject& GetKnowledgeObjectContainer    () const;
    DEC_KS_ObjectKnowledgeSynthetizer&        GetKsObjectKnowledgeSynthetizer() const;
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   () const;
    virtual void SendChangedState() const;
    //@}

    //! @name Queries
    //@{
    bool                  IsKnown                 ( const MIL_RealObject_ABC& oject ) const;
    DEC_Knowledge_Object* GetKnowledgeObjectFromID( uint nID ) const;

    void                  GetKnowledgesObject( T_KnowledgeObjectVector& container ) const;
    DEC_Knowledge_Object* GetKnowledgeObject ( MIL_RealObject_ABC& object ) const;
    DEC_Knowledge_Object* GetKnowledgeObject ( const DEC_Knowledge_ObjectCollision& collision ) const;

    void                  GetObjects            ( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter ) const;
    void                  GetObjects            ( T_KnowledgeObjectVector&      container, const MIL_RealObjectTypeFilter& filter ) const;
    void                  GetObjects            ( T_KnowledgeObjectVector&      container ) const;
    DEC_Knowledge_Object* GetClosestObject      ( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const;
    DEC_Knowledge_Object* GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const;

//    bool                  HasNewKnowledgeObject () const;
    //@}

private:
    MIL_Army* pArmy_;

    // Containers
    DEC_BlackBoard_CanContainKnowledgeObject* pKnowledgeObjectContainer_;

    // Knowledge sources
    DEC_KS_ObjectKnowledgeSynthetizer* pKsObjectKnowledgeSynthetizer_;
};

#include "DEC_KnowledgeBlackBoard_Army.inl"

#endif // __DEC_KnowledgeBlackBoard_Army_h_
