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

#include "game_asn/Simulation.h"
#include "ListParameter.h"

namespace kernel
{
    class ActionController;
    class ObjectTypes;
    class CoordinateConverter_ABC;
    class OrderParameter;
}

namespace gui
{
    class ParametersLayer;
}

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
             ParamObstacleList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::ObjectTypes& objectTypes, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller );
    virtual ~ParamObstacleList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ActionParameterContainer_ABC& action ) const;
    //@}
    
private:
    //! @name Copy/Assignment
    //@{
    ParamObstacleList( const ParamObstacleList& );            //!< Copy constructor
    ParamObstacleList& operator=( const ParamObstacleList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    virtual Param_ABC* CreateElement();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::ObjectTypes& objectTypes_;
    gui::ParametersLayer& layer_;
    unsigned int count_;
    //@}
};

#endif // __ParamObstacleList_h_
