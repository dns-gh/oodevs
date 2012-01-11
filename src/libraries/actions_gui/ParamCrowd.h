// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamCrowd_h_
#define __ParamCrowd_h_

#include "EntityParameter.h"
#include "clients_kernel/Population_ABC.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamCrowd
    @brief  ParamCrowd
*/
// Created: FPO 2011-05-30
// =============================================================================
class ParamCrowd : public EntityParameter< kernel::Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamCrowd(  const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamCrowd();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}
};

    }
}

#endif // __ParamCrowd_h_
