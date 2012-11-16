// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "protocol/SimulationSenders.h"
#include "ENT/ENT_Enums_Gen.h"

namespace sword
{
    class UnitCreation;
}

namespace kernel
{
    class Controller;
    class AgentType;
}

class Equipments; 

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public kernel::EntityImplementation< kernel::Agent_ABC >
            , public kernel::Extension_ABC
            , public kernel::Drawable_ABC
            , public kernel::Displayable_ABC
            , public tools::Observer_ABC
            , public tools::ElementObserver_ABC< Equipments >
{
public:
    //! @name Constructors/Destructor
    //@{
            Agent( const sword::UnitCreation& message,
                    kernel::Controller& controller,
                    const tools::Resolver_ABC< kernel::AgentType >& resolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    virtual const kernel::AgentType& GetType() const;
    
    virtual QString GetShortName() const;
    virtual QString GetName() const;

    virtual void NotifyUpdated( const Equipments& equipments );
    //@}

private:
    //! @name Helpers
    //@{
    void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void CreateDictionary( kernel::Controller& controller );
    //@}

private:
    //! @name Member data
    //@{
    kernel::AgentType& type_;
    kernel::Controller& controller_;
    mutable std::string symbol_;
    std::string level_;
    mutable bool initialized_;
    float weight_;
    //@}
};

#endif // __Agent_h_
