// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __DEC_KnowledgeBlackBoardObjects_ABC_h_
#define __DEC_KnowledgeBlackBoardObjects_ABC_h_

#include "DEC_KnowledgeBlackBoard_ABC.h"
#include "MIL.h"

class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_KS_ObjectKnowledgeSynthetizer;
class KnowledgesVisitor_ABC;
class MIL_Army_ABC;
class MIL_KnowledgeGroup;
class MIL_ObjectFilter;
class TER_Localisation;
class TER_Polygon;

// =============================================================================
/** @class  DEC_KnowledgeBlackBoardObjects_ABC
    @brief  DEC_KnowledgeBlackBoardObjects_ABC
*/
// Created: JSR 2013-06-24
// =============================================================================
class DEC_KnowledgeBlackBoardObjects_ABC : public DEC_KnowledgeBlackBoard_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KnowledgeBlackBoardObjects_ABC( MIL_KnowledgeGroup* pKnowledgeGroup );
             DEC_KnowledgeBlackBoardObjects_ABC();
    virtual ~DEC_KnowledgeBlackBoardObjects_ABC();
    //@}

    //! @name Checkpoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Update( int currentTimeStep );
    void SendFullState( unsigned int nCtx ) const;
    void SendObjectChangedState() const;
    virtual void Accept( KnowledgesVisitor_ABC& visitor ) const;
    void UpdateUniversalObjects( const MIL_Army_ABC* army );

    void SetKnowledgeGroup( MIL_KnowledgeGroup* group );
    boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;

    DEC_BlackBoard_CanContainKnowledgeObject& GetKnowledgeObjectContainer() const;
    DEC_KS_ObjectKnowledgeSynthetizer& GetKsObjectKnowledgeSynthetizer();
    virtual MIL_Army_ABC& GetArmy() const = 0;

    void GetKnowledgesObject( T_KnowledgeObjectVector& container ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::ObjectKnowledgeId& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int nID ) const;

    bool IsKnown( const MIL_Object_ABC& object ) const;
    void GetObjects( T_KnowledgeObjectVector& container ) const;
    void GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter ) const;
    void GetObjectsInCircle( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const MT_Vector2D& center, double rRadius, bool nonActivatedObstacles ) const;
    void GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone ) const;
    void GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Polygon& zone ) const;
    void GetObjectsIntersectingInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone ) const;
    void GetObjectsWithCapacityInZone( T_KnowledgeObjectDiaIDVector& container, const std::string& capacity, const TER_Localisation& zone ) const;
    bool IsPositionInsideObjectOfType( const std::string& capacity, const MT_Vector2D& loc ) const;
    void GetObjectsAtInteractionHeight( T_KnowledgeObjectVector& container, const MIL_Agent_ABC& agent, const MIL_ObjectFilter& filter ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetClosestObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter ) const;
    boost::shared_ptr< DEC_Knowledge_Object > GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_ObjectFilter& filter ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_KnowledgeGroup* pKnowledgeGroup_;
    std::auto_ptr< DEC_BlackBoard_CanContainKnowledgeObject > pKnowledgeObjectContainer_;
    std::auto_ptr< DEC_KS_ObjectKnowledgeSynthetizer > pKsObjectKnowledgeSynthetizer_;
    //@}
};

#endif // __DEC_KnowledgeBlackBoardObjects_ABC_h_
