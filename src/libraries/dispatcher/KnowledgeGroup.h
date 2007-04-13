 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "game_asn/Asn.h"
#include "ModelRefsContainer.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Model;
    class Dispatcher;
    class Side;
    class Automat;
    class Publisher_ABC;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
*/
// Created: NLD 2006-09-19
// =============================================================================
class KnowledgeGroup : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( Model& model, const ASN1T_MsgKnowledgeGroupCreation& msg );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Accessors
    //@{
    unsigned long                  GetID      () const;
    ModelRefsContainer< Automat >& GetAutomats();
    //@}

    //! @name Main
    //@{
    void Update( const ASN1T_MsgKnowledgeGroupCreation& msg );
    void SendCreation( Publisher_ABC& publisher ) const;
	virtual void SendFullUpdate( Publisher_ABC& publisher ) const;
    void Accept( ModelVisitor_ABC& visitor );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroup( const KnowledgeGroup& );            //!< Copy constructor
    KnowledgeGroup& operator=( const KnowledgeGroup& ); //!< Assignement operator
    //@}

private:
    const unsigned long                 nID_;
          Side&                         side_;
          ModelRefsContainer< Automat > automats_;
};

}

#include "KnowledgeGroup.inl"

#endif // __KnowledgeGroup_h_
