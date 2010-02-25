// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFactory_h_
#define __ActionParameterFactory_h_

#include "ParameterFactory_ABC.h"

namespace Common
{
    class MsgMissionParameter;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ActionParameterFactory
    @brief  ActionParameterFactory
*/
// Created: SBO 2007-04-13
// =============================================================================
class ActionParameterFactory : public ParameterFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const Model& model, const StaticModel& staticModel
                                   , kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter, kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter
                                   , kernel::Controller& controller );
    virtual ~ActionParameterFactory();
    //@}

    //! @name Operations
    //@{
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const Common::MsgMissionParameter& message, const kernel::Entity_ABC& entity ) const;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterFactory( const ActionParameterFactory& );            //!< Copy constructor
    ActionParameterFactory& operator=( const ActionParameterFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const Model& model_;
    const StaticModel& staticModel_;
    kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter_;
    kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;
    kernel::Controller& controller_;
    //@}
};

#endif // __ActionParameterFactory_h_
