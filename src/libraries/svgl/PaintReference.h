// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PaintReference_h_
#define __PaintReference_h_

#include "Paint_ABC.h"

namespace svg
{

// =============================================================================
/** @class  PaintReference
    @brief  PaintReference
*/
// Created: AGE 2006-08-15
// =============================================================================
class PaintReference : public Paint_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PaintReference( const std::string& reference );
    virtual ~PaintReference();
    //@}

    //! @name Operations
    //@{
    virtual bool Setup( References_ABC& references, float opacity ) const;
    virtual PaintReference& Clone() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PaintReference& operator=( const PaintReference& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string target_;
    //@}
};

}

#endif // __PaintReference_h_
