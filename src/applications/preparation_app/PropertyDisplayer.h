// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertyDisplayer_h_
#define __PropertyDisplayer_h_

#include "clients_gui/PropertyDisplayer.h"
#include "preparation/EnumTypes.h"

namespace kernel
{
    class InfrastructureType;
    class Moveable_ABC;
    class UrbanTemplateType;
    struct UrbanBlockColor;
}

class PopulationRepartition;

// =============================================================================
/** @class  PropertyDisplayer
    @brief  Property Displayer
*/
// Created: SBO 2006-10-27
// =============================================================================
class PropertyDisplayer : public gui::PropertyDisplayer
                        , public tools::Caller< Enum_PopulationAttitude >
                        , public tools::Caller< Enum_NbcState >
                        , public tools::Caller< geometry::Point2f >
                        , public tools::Caller< kernel::Moveable_ABC >
                        , public tools::Caller< PopulationRepartition >
                        , public tools::Caller< kernel::InfrastructureType >
                        , public tools::Caller< kernel::UrbanTemplateType >
                        , public tools::Caller< kernel::UrbanBlockColor >
                        , public tools::Caller< kernel::CriticalIntelligenceType >
                        , public tools::Caller< kernel::DisasterDirectory >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertyDisplayer( const kernel::CoordinateConverter_ABC& converter );
    virtual ~PropertyDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Call( const Enum_PopulationAttitude& value );
    virtual void Call( const Enum_NbcState & value );
    virtual void Call( const geometry::Point2f& value );
    virtual void Call( const kernel::Moveable_ABC& value );
    virtual void Call( const PopulationRepartition& value );
    virtual void Call( const kernel::InfrastructureType& value );
    virtual void Call( const kernel::UrbanTemplateType& value );
    virtual void Call( const kernel::UrbanBlockColor& value );
    virtual void Call( const kernel::CriticalIntelligenceType& value );
    virtual void Call( const kernel::DisasterDirectory& value );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __PropertyDisplayer_h_
