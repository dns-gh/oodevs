// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __InterfaceBuilder_ABC_h_
#define __InterfaceBuilder_ABC_h_

#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <QtCore/QDateTime>
#pragma warning( pop )

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class ObjectKnowledgeConverter_ABC;
    class StaticModel;
    class TacticalLine_ABC;
    class OrderParameter;
    class OrderType;
}

namespace gui
{
    class ParametersLayer;
}

namespace tools
{
    class ExerciseConfig;
}

namespace actions {
namespace gui {

    class MissionInterface;
    class Param_ABC;
    class ParamInterface_ABC;

// =============================================================================
/** @class  InterfaceBuilder_ABC
    @brief  InterfaceBuilder_ABC
*/
// Created: LDC 2010-08-18
// =============================================================================
class InterfaceBuilder_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             InterfaceBuilder_ABC() {}
    virtual ~InterfaceBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void BuildAll( actions::gui::MissionInterface& missionInterface, const kernel::OrderType& order ) = 0;
    virtual actions::gui::Param_ABC& BuildOne( const kernel::OrderParameter& parameter, bool isRegistered = true ) const = 0;
    //@}

    //! @name Getter
    //@{
    virtual QObject* GetParentObject() const = 0;
    virtual ParamInterface_ABC& GetParamInterface() const = 0;
    virtual ::gui::ParametersLayer& GetParameterLayer() const = 0;
    virtual kernel::Controllers& GetControllers() const = 0;
    virtual kernel::AgentKnowledgeConverter_ABC* GetAgentKnowledgeConverter() const = 0;
    virtual kernel::ObjectKnowledgeConverter_ABC* GetObjectKnowledgeConverter() const = 0;
    virtual tools::Resolver< kernel::TacticalLine_ABC >* GetTacticalLineResolver() const = 0;
    virtual const QDateTime GetCurrentDateTime() const = 0;
    virtual const kernel::StaticModel& GetStaticModel() const = 0;
    virtual const tools::ExerciseConfig& GetConfig() const = 0;
    //@}

    //! @name Setter
    //@{
    virtual void SetParentObject( QObject* parent ) = 0;
    virtual void SetParamInterface( ParamInterface_ABC& paramInterface ) = 0;
    //@}
};

}
}

#endif // __InterfaceBuilder_ABC_h_
