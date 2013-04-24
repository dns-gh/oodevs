// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesTableDisplayer_h_
#define __PropertiesTableDisplayer_h_

#include "clients_gui/TableItemDisplayer.h"
#include "preparation/EnumTypes.h"

namespace kernel
{
    class InfrastructureType;
    class MaterialCompositionType;
    class Moveable_ABC;
    class RoofShapeType;
    class UrbanTemplateType;
    struct UrbanBlockColor;
}

class DotationsItem;
class PopulationRepartition;

// =============================================================================
/** @class  PropertiesTableDisplayer
    @brief  PropertiesTableDisplayer
*/
// Created: SBO 2006-10-27
// =============================================================================
class PropertiesTableDisplayer : public gui::TableItemDisplayer
                               , public tools::Caller< Enum_PopulationAttitude >
                               , public tools::Caller< Enum_DemolitionTargetType >
                               , public tools::Caller< Enum_NbcState >
                               , public tools::Caller< DotationsItem >
                               , public tools::Caller< geometry::Point2f >
                               , public tools::Caller< kernel::Moveable_ABC >
                               , public tools::Caller< PopulationRepartition >
                               , public tools::Caller< kernel::InfrastructureType >
                               , public tools::Caller< kernel::UrbanTemplateType >
                               , public tools::Caller< kernel::MaterialCompositionType >
                               , public tools::Caller< kernel::RoofShapeType >
                               , public tools::Caller< kernel::UrbanBlockColor >
                               , public tools::Caller< kernel::CriticalIntelligence >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertiesTableDisplayer( const kernel::CoordinateConverter_ABC& converter );
    virtual ~PropertiesTableDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Call( const Enum_PopulationAttitude& value );
    virtual void Call( const Enum_DemolitionTargetType & value );
    virtual void Call( const Enum_NbcState & value );
    virtual void Call( const DotationsItem& value );
    virtual void Call( const geometry::Point2f& value );
    virtual void Call( const kernel::Moveable_ABC& value );
    virtual void Call( const PopulationRepartition& value );
    virtual void Call( const kernel::InfrastructureType& value );
    virtual void Call( const kernel::UrbanTemplateType& value );
    virtual void Call( const kernel::MaterialCompositionType& value );
    virtual void Call( const kernel::RoofShapeType& value );
    virtual void Call( const kernel::UrbanBlockColor& value );
    virtual void Call( const kernel::CriticalIntelligence& value );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __PropertiesTableDisplayer_h_