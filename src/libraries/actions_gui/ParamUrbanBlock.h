// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamUrbanBlock_h_
#define __ParamUrbanBlock_h_

#include "EntityParameter.h"
#include "clients_kernel/Object_ABC.h"

namespace actions
{
namespace gui
{
// =============================================================================
/** @class  ParamUrbanBlock
    @brief  Param urban block
*/
// Created: LGY 2011-02-25
// =============================================================================
class ParamUrbanBlock : public EntityParameter< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamUrbanBlock( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamUrbanBlock();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}
};

}
}

#endif // __ParamUrbanBlock_h_
