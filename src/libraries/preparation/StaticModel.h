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

namespace kernel
{
    class AgentTypes;
    class Controllers;
    class CoordinateConverter_ABC;
    class CoordinateSystems;
    class DetectionMap;
    class ExtensionTypes;
    class FormationLevels;
    class ObjectTypes;
}

namespace tools
{
    class ExerciseConfig;
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

class IntelligenceKarmas;
class SuccessFactorActionTypes;
class TeamKarmas;

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
    explicit StaticModel( kernel::Controllers& controllers );
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
    kernel::Controllers& controllers_;
    kernel::CoordinateSystems&       coordinateSystems_;
    kernel::CoordinateConverter_ABC& coordinateConverter_;
    kernel::DetectionMap&            detection_;
    kernel::AgentTypes&              types_;
    kernel::ObjectTypes&             objectTypes_;
    kernel::FormationLevels&         levels_;
    kernel::ExtensionTypes&          extensions_;
    TeamKarmas&                      teamKarmas_;
    IntelligenceKarmas&              intelligenceKarmas_;
    gui::DrawingTypes&               drawings_;
    indicators::Primitives&          indicators_;
    indicators::GaugeTypes&          gaugeTypes_;
    SuccessFactorActionTypes&        successFactorActionTypes_;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StaticModel( const StaticModel& );            //!< Copy constructor
    StaticModel& operator=( const StaticModel& ); //!< Assignment operator
    //@}
};

#endif // __StaticModel_h_
