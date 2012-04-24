// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLocation_h_
#define __ParamLocation_h_

#include "ParamLocationComposite.h"
#include "InterfaceBuilder_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class ParametersLayer;
    class LocationCreator;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamLocation
    @brief  ParamLocation
*/
// Created: ABR 2012-01-09
// =============================================================================
class ParamLocation : public ParamLocationComposite
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamLocation( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamLocation();
    //@}

    //! @name Operations
    //@{
    void SetShapeFilter( bool point, bool line, bool polygon, bool circle, bool rectangle );
    //@}
};

    }
}

#endif // __ParamLocation_h_
