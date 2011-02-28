// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LayerFilter_ABC_h_
#define __LayerFilter_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  LayerFilter_ABC
    @brief  LayerFilter_ABC
*/
// Created: ABR 2011-02-28
// =============================================================================
class LayerFilter_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LayerFilter_ABC() {}
    virtual ~LayerFilter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsAllowed( const QString& ) const = 0;
    //@}
};

} // namespace gui

#endif // __LayerFilter_ABC_h_
