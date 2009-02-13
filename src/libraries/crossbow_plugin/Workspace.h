// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Workspace_h_
#define __Workspace_h_

#include "ESRI.h"
#include "Workspace_ABC.h"

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  Workspace
    @brief  Workspace
*/
// Created: SBO 2007-09-27
// =============================================================================
class Workspace : public Workspace_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Workspace();
    virtual ~Workspace();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Workspace( const Workspace& );            //!< Copy constructor
    Workspace& operator=( const Workspace& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeLicence( esriLicenseProductCode product );
    //@}

private:
    //! @name Member data
    //@{
    IAoInitializePtr license_;
    //@}
};

}
}

#endif // __Workspace_h_
