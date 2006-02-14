// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledge_h_
#define __ObjectKnowledge_h_

#include "ASN_Types.h"
#include "Entity_ABC.h"
#include "Resolver_ABC.h"

class Object_ABC;

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledge : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& message, const Resolver_ABC< Object_ABC >& resolver );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const Resolver_ABC< Object_ABC >& resolver_;
    unsigned long id_;
    unsigned long type_;
    //@}
};

#endif // __ObjectKnowledge_h_
