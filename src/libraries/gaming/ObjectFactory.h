// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectFactory_h_
#define __ObjectFactory_h_

#include "ObjectFactory_ABC.h"
#include "ObjectAttributesFactory.h"

namespace actions
{
    class ActionsModel;
}

namespace sword
{
    class ObjectAttributes;
    class ObjectKnowledgeCreation;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ObjectFactory
    @brief  Object factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class ObjectFactory : public ObjectFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectFactory( kernel::Controllers& controllers,
                            Model& model,
                            const StaticModel& staticModel,
                            const kernel::Time_ABC& simulation,
                            const kernel::Profile_ABC& profile,
                            actions::ActionsModel& actionsModel );
    virtual ~ObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Object_ABC* Create( const sword::ObjectCreation& message );

    virtual void RegisterAttributes( kernel::Object_ABC& result, const sword::ObjectAttributes& attributes ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectFactory( const ObjectFactory& );            //!< Copy constructor
    ObjectFactory& operator=( const ObjectFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const ObjectAttributesFactory attributesFactory_;
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    const kernel::Profile_ABC& profile_;
    actions::ActionsModel& actionsModel_;
    //@}
};

#endif // __ObjectFactory_h_
