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

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "protocol/SimulationSenders.h"
#include "ENT/ENT_Enums.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class UnitCreation;
    class UnitAttributes;
}

namespace kernel
{
    class Controller;
    class AgentType;
    class Equipments_ABC;
}

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public gui::EntityImplementation< kernel::Agent_ABC >
            , public kernel::Extension_ABC
            , public kernel::Displayable_ABC
            , public tools::Observer_ABC
            , public tools::ElementObserver_ABC< kernel::Equipments_ABC >
            , public gui::Drawable_ABC
            , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
            Agent( const sword::UnitCreation& message,
                   kernel::Controller& controller,
                   const tools::Resolver_ABC< kernel::AgentType >& resolver,
                   const T_CanBeRenamedFunctor& canBeRenamedFunctor );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    virtual const kernel::AgentType& GetType() const;

    virtual void NotifyUpdated( const kernel::Equipments_ABC& equipments );
    virtual void DoUpdate( const sword::UnitAttributes& attributes );

    bool ShouldDisplayStaticSymbol() const;
    unsigned int GetSensorsDirection() const;
    unsigned int GetMoveDirection() const;
    float GetSymbolWidth() const;
    float GetSymbolDepth() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    void CreateDictionary();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AgentType& type_;
    kernel::Controller& controller_;
    mutable std::string symbol_;
    mutable std::string moveSymbol_;
    mutable std::string staticSymbol_;
    std::string level_;
    mutable bool initialized_;
    float weight_;
    float speed_;
    unsigned int direction_;
    unsigned int sensorsDirection_;
    //@}
};

#endif // __Agent_h_
