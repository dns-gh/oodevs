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
    virtual unsigned int GetThreshold() const = 0;
    //@}

	//! @name Operations
    //@{
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Infrastructure_ABC( const Infrastructure_ABC& );            //!< Copy constructor
    Infrastructure_ABC& operator=( const Infrastructure_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __Infrastructure_ABC_h_
