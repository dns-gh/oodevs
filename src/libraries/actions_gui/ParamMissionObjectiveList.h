// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamMissionObjectiveList_h_
#define __ParamMissionObjectiveList_h_

#include "ListParameter.h"

namespace kernel
{
    class ActionController;
    class CoordinateConverter_ABC;
    class OrderParameter;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamMissionObjectiveList
    @brief  ParamMissionObjectiveList
*/
// Created: SBO 2007-05-14
// =============================================================================
class ParamMissionObjectiveList : public ListParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamMissionObjectiveList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const QDateTime& currentDate, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller );
    virtual ~ParamMissionObjectiveList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement();
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    const QDateTime currentDate_;
    ::gui::ParametersLayer& layer_;
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamMissionObjectiveList_h_
