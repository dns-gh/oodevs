// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ObjectInteraction.h $
// $Author: Jvt $
// $Modtime: 24/03/05 15:16 $
// $Revision: 4 $
// $Workfile: DEC_KS_ObjectInteraction.h $
//
// *****************************************************************************

#ifndef __DEC_KS_ObjectInteraction_h_
#define __DEC_KS_ObjectInteraction_h_

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_AgentPion;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_ObjectPerception;
class MIL_Object_ABC;
class MIL_AgentPion;
class MT_Line;

// =============================================================================
/** @class  DEC_KS_ObjectInteraction
    @brief  This knowledge source (KS) is an 'IN', which is used when an agent
            interacts with an object, to add this object to its perception
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_ObjectInteraction : public DEC_KnowledgeSource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_ObjectInteraction( DEC_KnowledgeBlackBoard_AgentPion& blackBoard );
             DEC_KS_ObjectInteraction();
    virtual ~DEC_KS_ObjectInteraction();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk( int currentTimeStep );
    virtual void Clean();
    void Clear();
    virtual void CleanDeletedAgentKnowledges();
    //@}

    //! @name Events
    //@{
    void NotifyObjectInteraction( MIL_Object_ABC& object );
    void NotifyObjectCollision( MIL_Object_ABC& object, const MT_Vector2D& startPos, const MT_Vector2D& endPos );
    void NotifyDisasterCollision( MIL_Object_ABC& object, const MT_Vector2D& startPos, const MT_Vector2D& endPos );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_Object_ABC* >   T_ObjectVector;

    typedef std::pair< MIL_Object_ABC*, MT_Vector2D > T_ObjectCollisionPair;
    typedef std::vector< T_ObjectCollisionPair >      T_ObjectCollisionVector;
    //@}

private:
    //! @name Accessors
    //@{
    DEC_Knowledge_ObjectPerception& GetKnowledgeObjectPerception( MIL_Object_ABC& object ) const;
    DEC_Knowledge_ObjectCollision&  GetKnowledgeObjectCollision ( MIL_Object_ABC& object ) const;
    //@}

    //! @name Helpers
    //@{
    void CleanKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge );
    void NotifyCollision( MIL_Object_ABC& object, const MT_Vector2D& startPos, const MT_Vector2D& endPos );
    //@}

private:
    DEC_KnowledgeBlackBoard_AgentPion* pBlackBoard_;
    T_ObjectVector objectInteractions_;
    T_ObjectCollisionVector objectCollisions_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_ObjectInteraction )

#endif // __DEC_KS_ObjectInteraction_h_
