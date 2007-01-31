// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeFactory_h_
#define __ObjectKnowledgeFactory_h_

#include "game_asn/Asn.h"

namespace kernel
{
    class Controllers;
    class Team_ABC;
}

class ObjectKnowledge_ABC;
class Model;
class StaticModel;

// =============================================================================
/** @class  ObjectKnowledgeFactory
    @brief  ObjectKnowledgeFactory
    // $$$$ AGE 2006-08-02: abc
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledgeFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~ObjectKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual ObjectKnowledge_ABC* Create( const kernel::Team_ABC& owner, const ASN1T_MsgObjectKnowledgeCreation& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgeFactory( const ObjectKnowledgeFactory& );            //!< Copy constructor
    ObjectKnowledgeFactory& operator=( const ObjectKnowledgeFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __ObjectKnowledgeFactory_h_
