// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Side_h_
#define __Side_h_

#include "game_asn/Simulation.h"
#include "ModelRefsContainer.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class KnowledgeGroup;
    class Formation;
    class Model;
	class Object;
	class Population;

// =============================================================================
/** @class  Side
    @brief  Side
*/
// Created: NLD 2006-09-19
// =============================================================================
class Side : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Side( Model& model, const ASN1T_MsgTeamCreation& msg );
    virtual ~Side();
    //@}

    //! @name Accessors
    //@{
    ModelRefsContainer< KnowledgeGroup >& GetKnowledgeGroups();
    ModelRefsContainer< Formation      >& GetFormations     ();
	ModelRefsContainer< Object         >& GetObjects        ();
	ModelRefsContainer< Population     >& GetPopulations    ();
    unsigned long                         GetID             () const;
    //@}

    //! @name Operations
    //@{
    using Entity_ABC::Update;
    void Update( const ASN1T_MsgChangeDiplomacy&    asnMsg );
    void Update( const ASN1T_MsgChangeDiplomacyAck& asnMsg );
    virtual void SendCreation  ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void Accept        ( ModelVisitor_ABC& visitor );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Side( const Side& );            //!< Copy constructor
    Side& operator=( const Side& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< Side*, ASN1T_EnumDiplomacy >    T_DiplomacyMap;
    typedef T_DiplomacyMap::const_iterator          CIT_DiplomacyMap;
    //@}

public:
          Model&                               model_;
    const unsigned long                        nID_;
          std::string                          strName_;
          ASN1T_EnumDiplomacy                  nType_;
          ModelRefsContainer< KnowledgeGroup > knowledgeGroups_;
          ModelRefsContainer< Formation      > formations_;
		  ModelRefsContainer< Object		 > objects_;
		  ModelRefsContainer< Population     > populations_;
          T_DiplomacyMap                       diplomacies_;
};

}

#include "Side.inl"

#endif // __Side_h_
