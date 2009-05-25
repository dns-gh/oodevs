// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPolygon_h_
#define __ParamPolygon_h_

#include "ParamLocation.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamPolygon
    @brief  ParamPolygon
*/
// Created: SBO 2007-03-14
// =============================================================================
class ParamPolygon : public ParamLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPolygon( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamPolygon();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}
};

    }
}

#endif // __ParamPolygon_h_
