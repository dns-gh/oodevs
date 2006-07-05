// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GraphicPreference_ABC_h_
#define __GraphicPreference_ABC_h_

// =============================================================================
/** @class  GraphicPreference_ABC
    @brief  Graphic preference base class
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicPreference_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GraphicPreference_ABC() {}
    virtual ~GraphicPreference_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Display( QWidget* parent ) = 0;
    virtual void Commit() = 0;
    virtual void Revert() = 0;
    //@}
};

#endif // __GraphicPreference_ABC_h_
