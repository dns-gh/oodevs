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

#include "DEC_KnowledgeBlackBoard_ABC.h"

class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_KS_ObjectKnowledgeSynthetizer;
class MIL_Army;
class MIL_Object_ABC;
class MIL_ObjectFilter;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoard_Army
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KnowledgeBlackBoard_Army : public DEC_KnowledgeBlackBoard_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KnowledgeBlackBoard_Army( MIL_Army& army );
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

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const ASN1T_UnitKnowledge&  asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent ( const DIA_Variable_ABC&      dia ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent (       uint                   nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const DIA_Variable_ABC&      dia ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject(       uint                   nID ) const;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const DIA_Variable_ABC&          dia ) const;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation(       uint                       nID ) const;
    //@}

    //! @name Queries
    //@{
    bool                  IsKnown                 ( const MIL_Object_ABC& oject ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObjectFromID( uint nID ) const;

    void                  GetKnowledgesObject( T_KnowledgeObjectVector& container ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObject ( MIL_Object_ABC& object ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObject ( const DEC_Knowledge_ObjectCollision& collision ) const;

    void                  GetObjects                   ( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter ) const;
    void                  GetObjects                   ( T_KnowledgeObjectVector&      container ) const;
    void                  GetObjectsInCircle           ( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, MT_Float rRadius );
    void                  GetObjectsInZone             ( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone );
    void                  GetObjectsInZone             ( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Polygon& zone );
    void                  GetObjectsAtInteractionHeight( T_KnowledgeObjectVector&      container, MT_Float rHeight, const MIL_ObjectFilter& filter ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetClosestObject             ( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter  ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetClosestFriendObject       ( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter  ) const;
    //@}

private:
    MIL_Army* pArmy_;

    // Containers
    DEC_BlackBoard_CanContainKnowledgeObject* pKnowledgeObjectContainer_;

    // Knowledge sources
    DEC_KS_ObjectKnowledgeSynthetizer* pKsObjectKnowledgeSynthetizer_;
};

#endif // __DEC_KnowledgeBlackBoard_Army_h_
