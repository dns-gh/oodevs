// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLocationList_h_
#define __ParamLocationList_h_

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
/** @class  ParamLocationList
    @brief  ParamLocationList
*/
// Created: AGE 2006-04-03
// =============================================================================
class ParamLocationList : public ListParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    ParamLocationList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller );
    virtual ~ParamLocationList();

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamLocationList( const ParamLocationList& );            //!< Copy constructor
    ParamLocationList& operator=( const ParamLocationList& ); //!< Assignement operator
    //@}

protected:
    //! @name Types
    //@{
    virtual Param_ABC* CreateElement();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::OrderParameter& parameter_;
    ::gui::ParametersLayer& layer_;
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamLocationList_h_
