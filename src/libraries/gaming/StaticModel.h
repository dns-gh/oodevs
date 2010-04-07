// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StaticModel_h_
#define __StaticModel_h_


namespace urban
{
    class StaticModel;
}
namespace kernel
{
    class AgentTypes;
    class ObjectTypes;
    class MagicActionType;
    class DetectionMap;
    class Controllers;
    class CoordinateConverter_ABC;
    class CoordinateSystems;
    class FormationLevels;
    class AtlasNatures;
    class Time_ABC;
}

namespace gui
{
    class DrawingTypes;
}

namespace indicators
{
    class GaugeTypes;
    class Primitives;
}

namespace tools
{
    class ExerciseConfig;
}

class RcEntityResolver_ABC;
class ReportFactory;
class Simulation;
class UrbanModel;

// =============================================================================
/** @class  StaticModel
    @brief  StaticModel
*/
// Created: AGE 2006-08-01
// =============================================================================
class StaticModel
{

public:
    //! @name Constructors/Destructor
    //@{
             StaticModel( kernel::Controllers& controllers, const RcEntityResolver_ABC& rcResolver, const kernel::Time_ABC& simu );
    virtual ~StaticModel();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controllers&             controllers_;
    kernel::CoordinateSystems&       coordinateSystems_;
    kernel::CoordinateConverter_ABC& coordinateConverter_;
    kernel::DetectionMap&            detection_;
    kernel::AgentTypes&              types_;
    kernel::ObjectTypes&             objectTypes_;
    kernel::MagicActionType&         magicActionType_;
    kernel::FormationLevels&         levels_;
    kernel::AtlasNatures&            atlasNatures_;
    ReportFactory&                   reportFactory_;
    gui::DrawingTypes&               drawings_;
    indicators::Primitives&          indicators_;
    indicators::GaugeTypes&          gaugeTypes_;
    urban::StaticModel&              urbanTypes_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StaticModel( const StaticModel& );            //!< Copy constructor
    StaticModel& operator=( const StaticModel& ); //!< Assignement operator
    //@}
};

#endif // __StaticModel_h_
