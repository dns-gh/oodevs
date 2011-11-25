// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
             ParamAutomat( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller );
             ParamAutomat( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Automat_ABC& entity, kernel::Controller& controller );
    virtual ~ParamAutomat();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    //@}
};

    }
}

#endif // __ParamAutomat_h_
