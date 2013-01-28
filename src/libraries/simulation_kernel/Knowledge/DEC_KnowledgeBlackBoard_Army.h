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

namespace sword
{
    class ObjectKnowledgeId;
    class CrowdKnowledgeId;
    class UnitKnowledgeId;
    class UrbanObjectKnowledgeId;
}

class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_BlackBoard_CanContainKnowledgeUrban;
class DEC_KS_ObjectKnowledgeSynthetizer;
class DEC_KS_UrbanKnowledgeSynthetizer;
class MIL_Army_ABC;
class MIL_Object_ABC;
class MIL_ObjectFilter;
class TER_Polygon;
class TER_Localisation;
class KnowledgesVisitor_ABC;

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
    explicit DEC_KnowledgeBlackBoard_Army( MIL_Army_ABC& army );
             DEC_KnowledgeBlackBoard_Army();
    virtual ~DEC_KnowledgeBlackBoard_Army();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name CheckPoints
    //@{
    virtual void Update( int currentTimeStep );
    //@}

    //! @name Accessors
    //@{
    MIL_Army_ABC& GetArmy() const;
    DEC_BlackBoard_CanContainKnowledgeObject& GetKnowledgeObjectContainer() const;
    DEC_BlackBoard_CanContainKnowledgeUrban& GetKnowledgeUrbanContainer() const;
    DEC_KS_ObjectKnowledgeSynthetizer& GetKsObjectKnowledgeSynthetizer() const;
    DEC_KS_UrbanKnowledgeSynthetizer& GetKsUrbanKnowledgeSynthetizer() const;
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( unsigned int nCtx ) const;
    virtual void SendChangedState() const;
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const sword::UnitKnowledgeId& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( unsigned int nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::ObjectKnowledgeId& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const MIL_Population& population ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( unsigned int nID ) const;
    //@}

    //! @name Queries
    //@{
    void Finalize();
    void GetObjects( T_KnowledgeObjectVector& container ) const; // only for population
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObjectFromID( unsigned int nID ) const; ; // only for population
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObjectFromObjectID( unsigned int nID ) const; // only for population
    void GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone ); // only for population
    boost::shared_ptr< DEC_Knowledge_Object > GetKnowledgeObject( const MIL_Object_ABC& object ) const; // only for population
    void GetKnowledgesObject( T_KnowledgeObjectVector& container ) const;
    void GetUrbanObjects( T_UrbanObjectVector& container ) const;
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
    //@}

private:
    MIL_Army_ABC* pArmy_;
    // Containers
    DEC_BlackBoard_CanContainKnowledgeObject* pKnowledgeObjectContainer_;
    DEC_BlackBoard_CanContainKnowledgeUrban* pKnowledgeUrbanContainer_;
    // Knowledge sources
    DEC_KS_ObjectKnowledgeSynthetizer* pKsObjectKnowledgeSynthetizer_;
    DEC_KS_UrbanKnowledgeSynthetizer* pKsUrbanKnowledgeSynthetizer_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_Army )

#endif // __DEC_KnowledgeBlackBoard_Army_h_
