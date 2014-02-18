// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorCurrent_h_
#define __ColorCurrent_h_

#include "Paint_ABC.h"

namespace svg
{

// =============================================================================
/** @class  ColorCurrent
    @brief  ColorCurrent
*/
// Created: AGE 2006-08-15
// =============================================================================
class ColorCurrent : public Paint_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorCurrent();
    virtual ~ColorCurrent();
    //@}

    //! @name Operations
    //@{
    virtual bool Setup( References_ABC& , float opacity ) const;
    virtual ColorCurrent& Clone() const;
    virtual void ReachTop( const RenderingContext_ABC& context );
    virtual void LeaveTop( const RenderingContext_ABC& context );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColorCurrent( const ColorCurrent& );            //!< Copy constructor
    ColorCurrent& operator=( const ColorCurrent& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const RenderingContext_ABC* context_;
    //@}
};

}

#endif // __ColorCurrent_h_
