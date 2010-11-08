// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeFactory_h_
#define __ObjectKnowledgeFactory_h_

#include "ObjectAttributesFactory.h"

namespace Common
{
    class ObjectAttributes;
}

namespace MsgsSimToClient
{
    class MsgObjectKnowledgeCreation;
}

namespace kernel
{
    class Controllers;    
    class ObjectKnowledge_ABC;
}

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
    virtual kernel::ObjectKnowledge_ABC* Create( const kernel::Entity_ABC& owner, const MsgsSimToClient::MsgObjectKnowledgeCreation& message );
    void RegisterAttributes( kernel::ObjectKnowledge_ABC& result, const Common::ObjectAttributes& attributes ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledgeFactory( const ObjectKnowledgeFactory& );            //!< Copy constructor
    ObjectKnowledgeFactory& operator=( const ObjectKnowledgeFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{    
    const ObjectAttributesFactory attributesFactory_;
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __ObjectKnowledgeFactory_h_
