// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanFilter_h_
#define __UrbanFilter_h_

#include "LayerFilter_ABC.h"

namespace gui
{

// =============================================================================
/** @class  UrbanFilter
    @brief  UrbanFilter
*/
// Created: ABR 2011-02-28
// =============================================================================
class UrbanFilter : public LayerFilter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanFilter();
    virtual ~UrbanFilter();
    //@}

    //! @name Operations
    //@{
    virtual bool IsAllowed( const QString& typeName ) const;
    //@}
};

} // namespace gui

#endif // __UrbanFilter_h_
