// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Team.h $
// $Author: Sbo $
// $Modtime: 1/06/05 11:19 $
// $Revision: 4 $
// $Workfile: Team.h $
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"
#include "Entities/EntityManager.h"

namespace TEST
{
    class ObjectKnowledge;

// =============================================================================
/** @class  Team
    @brief  Team
    @par    Using example
    @code
    Team;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class Team
{
public:
    //! @name Enums
    //@{
    enum E_Diplomacy
    {
        eUnknown,
        eFriend,
        eEnemy,
        eNeutral
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Team( const EntityManager& entityManager, T_EntityId nSimId, DIN::DIN_Input& input );
    virtual ~Team();
    //@}

    //! @name Accessors
    //@{
          T_EntityId   GetId          ()                   const;
	const std::string& GetName        ()                   const;
          E_Tristate   IsAFriend      ( const Team& team ) const;
          E_Tristate   IsAnEnemy      ( const Team& team ) const;
          E_Tristate   IsNeutral      ( const Team& team ) const;
          E_Diplomacy  GetRelationWith( const Team& team ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetRelation( Team& otherTeam, ASN1T_EnumDiplomacy diplomacy );
    //@}

    //! @name Message handlers
    //@{
    void OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );
    //@}

    //! @name Test Parameters
    //@{
    T_EntityId  GetTestParam_Object () const;
    T_IdVector& GetTestParam_Objects() const;
    T_EntityId  GetTestParam_Population() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Team( const Team& );            //!< Copy constructor
    Team& operator=( const Team& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const Team*, E_Diplomacy > T_TeamRelationMap;
    typedef T_TeamRelationMap::const_iterator    CIT_TeamRelationMap;

    typedef std::map< T_EntityId, ObjectKnowledge* > T_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_iterator     CIT_ObjectKnowledgeMap;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId           nId_;
	std::string          strName_;
	T_TeamRelationMap    relations_;
    T_ObjectKnowledgeMap knownObjects_;

    const EntityManager& entityManager_;
    //@}
};

} // end namespace TEST

#include "Team.inl"

#endif // __Team_h_
