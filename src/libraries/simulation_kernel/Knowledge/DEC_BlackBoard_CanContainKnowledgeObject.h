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
#include <boost/shared_ptr.hpp>

class DEC_Knowledge_Object;
class MIL_Object_ABC;
class MIL_Army_ABC;
class MIL_Agent_ABC;
class MIL_KnowledgeGroup;
class MIL_ObjectFilter;
class TER_Localisation;
class TER_Polygon;
class KnowledgesVisitor_ABC;

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
             DEC_BlackBoard_CanContainKnowledgeObject( MIL_KnowledgeGroup* pKnowledgeGroup );
    virtual ~DEC_BlackBoard_CanContainKnowledgeObject();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< unsigned int, boost::shared_ptr< DEC_Knowledge_Object > > T_KnowledgeObjectMap;
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void SetKnowledgeGroup( MIL_KnowledgeGroup* group );
    //@}

    //! @name Operations
    //@{
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject( MIL_Object_ABC& objectKnown );
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject( boost::shared_ptr< DEC_Knowledge_Object >& object );
    void DestroyKnowledgeObject( DEC_Knowledge_Object& knowledge );
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
    void Prepare();
    void UpdateUniversalObjects();
    void Merge( const DEC_BlackBoard_CanContainKnowledgeObject& subGroup );
    void GetObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter, bool bCheckBypassed = true );
    void GetObjectsInCircle( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, double rRadius, bool nonActivatedObstacles );
    void GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone );
    void GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Polygon& zone );
    void GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter ) const;
    void GetObjects( T_KnowledgeObjectVector& container ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetClosestObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter, const MIL_Army_ABC& army ) const;
    void GetResourceNetworksInZone( T_ResourceNetworkVector& container, const TER_Localisation& zone, const std::string& type = std::string() );
    bool IsPositionInsideObjectOfType( const std::string& capacity, const MT_Vector2D& loc );
    void GetObjectsWithCapacityInZone( T_KnowledgeObjectDiaIDVector& container, const std::string& capacity, const TER_Localisation& zone );
    void GetObjectsIntersectingInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone );
    void SaveAllCurrentKnowledgeObject();
    //@}

    //! @name Queries
    //@{
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObjectFromID( unsigned int nID ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObjectFromObjectID( unsigned int nID ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObject( const MIL_Object_ABC& objectKnown ) const;
    const T_KnowledgeObjectMap& GetKnowledgeObjects() const;
    DEC_Knowledge_Object* RetrieveKnowledgeObject( const MIL_Object_ABC& objectKnown ) const;
    void GetKnowledgesObject( T_KnowledgeObjectVector& outContainer ) const;
    bool HasKnowledgeObject( const MIL_Object_ABC& objectKnown ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesObject( UnaryFunction& fct ) const
    {
        for( auto it = objectMap_.begin(); it != objectMap_.end(); )
        {
            boost::shared_ptr< DEC_Knowledge_Object > knowledge = it->second;
            ++it;
            fct( knowledge );
        }
    }
    template < class UnaryFunction >
    void ApplyOnPreviousKnowledgesObject( UnaryFunction& fct ) const
    {
        for( auto itKnowledge = previousObjectMap_.begin(); itKnowledge != previousObjectMap_.end(); )
        {
            DEC_Knowledge_Object& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    template < class UnaryFunction >
    void ApplyOnKnowledgesObjectRef( UnaryFunction& fct ) const
    {
        for( auto it = objectMap_.begin(); it != objectMap_.end(); )
        {
            DEC_Knowledge_Object& knowledge = *it->second;
            ++it;
            fct( knowledge );
        }
    }
    //@}

private:
    //! @name Types
    //@{
    
    //@}

private:
    //! @name Helpers
    //@
    void SetCachedObjectsAtInteractionHeight( const T_KnowledgeObjectVector& container, double rHeight );
    void GetCachedObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, double rHeight );
    bool HasObjectsAtInteractionHeightCache( double rHeight ) const;
    void UpdateUniversalObject( MIL_Object_ABC& object );
    void CreateKnowledgeObject( const DEC_Knowledge_Object& knowledge );
    //@}

private:
    //! @name Member data
    //@{
    MIL_KnowledgeGroup* pKnowledgeGroup_;
    std::map< double, T_KnowledgeObjectVector > obstacleCache_;
    T_KnowledgeObjectMap objectMap_;
    T_KnowledgeObjectMap previousObjectMap_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeObject )

#endif // __DEC_BlackBoard_CanContainKnowledgeObject_h_
