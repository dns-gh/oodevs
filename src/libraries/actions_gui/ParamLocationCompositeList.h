// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ParamLocationCompositeList_h_
#define __ParamLocationCompositeList_h_

#include "ListParameter.h"

namespace kernel
{
    class ActionController;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamLocationCompositeList
    @brief  ParamLocationCompositeList
*/
// Created: LDC 2010-09-20
// =============================================================================
class ParamLocationCompositeList : public ListParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamLocationCompositeList( QObject* parent, const kernel::OrderParameter& parameter, const InterfaceBuilder_ABC& builder, kernel::ActionController& controller );
    virtual ~ParamLocationCompositeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamLocationCompositeList( const ParamLocationCompositeList& );            //!< Copy constructor
    ParamLocationCompositeList& operator=( const ParamLocationCompositeList& ); //!< Assignment operator
    //@}

protected:
    //! @name Types
    //@{
    virtual Param_ABC* CreateElement();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    const InterfaceBuilder_ABC& builder_;
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamLocationCompositeList_h_
