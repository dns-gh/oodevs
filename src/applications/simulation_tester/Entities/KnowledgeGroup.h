// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"
#include "EntityManager.h"

namespace TEST
{
    class Team;
    class PawnKnowledge;
    class PopulationKnowledge;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
    @par    Using example
    @code
    KnowledgeGroup;
    @endcode
*/
// Created: SBO 2005-05-12
// =============================================================================
class KnowledgeGroup
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( const EntityManager& entityManager, Team& team, T_EntityId id );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Accessors
    //@{
    T_EntityId GetId() const;
    //@}

    //! @name Message handlers
    //@{
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );

    void OnReceiveMsgPopulationKnowledgeCreation   ( const ASN1T_MsgPopulationKnowledgeCreation&    asnMsg );
    void OnReceiveMsgPopulationKnowledgeUpdate     ( const ASN1T_MsgPopulationKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg );
    //@}

    //! @name Test Parameters
    //@{
    T_EntityId  GetTestParam_Knowledge          () const;
    T_IdVector& GetTestParam_Knowledges         ( uint nNbr ) const;
    T_EntityId  GetTestParam_KnowledgePopulation() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroup( const KnowledgeGroup& );            //!< Copy constructor
    KnowledgeGroup& operator=( const KnowledgeGroup& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< T_EntityId, PawnKnowledge* > T_PawnKnowledgeMap;
    typedef T_PawnKnowledgeMap::const_iterator     CIT_PawnKnowledgeMap;

    typedef std::map< T_EntityId, PopulationKnowledge* > T_PopulationKnowledgeMap;
    typedef T_PopulationKnowledgeMap::const_iterator     CIT_PopulationKnowledgeMap;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId               nId_;
    Team&                    team_;
    T_PawnKnowledgeMap       knownPawns_;
    T_PopulationKnowledgeMap knownPopulations_;

    const EntityManager&     entityManager_;
    //@}
};

} // end namespace TEST

#include "KnowledgeGroup.inl"

#endif // __KnowledgeGroup_h_
