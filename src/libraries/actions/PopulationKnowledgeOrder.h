// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ActionParameterPopulationKnowledgeOrder_h_
#define __ActionParameterPopulationKnowledgeOrder_h_

#include "PopulationKnowledge.h"

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  PopulationKnowledgeOrder
    @brief  Population knowledge order
*/
// Created: LGY 2011-07-07
// =============================================================================
class PopulationKnowledgeOrder : public PopulationKnowledge
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgeOrder( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                       kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~PopulationKnowledgeOrder();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool CheckKnowledgeValidity() const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::AgentKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& owner_;
    bool optional_;
    const kernel::Population_ABC* pPopulation_;
    //@}
};
}
}
#endif // __ActionParameterPopulationKnowledgeOrder_h_
