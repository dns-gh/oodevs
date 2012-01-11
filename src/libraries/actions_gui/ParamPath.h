// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPath_h_
#define __ParamPath_h_

#include "ParamLocation_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

namespace actions
{
    namespace parameters
    {
        class Path;
    }

    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamPath
    @brief  ParamPath
*/
// Created: APE 2004-03-25
// =============================================================================
class ParamPath : public ParamLocation_ABC< actions::parameters::Path >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamPath( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamPath();
    //@}

protected:
    //! @name Param_ABC
    //@{
    virtual void OnMenuClick();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    //@}
};

    }
}

#endif // __ParamPath_h_
