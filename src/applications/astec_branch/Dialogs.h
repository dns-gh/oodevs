// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dialogs_h_
#define __Dialogs_h_

class Controllers;
class Model;

// =============================================================================
/** @class  Dialogs
    @brief  Dialogs
*/
// Created: AGE 2006-04-20
// =============================================================================
class Dialogs
{

public:
    //! @name Constructors/Destructor
    //@{
             Dialogs( QWidget* parent, Controllers& controllers, const Model& model );
    virtual ~Dialogs();
    //@}
};

#endif // __Dialogs_h_
