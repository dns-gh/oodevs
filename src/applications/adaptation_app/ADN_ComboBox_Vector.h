// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ComboBox_Vector_h_
#define __ADN_ComboBox_Vector_h_

#include "ADN_ComboBox.h"

// =============================================================================
/** @class  ADN_ComboBox_Vector
    @brief  ADN_ComboBox_Vector
    @par    Using example
    @code
    ADN_ComboBox_Vector;
    @endcode
*/
// Created: APE 2005-01-05
// =============================================================================
class ADN_ComboBox_Vector : public ADN_ComboBox
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ComboBox_Vector( QWidget* pParent = 0 );
    virtual ~ADN_ComboBox_Vector();
    //@}
};

#endif // __ADN_ComboBox_Vector_h_
