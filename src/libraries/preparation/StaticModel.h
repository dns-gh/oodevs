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

#include "clients_kernel/StaticModel.h"

namespace kernel
{
    class Controllers;
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

namespace gui
{
    class DrawingTypes;
}

namespace preparation
{
    class LogisticLevel;
}

class SuccessFactorActionTypes;
class TeamKarmas;

// =============================================================================
/** @class  StaticModel
    @brief  StaticModel
*/
// Created: AGE 2006-08-01
// =============================================================================
class StaticModel : public kernel::StaticModel
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
    kernel::Controllers&        controllers_;
    indicators::Primitives&     indicators_; // Move to kernel::StaticModel
    indicators::GaugeTypes&     gaugeTypes_; // Move to kernel::StaticModel
    gui::DrawingTypes&          drawings_;   // Move to kernel::StaticModel
    SuccessFactorActionTypes&   successFactorActionTypes_;
    preparation::LogisticLevel& logisticLevels_;
    TeamKarmas&                 teamKarmas_;
    //@}
};

#endif // __StaticModel_h_
