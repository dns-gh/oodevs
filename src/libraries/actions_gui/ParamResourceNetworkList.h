// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamResourceNetworkList_h_
#define __ParamResourceNetworkList_h_

#include "ListParameter.h"

namespace kernel
{
    class Controller;
}

namespace actions
{
    namespace gui
    {
// =============================================================================
/** @class  ParamResourceNetworkList
    @brief  ParamResourceNetworkList
*/
// Created: LDC 2011-05-11
// =============================================================================
class ParamResourceNetworkList : public ListParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamResourceNetworkList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamResourceNetworkList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    kernel::Controller& controller_;
    //@}
};
    }
}

#endif // __ParamResourceNetworkList_h_
