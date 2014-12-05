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

namespace gui
{
    class DrawingTypes;
}

namespace tools
{
    class ExerciseConfig;
}

class RcEntityResolver_ABC;
class ReportFactory;

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
             StaticModel( kernel::Controllers& controllers, const RcEntityResolver_ABC& rcResolver );
    virtual ~StaticModel();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const tools::ExerciseConfig& config );
    virtual void Purge();
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controllers&     controllers_;
    indicators::Primitives&  indicators_; // Move to kernel::StaticModel
    indicators::GaugeTypes&  gaugeTypes_; // Move to kernel::StaticModel
    gui::DrawingTypes&       drawings_;   // Move to kernel::StaticModel
    ReportFactory&           reportFactory_;
    //@}
};

#endif // __StaticModel_h_
