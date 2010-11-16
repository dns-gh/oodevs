// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamUrbanKnowledge_h_
#define __ParamUrbanKnowledge_h_

#include "EntityParameter.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"


namespace gui
{
    class TerrainObjectProxy;
}

namespace kernel
{
    class Entity_ABC;
    class UrbanKnowledgeConverter_ABC;
}

namespace actions
{
namespace gui
{

// =============================================================================
/** @class  ParamUrbanKnowledge
@brief  ParamUrbanKnowledge
*/
// Created: MGD 2010-11-16
// =============================================================================
class ParamUrbanKnowledge : public EntityParameter< kernel::UrbanKnowledge_ABC >
                          , public kernel::ContextMenuObserver_ABC< ::gui::TerrainObjectProxy >
{
public:
    //! @name Constructors/Destructor
    //@{
    ParamUrbanKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::UrbanKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, kernel::Controller& controller );
    ParamUrbanKnowledge( QObject* parent, const kernel::OrderParameter& parameter, kernel::UrbanKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const kernel::UrbanKnowledge_ABC& potential, kernel::Controller& controller );
    virtual ~ParamUrbanKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamUrbanKnowledge( const ParamUrbanKnowledge& );            //!< Copy constructor
    ParamUrbanKnowledge& operator=( const ParamUrbanKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const ::gui::TerrainObjectProxy& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    kernel::UrbanKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    //@}
};

}
}

#endif // __ParamUrbanKnowledge_h_