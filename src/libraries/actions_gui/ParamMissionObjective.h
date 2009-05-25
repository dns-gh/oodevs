// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamMissionObjective_h_
#define __ParamMissionObjective_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    class Parameter_ABC;

    namespace gui
    {
        class ParamLocation;
        class ParamDateTime;

// =============================================================================
/** @class  ParamMissionObjective
    @brief  ParamMissionObjective
*/
// Created: SBO 2007-05-14
// =============================================================================
class ParamMissionObjective : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamMissionObjective( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const QDateTime& current, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamMissionObjective();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamMissionObjective( const ParamMissionObjective& );            //!< Copy constructor
    ParamMissionObjective& operator=( const ParamMissionObjective& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    ::gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    const QDateTime currentDate_;
    ParamLocation* location_;
    ParamDateTime* schedule_;
    bool optional_;
    //@}
};

    }
}

#endif // __ParamMissionObjective_h_
