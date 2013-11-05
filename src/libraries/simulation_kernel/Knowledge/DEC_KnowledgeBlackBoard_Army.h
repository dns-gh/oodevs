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

class DEC_BlackBoard_CanContainKnowledgeUrban;
class DEC_KS_UrbanKnowledgeSynthetizer;
class MIL_Army_ABC;
class TER_Polygon;
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

    //! @name Accessors
    //@{
    MIL_Army_ABC& GetArmy() const;
    DEC_BlackBoard_CanContainKnowledgeUrban& GetKnowledgeUrbanContainer() const;
    DEC_KS_UrbanKnowledgeSynthetizer& GetKsUrbanKnowledgeSynthetizer() const;
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( unsigned int nCtx ) const;
    virtual void SendChangedState() const;
    //@}

    //! @name tools::Resolver
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( unsigned int nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int nID ) const;

    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const sword::Id& asn ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( const MIL_Population& population ) const;
    virtual boost::shared_ptr< DEC_Knowledge_Population > ResolveKnowledgePopulation( unsigned int nID ) const;
    //@}

    //! @name Queries
    //@{
    void Finalize();
    void GetUrbanObjects( T_UrbanObjectVector& container ) const;
    //@}

private:
    MIL_Army_ABC* pArmy_;
    // Containers
    DEC_BlackBoard_CanContainKnowledgeUrban* pKnowledgeUrbanContainer_;
    // Knowledge sources
    DEC_KS_UrbanKnowledgeSynthetizer* pKsUrbanKnowledgeSynthetizer_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KnowledgeBlackBoard_Army )

#endif // __DEC_KnowledgeBlackBoard_Army_h_
