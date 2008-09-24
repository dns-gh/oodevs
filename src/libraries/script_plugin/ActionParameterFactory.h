// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFactory_h_
#define __ActionParameterFactory_h_

#include "actions/ParameterFactory_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class FormationLevels;
    class ObjectTypes;
    class AtlasNatures;
}

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
    class Model;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  ActionParameterFactory
    @brief  ActionParameterFactory
*/
// Created: AGE 2008-07-16
// =============================================================================
class ActionParameterFactory : public actions::ParameterFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const dispatcher::Model& model, const tools::ExerciseConfig& config );
    virtual ~ActionParameterFactory();
    //@}

    //! @name Operations
    //@{
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterFactory( const ActionParameterFactory& );            //!< Copy constructor
    ActionParameterFactory& operator=( const ActionParameterFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    struct Adapters;
    struct AgentConverter;
    struct ObjectConverter;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< Adapters >                adapters_;
    std::auto_ptr< AgentConverter >          agentsKnowledges_;
    std::auto_ptr< ObjectConverter >         objectsKnowledges_;
    std::auto_ptr< kernel::FormationLevels > formations_;
    std::auto_ptr< kernel::AtlasNatures >    natures_;
    std::auto_ptr< kernel::ObjectTypes >     objects_;
    //@}
};

}
}

#endif // __ActionParameterFactory_h_
