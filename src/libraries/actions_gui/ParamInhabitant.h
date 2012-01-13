// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamInhabitant_h_
#define __ParamInhabitant_h_

#include "EntityParameter.h"
#include "clients_kernel/Inhabitant_ABC.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamInhabitant
    @brief  ParamInhabitant
*/
// Created: FPO 2011-05-30
// =============================================================================
class ParamInhabitant : public EntityParameter< kernel::Inhabitant_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamInhabitant( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamInhabitant();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}
};

    }
}

#endif // __ParamInhabitant_h_
