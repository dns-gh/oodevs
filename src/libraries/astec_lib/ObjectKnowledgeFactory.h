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

#include "ASN_Types.h"

class ObjectKnowledge;
class Controllers;
class Model;
class StaticModel;
class Team;

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
             ObjectKnowledgeFactory( Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~ObjectKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual ObjectKnowledge* Create( const Team& owner, const ASN1T_MsgObjectKnowledgeCreation& message );
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
    Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __ObjectKnowledgeFactory_h_
