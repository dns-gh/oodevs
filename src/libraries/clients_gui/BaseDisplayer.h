// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BaseDisplayer_h_
#define __BaseDisplayer_h_

#include "clients_kernel/Displayer_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
    class Object_ABC;
    class Team_ABC;
    class DotationType;
    class EquipmentType;
    class Population_ABC;
    class NBCAgent;
    class ComponentType;
    class BreakdownType;
    class Mission;
    class CoordinateConverter_ABC;
    class PopulationConcentration_ABC;
    class Entity_ABC;
    class AgentType;
    class AutomatType;
    class PopulationType;
    class ObjectType;
}

namespace gui
{

// =============================================================================
/** @class  BaseDisplayer
    @brief  BaseDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class BaseDisplayer : public kernel::Displayer_ABC
                    , public kernel::Caller< bool >
                    , public kernel::Caller< QTime >
                    , public kernel::Caller< kernel::Population_ABC>
                    , public kernel::Caller< kernel::PopulationConcentration_ABC >
                    , public kernel::Caller< kernel::Agent_ABC >
                    , public kernel::Caller< kernel::Automat_ABC >
                    , public kernel::Caller< kernel::Object_ABC >
                    , public kernel::Caller< kernel::Team_ABC >
                    , public kernel::Caller< kernel::Entity_ABC >
                    , public kernel::Caller< kernel::DotationType >
                    , public kernel::Caller< kernel::EquipmentType >
                    , public kernel::Caller< kernel::NBCAgent >
                    , public kernel::Caller< kernel::ComponentType >
                    , public kernel::Caller< kernel::BreakdownType >
                    , public kernel::Caller< kernel::Mission >
                    , public kernel::Caller< kernel::AgentType >
                    , public kernel::Caller< kernel::AutomatType >
                    , public kernel::Caller< kernel::PopulationType >
                    , public kernel::Caller< kernel::ObjectType >
{

public:
    //! @name Constructors/Destructor
    //@{
             BaseDisplayer();
    virtual ~BaseDisplayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    BaseDisplayer( const BaseDisplayer& );            //!< Copy constructor
    BaseDisplayer& operator=( const BaseDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Call( const bool& value );
    virtual void Call( const QTime& value );
    virtual void Call( const kernel::Agent_ABC& value );
    virtual void Call( const kernel::Automat_ABC& value );
    virtual void Call( const kernel::Population_ABC& value );
    virtual void Call( const kernel::PopulationConcentration_ABC& value );
    virtual void Call( const kernel::Object_ABC& value );
    virtual void Call( const kernel::Team_ABC& value );
    virtual void Call( const kernel::Entity_ABC& value );
    virtual void Call( const kernel::DotationType& value );
    virtual void Call( const kernel::EquipmentType& value );
    virtual void Call( const kernel::NBCAgent& value );
    virtual void Call( const kernel::ComponentType& value );
    virtual void Call( const kernel::BreakdownType& value );
    virtual void Call( const kernel::Mission& value );
    virtual void Call( const kernel::AgentType& value );
    virtual void Call( const kernel::AutomatType& value );
    virtual void Call( const kernel::PopulationType& value );
    virtual void Call( const kernel::ObjectType& value );
    //@}
};

}

#endif // __BaseDisplayer_h_
