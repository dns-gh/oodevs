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

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class MIL_RealObject_ABC;
class MIL_AgentPion;
class DEC_Knowledge_ObjectCollision;

// =============================================================================
/** @class  DEC_KS_ObjectInteraction
    @brief  This knowledge source (KS) is an 'IN', which is used when an agent
            interacts with an object, to add this object to its perception
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_ObjectInteraction : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_ObjectInteraction ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_ObjectInteraction( DEC_KnowledgeBlackBoard& blackBoard, const MIL_AgentPion& agentInteracting );
    virtual ~DEC_KS_ObjectInteraction();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    virtual void Clean  ();
    //@}

    //! @name Events
    //@{
    void NotifyObjectInteraction( MIL_RealObject_ABC& object );
    void NotifyObjectCollision  ( MIL_RealObject_ABC& object, const MT_Vector2D& vPosition );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_RealObject_ABC* > T_ObjectVector;
    typedef T_ObjectVector::const_iterator     CIT_ObjectVector;

    typedef std::pair< MIL_RealObject_ABC*, MT_Vector2D > T_ObjectCollisionPair;
    typedef std::vector< T_ObjectCollisionPair >          T_ObjectCollisionVector;
    typedef T_ObjectCollisionVector::const_iterator       CIT_ObjectCollisionVector;
    //@}

private:
    //! @name Accessors
    //@{
    const MIL_AgentPion&            GetAgentInteracting         () const;
    DEC_Knowledge_ObjectPerception& GetKnowledgeObjectPerception( MIL_RealObject_ABC& object ) const;
    DEC_Knowledge_ObjectCollision&  GetKnowledgeObjectCollision ( MIL_RealObject_ABC& object ) const;
    //@}

    //! @name Tools
    //@{
    void CleanKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge );
    //@}

private:
    const MIL_AgentPion*    pAgentInteracting_;
    T_ObjectVector          objectInteractions_;
    T_ObjectCollisionVector objectCollisions_;
};

#include "DEC_KS_ObjectInteraction.inl"

#endif // __DEC_KS_ObjectInteraction_h_
