// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationEditor_ABC_h_
#define __LocationEditor_ABC_h_

namespace gui
{
    class ParametersLayer;

// =============================================================================
/** @class  LocationEditor_ABC
    @brief  LocationEditor_ABC
*/
// Created: SBO 2007-03-06
// =============================================================================
class LocationEditor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LocationEditor_ABC() {}
    virtual ~LocationEditor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void StartEdit( gui::ParametersLayer& parameters ) = 0;
    virtual void EndEdit() = 0;
    //@}
};

}

#endif // __LocationEditor_ABC_h_
