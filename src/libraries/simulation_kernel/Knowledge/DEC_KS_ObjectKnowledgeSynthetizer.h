// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h $
// $Author: Jvt $
// $Modtime: 23/03/05 18:57 $
// $Revision: 3 $
// $Workfile: DEC_KS_ObjectKnowledgeSynthetizer.h $
//
// *****************************************************************************

#ifndef __DEC_KS_ObjectKnowledgeSynthetizer_h_
#define __DEC_KS_ObjectKnowledgeSynthetizer_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard_Army;
class DEC_Knowledge_ObjectPerception;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_Object;
class MIL_Army;
class MIL_Object_ABC;

// =============================================================================
/** @class  DEC_KS_ObjectKnowledgeSynthetizer
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used :
                - To synthetize the subordinate's perceptions of the objects 
                  into knowledges for a army
                - To add the 'global' army knowledges (magic actions on an object)
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_ObjectKnowledgeSynthetizer : public DEC_KnowledgeSource_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_ObjectKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_Army& blackBoard );
             DEC_KS_ObjectKnowledgeSynthetizer();
    virtual ~DEC_KS_ObjectKnowledgeSynthetizer();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Events
    //@{
    void AddEphemeralObjectKnowledge( MIL_Object_ABC& objectKnown ); // The object will be perceived virtually only for one tick
    void AddObjectKnowledgeToForget ( MIL_Object_ABC& objectKnown );
    void AddObjectKnowledgeToForget ( DEC_Knowledge_Object& knowledge );
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    virtual void Clean  ();
    //@}

private:    
    //! @name Update
    //@{
    void                  UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception );
    void                  UpdateKnowledgesFromObjectCollision ( const DEC_Knowledge_ObjectCollision& collision );
    DEC_Knowledge_Object& GetKnowledgeToUpdate                ( MIL_Object_ABC& objectKnown ) const;
    void                  UpdateKnowledgeRelevance            ( DEC_Knowledge_Object& knowledge );
    void                  CleanKnowledgeObject                ( DEC_Knowledge_Object& knowledge );
    //@}

    //! @name Operations
    //@{
    void SynthetizeSubordinatesPerception();
    void ProcessEphemeralKnowledges      ();
    void ProcessObjectsToForget          ();
    void ProcessKnowledgesObjectToForget ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_Object_ABC* >  T_ObjectVector;
    typedef T_ObjectVector::const_iterator  CIT_ObjectVector;
    //@}
    
private:
    //! @name Members
    //@{
    DEC_KnowledgeBlackBoard_Army* pBlackBoard_;
    T_ObjectVector                ephemeralKnowledges_;
    T_ObjectVector                objectsToForget_;
    T_KnowledgeObjectVector       knowledgesObjectToForget_;
    //@}
};

#include "DEC_KS_ObjectKnowledgeSynthetizer.inl"

#endif // __DEC_KS_ObjectKnowledgeSynthetizer_h_
