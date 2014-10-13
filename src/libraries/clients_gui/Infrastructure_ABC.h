// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef CLIENTS_GUI_INFRASTRUCTURE_ABC_H__
#define CLIENTS_GUI_INFRASTRUCTURE_ABC_H__

#include "clients_kernel/UrbanExtensions.h"
#include "Drawable_ABC.h"

namespace kernel
{
    class InfrastructureType;
}

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;
}

namespace gui
{
// =============================================================================
/** @class  Infrastructure_ABC
    @brief  Infrastructure_ABC
*/
// Created: JSR 2010-08-19
// =============================================================================
class Infrastructure_ABC : public kernel::InfrastructureAttribute_ABC
                         , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Infrastructure_ABC() {}
    virtual ~Infrastructure_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual bool IsEnabled() const = 0;
    virtual const std::string& GetInvalidType() const = 0;
    virtual unsigned int GetThreshold() const = 0;
    virtual const kernel::InfrastructureType* GetType() const = 0;
    virtual void SetType( kernel::InfrastructureType* infrastructure ) = 0;
    //@}
};

}

#endif // CLIENTS_GUI_INFRASTRUCTURE_ABC_H__
