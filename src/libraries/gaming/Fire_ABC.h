// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Fire_ABC_h_
#define __Fire_ABC_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

namespace sword
{
    class StopCrowdFire;
    class StopUnitFire;
}

// =============================================================================
/** @class  Fire_ABC
    @brief  Fire_ABC
*/
// Created: AGE 2006-03-10
// =============================================================================
class Fire_ABC : public kernel::Updatable_ABC< sword::StopUnitFire >
               , public kernel::Updatable_ABC< sword::StopCrowdFire >
               , public gui::Drawable_ABC
               , boost::noncopyable
{
public:
    explicit Fire_ABC( kernel::Entity_ABC& origin );
    virtual ~Fire_ABC();

    virtual void DoUpdate( const sword::StopUnitFire& message );
    virtual void DoUpdate( const sword::StopCrowdFire& message );

protected:
    kernel::Entity_ABC& origin_;
};

#endif // __Fire_ABC_h_
