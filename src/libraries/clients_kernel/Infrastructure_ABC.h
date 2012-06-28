// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Infrastructure_ABC_h_
#define __Infrastructure_ABC_h_

#include "UrbanExtensions.h"

namespace kernel
{
    class GlTools_ABC;
    class InfrastructureType;
    class Viewport_ABC;

// =============================================================================
/** @class  Infrastructure_ABC
    @brief  Infrastructure_ABC
*/
// Created: JSR 2010-08-19
// =============================================================================
class Infrastructure_ABC : public InfrastructureAttribute_ABC
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
    virtual const InfrastructureType* GetType() const = 0;
    virtual void SetType( InfrastructureType* infrastructure ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const Viewport_ABC& viewport, const GlTools_ABC& tools ) const = 0;
    //@}
};

}

#endif // __Infrastructure_ABC_h_
