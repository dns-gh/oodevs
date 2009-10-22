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
class MIL_Object_ABC;
class MIL_Army;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgeObject
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeObject : private boost::noncopyable
{

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
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject ( const MIL_Army& teamKnowing, MIL_Object_ABC& objectKnown );
    
    void                  DestroyKnowledgeObject( DEC_Knowledge_Object& knowledge );

    void                  NotifyKnowledgeObjectDissociatedFromRealObject( const MIL_Object_ABC& objectKnown, DEC_Knowledge_Object& knowledge );
    //@}

    //! @name Queries
    //@{
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObjectFromID( uint nID ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObject      ( const MIL_Object_ABC& objectKnown ) const;
    void                  GetKnowledgesObject     ( T_KnowledgeObjectVector& outContainer ) const;
    bool                  HasKnowledgeObject      ( const MIL_Object_ABC& objectKnown ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesObject( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.begin(); itKnowledge != knowledgeObjectFromIDMap_.end(); )
        {
            boost::shared_ptr< DEC_Knowledge_Object > knowledge = itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesObjectRef( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.begin(); itKnowledge != knowledgeObjectFromIDMap_.end(); )
        {
            DEC_Knowledge_Object& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Object_ABC*, boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectMap;
    typedef T_KnowledgeObjectMap::iterator                                               IT_KnowledgeObjectMap;
    typedef T_KnowledgeObjectMap::const_iterator                                         CIT_KnowledgeObjectMap;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectIDMap;
    typedef T_KnowledgeObjectIDMap::iterator                            IT_KnowledgeObjectIDMap;
    typedef T_KnowledgeObjectIDMap::const_iterator                      CIT_KnowledgeObjectIDMap;
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeObjectMap   objectMap_;
    T_KnowledgeObjectIDMap knowledgeObjectFromIDMap_;
    //@}
};

#endif // __DEC_BlackBoard_CanContainKnowledgeObject_h_
