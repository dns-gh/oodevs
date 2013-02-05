// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Layer_ABC_h_
#define __Layer_ABC_h_

#include <graphics/MapLayer_ABC.h>
#include <boost/noncopyable.hpp>
#include "clients_kernel/Selectable_ABC.h"

namespace gui
{

// =============================================================================
/** @class  Layer_ABC
    @brief  Layer_ABC
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer_ABC : public MapLayer_ABC
                , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Layer_ABC() {}
    virtual ~Layer_ABC() {}
    //@}

    //! @name Types
    //@{
    typedef std::vector< Layer_ABC* >                                       T_Layers;
    typedef std::map< Layer_ABC*, kernel::Selectable_ABC::T_Selectables >   T_LayerElements;
    //@}

    //! @name Abstract methods
    //@{
    virtual void ExtractElements( T_LayerElements& extractedElement, const geometry::Point2f& point ) = 0;
    //@}
};

}

#endif // __Layer_ABC_h_
