// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_SearchLineEdit_h_
#define __ADN_SearchLineEdit_h_

#include "ADN_BaseEditLine.h"

class ADN_ClearButton;
class ADN_SearchButton;

// =============================================================================
/** @class  ADN_SearchLineEdit
    @brief  ADN_SearchLineEdit
*/
// Created: ABR 2012-01-19
// =============================================================================
class ADN_SearchLineEdit : public ADN_BaseEditLine
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_SearchLineEdit( QWidget* parent = 0 );
    virtual ~ADN_SearchLineEdit();
    //@}

private:
    //! @name Member data
    //@{
    ADN_ClearButton* clearButton_;
    ADN_SearchButton* searchButton_;
    //@}
};

#endif // __ADN_SearchLineEdit_h_
