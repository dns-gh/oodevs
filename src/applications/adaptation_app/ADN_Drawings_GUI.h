// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Drawings_GUI_h_
#define __ADN_Drawings_GUI_h_

#include <boost/noncopyable.hpp>
#include "ADN_GUI_ABC.h"

class ADN_Drawings_Data;

// =============================================================================
/** @class  ADN_Symbols_GUI
    @brief  ADN_Symbols_GUI
*/
// Created: ABR 2011-04-18
// =============================================================================
class ADN_Drawings_GUI : public ADN_GUI_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Drawings_GUI( ADN_Drawings_Data& data );
    virtual ~ADN_Drawings_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}
};

#endif // __ADN_Drawings_GUI_h_
