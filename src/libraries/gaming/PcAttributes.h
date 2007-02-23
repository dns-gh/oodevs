// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PcAttributes_h_
#define __PcAttributes_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel {
    class Entity_ABC;
}

// =============================================================================
/** @class  PcAttributes
    @brief  PcAttributes
*/
// Created: AGE 2006-11-29
// =============================================================================
class PcAttributes : public kernel::Extension_ABC
                   , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PcAttributes( kernel::Entity_ABC& holder );
    virtual ~PcAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PcAttributes( const PcAttributes& );            //!< Copy constructor
    PcAttributes& operator=( const PcAttributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& holder_;
    //@}
};

#endif // __PcAttributes_h_
