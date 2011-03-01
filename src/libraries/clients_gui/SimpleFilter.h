// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SimpleFilter_h_
#define __SimpleFilter_h_

#include "LayerFilter_ABC.h"

namespace gui
{

// =============================================================================
/** @class  SimpleFilter
@brief  SimpleFilter
*/
// Created: ABR 2011-02-28
// =============================================================================
class SimpleFilter : public LayerFilter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    SimpleFilter();
    virtual ~SimpleFilter();
    //@}

    //! @name Operations
    //@{
    virtual bool IsAllowed( const QString& typeName ) const;
    //@}
};

} // namespace gui

#endif // __UrbanFilter_h_
