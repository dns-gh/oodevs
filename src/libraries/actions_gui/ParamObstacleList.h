// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamObstacleList_h_
#define __ParamObstacleList_h_

#include "ListParameter.h"

namespace kernel
{
    class ActionController;
    class ObjectTypes;
    class CoordinateConverter_ABC;
    class OrderParameter;
    class Controller;
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
/** @class  ParamObstacleList
    @brief  ParamObstacleList
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacleList : public ListParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacleList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::ObjectTypes& objectTypes, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamObstacleList();
    //@}

private:
    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Controller& controller_;
    const kernel::ObjectTypes& objectTypes_;
    ::gui::ParametersLayer& layer_;
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamObstacleList_h_
