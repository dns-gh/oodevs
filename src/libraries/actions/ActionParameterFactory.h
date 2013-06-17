// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __actions_ActionParameterFactory_h_
#define __actions_ActionParameterFactory_h_

#include "ParameterFactory_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class ObjectKnowledgeConverter_ABC;
    class StaticModel;
}

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }

// =============================================================================
/** @class  ActionParameterFactory
    @brief  ActionParameterFactory
*/
// Created: SBO 2007-04-13
// =============================================================================
class ActionParameterFactory : public actions::ParameterFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel
                                   , kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter, kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter
                                   , kernel::Controller& controller );
    virtual ~ActionParameterFactory();
    //@}

    //! @name Operations
    //@{
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const sword::MissionParameter& message, const kernel::Entity_ABC* entity ) const;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const sword::MissionParameter_Value& message, const kernel::Entity_ABC* entity, bool nullValue = false ) const;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateListParameter( xml::xistream& xis, actions::parameters::ParameterList& list, const std::string& parent ) const;
    void CreateListParameter( xml::xistream& xis, actions::parameters::ParameterList& list, const kernel::Entity_ABC& entity, const std::string& parent ) const;
    bool DoCreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const std::string& type, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    bool DoCreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity, const std::string& type, std::auto_ptr< actions::Parameter_ABC >& param ) const;
    void CreateLocationComposite( const std::string &type, xml::xistream& xis, const kernel::OrderParameter& parameter, const kernel::Entity_ABC& entity, std::auto_ptr< actions::Parameter_ABC >& param ) const;

    actions::Parameter_ABC* DoCreateParameter( const kernel::OrderParameter& parameter, const sword::MissionParameter& message, const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::EntityResolver_ABC& entities_;
    const kernel::StaticModel& staticModel_;
    kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter_;
    kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;
    kernel::Controller& controller_;
    //@}
};

}

#endif // __actions_ActionParameterFactory_h_
