// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FillRule_h_
#define __FillRule_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  FillRule
    @brief  FillRule
*/
// Created: AGE 2006-08-17
// =============================================================================
class FillRule : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FillRule( const std::string& name = "" );
    virtual ~FillRule();
    //@}

    //! @name Operations
    //@{
    unsigned int GetType() const;
    virtual FillRule& Clone() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int type_;
    //@}
};

}

#endif // __FillRule_h_
