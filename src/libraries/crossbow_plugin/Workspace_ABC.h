// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Workspace_ABC_h_
#define __Workspace_ABC_h_

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  Workspace_ABC
    @brief  Workspace base class: designed for COM initialization abstraction
*/
// Created: SBO 2007-09-27
// =============================================================================
class Workspace_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Workspace_ABC() {}
    virtual ~Workspace_ABC() {}
    //@}
};

}
}

#endif // __Workspace_ABC_h_
