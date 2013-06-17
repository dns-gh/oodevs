// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAutomat_h_
#define __ParamAutomat_h_

#include "clients_kernel/Automat_ABC.h"
#include "EntityParameter.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamAutomat
    @brief  ParamAutomat
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAutomat : public EntityParameter< kernel::Automat_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamAutomat( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamAutomat();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::Automat& param );
    //@}
};

    }
}

#endif // __ParamAutomat_h_
