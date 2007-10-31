// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFactory_h_
#define __ActionParameterFactory_h_

#include "ActionParameterFactory_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

class Model;
class StaticModel;
class AgentKnowledgeConverter_ABC;
class ObjectKnowledgeConverter_ABC;

// =============================================================================
/** @class  ActionParameterFactory
    @brief  ActionParameterFactory
*/
// Created: SBO 2007-04-13
// =============================================================================
class ActionParameterFactory : public ActionParameterFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterFactory( const kernel::CoordinateConverter_ABC& converter, const Model& model, const StaticModel& staticModel
                                   , AgentKnowledgeConverter_ABC& agentKnowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter );
    virtual ~ActionParameterFactory();
    //@}

    //! @name Operations
    //@{
    virtual ActionParameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_MissionParameter& asn, const kernel::Entity_ABC& entity ) const;
    virtual ActionParameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_Line& line1, const ASN1T_Line& line2 ) const;
    virtual ActionParameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_LimasOrder& asn ) const;
    virtual ActionParameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_IntelligenceList& asn ) const;
    virtual ActionParameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const ASN1T_Heading& asn ) const;
    virtual ActionParameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterFactory( const ActionParameterFactory& );            //!< Copy constructor
    ActionParameterFactory& operator=( const ActionParameterFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const Model& model_;
    const StaticModel& staticModel_;
    AgentKnowledgeConverter_ABC& agentKnowledgeConverter_;
    ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;
    //@}
};

#endif // __ActionParameterFactory_h_
