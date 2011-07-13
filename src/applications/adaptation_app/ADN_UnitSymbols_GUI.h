// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_UnitSymbols_GUI_h_
#define __ADN_UnitSymbols_GUI_h_

#include <boost/noncopyable.hpp>
#include "ADN_GUI_ABC.h"

class ADN_UnitSymbols_Data;

// =============================================================================
/** @class  ADN_Symbols_GUI
@brief  ADN_Symbols_GUI
*/
// Created: MMC 2011-07-07
// =============================================================================
class ADN_UnitSymbols_GUI : public ADN_GUI_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_UnitSymbols_GUI( ADN_UnitSymbols_Data& data );
    virtual ~ADN_UnitSymbols_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}
};

#endif // __ADN_UnitSymbols_GUI_h_