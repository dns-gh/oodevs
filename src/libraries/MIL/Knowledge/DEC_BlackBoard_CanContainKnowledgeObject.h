// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:35 $
// $Revision: 4 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObject.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeObject_h_
#define __DEC_BlackBoard_CanContainKnowledgeObject_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_Object;
class MIL_RealObject_ABC;
class MIL_Army;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgeObject
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeObject
{
    MT_COPYNOTALLOWED( DEC_BlackBoard_CanContainKnowledgeObject )

public:
    //! @name Constructors/Destructor
    //@{
     DEC_BlackBoard_CanContainKnowledgeObject();
    ~DEC_BlackBoard_CanContainKnowledgeObject();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_Object& CreateKnowledgeObject ( const MIL_Army& teamKnowing, MIL_RealObject_ABC& objectKnown );
    void                  DestroyKnowledgeObject( DEC_Knowledge_Object& knowledge );

    void                  NotifyKnowledgeObjectDissociatedFromRealObject( const MIL_RealObject_ABC& objectKnown, DEC_Knowledge_Object& knowledge );
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_Object* GetKnowledgeObjectFromMosID( uint nID ) const;
    DEC_Knowledge_Object* GetKnowledgeObjectFromDiaID( uint nID ) const;
    void                  GetKnowledgesObject        ( const MIL_RealObject_ABC& objectKnown, T_KnowledgeObjectVector& outContainer ) const;
    void                  GetKnowledgesObject        ( T_KnowledgeObjectVector& outContainer ) const;
    bool                  HasKnowledgeObject         ( const MIL_RealObject_ABC& objectKnown ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesObject( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromMosIDMap_.begin(); itKnowledge != knowledgeObjectFromMosIDMap_.end(); )
        {
            DEC_Knowledge_Object& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    //@}

    //! @name Types
    //@{
public:
    typedef std::map< const MIL_RealObject_ABC*, DEC_Knowledge_Object* > T_KnowledgeObjectMap;
    typedef T_KnowledgeObjectMap::iterator                               IT_KnowledgeObjectMap;
    typedef T_KnowledgeObjectMap::const_iterator                         CIT_KnowledgeObjectMap;

private:
    typedef std::map< uint, DEC_Knowledge_Object* >   T_KnowledgeObjectIDMap;
    typedef T_KnowledgeObjectIDMap::iterator          IT_KnowledgeObjectIDMap;
    typedef T_KnowledgeObjectIDMap::const_iterator    CIT_KnowledgeObjectIDMap;
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeObjectMap   knowledgeObjectMap_;
    T_KnowledgeObjectIDMap knowledgeObjectFromMosIDMap_;
    T_KnowledgeObjectIDMap knowledgeObjectFromDiaIDMap_; //$$$ A remplacer par une table de hash
    //@}
};

#include "DEC_BlackBoard_CanContainKnowledgeObject.inl"

#endif // __DEC_BlackBoard_CanContainKnowledgeObject_h_
