// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPathList_h_
#define __ParamPathList_h_

#include "ListParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class ActionController;
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
/** @class  ParamPathList
    @brief  ParamPathList
*/
// Created: SBO 2006-06-28
// =============================================================================
class ParamPathList : public ListParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPathList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& agent, kernel::ActionController& controller );
    virtual ~ParamPathList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamPathList( const ParamPathList& );            //!< Copy constructor
    ParamPathList& operator=( const ParamPathList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::OrderParameter& parameter_;
    ::gui::ParametersLayer& layer_;
    const kernel::Entity_ABC& entity_;
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamPathList_h_
