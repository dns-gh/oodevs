// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Linear_Container_h_
#define __ADN_Linear_Container_h_

#include "ADN_GUI_Container.h"

// =============================================================================
/** @class  ADN_Linear_Container
    @brief  ADN_Linear_Container
*/
// Created: ABR 2012-01-24
// =============================================================================
class ADN_Linear_Container : public ADN_GUI_Container
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Linear_Container( Qt::Orientation orientation, bool useScrollBar, const char* szClassName, ADN_Data_Container& data );
    virtual ~ADN_Linear_Container();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}

    //! @name Member data
    //@{
    Qt::Orientation orientation_;
    bool            useScrollBar_;
    //@}
};

#endif // __ADN_Linear_Container_h_
