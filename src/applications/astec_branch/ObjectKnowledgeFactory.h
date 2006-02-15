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
class Controller;
class Model;

// =============================================================================
/** @class  ObjectKnowledgeFactory
    @brief  ObjectKnowledgeFactory
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledgeFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeFactory( Controller& controller, Model& model );
    virtual ~ObjectKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual ObjectKnowledge* Create( const ASN1T_MsgObjectKnowledgeCreation& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgeFactory( const ObjectKnowledgeFactory& );            //!< Copy constructor
    ObjectKnowledgeFactory& operator=( const ObjectKnowledgeFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Model& model_;
    //@}
};

#endif // __ObjectKnowledgeFactory_h_
