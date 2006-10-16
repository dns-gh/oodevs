// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPopulationKnowledge_h_
#define __ParamPopulationKnowledge_h_

#include "EntityParameter.h"

namespace kernel
{
    class Population_ABC;
    class Entity_ABC;
}

class PopulationKnowledge_ABC;
class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ParamPopulationKnowledge
    @brief  ParamPopulationKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamPopulationKnowledge : public EntityParameter< PopulationKnowledge_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPopulationKnowledge( QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent );
    virtual ~ParamPopulationKnowledge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamPopulationKnowledge( const ParamPopulationKnowledge& );            //!< Copy constructor
    ParamPopulationKnowledge& operator=( const ParamPopulationKnowledge& ); //!< Assignement operator
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    //@}

    //! @name Member data
    //@{
    AgentKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    //@}
};

#endif // __ParamPopulationKnowledge_h_
