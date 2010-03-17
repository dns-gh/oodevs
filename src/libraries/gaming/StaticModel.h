// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    class DetectionMap;
    class Controllers;
    class CoordinateConverter_ABC;
    class CoordinateSystems;
    class FormationLevels;
    class AtlasNatures;
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
class ScoreDefinitions;
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
             StaticModel( kernel::Controllers& controllers, const RcEntityResolver_ABC& rcResolver, const Simulation& simu );
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
    kernel::FormationLevels&         levels_;
    kernel::AtlasNatures&            atlasNatures_;
    ReportFactory&                   reportFactory_;
    gui::DrawingTypes&               drawings_;
    indicators::Primitives&          indicators_;
    indicators::GaugeTypes&          gaugeTypes_;
    ScoreDefinitions&                scores_;
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
