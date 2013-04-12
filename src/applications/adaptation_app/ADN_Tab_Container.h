// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Tab_Container_h_
#define __ADN_Tab_Container_h_

#include "ADN_GUI_Container.h"

// =============================================================================
/** @class  ADN_Tab_Container
    @brief  ADN_Tab_Container
*/
// Created: ABR 2012-01-24
// =============================================================================
class ADN_Tab_Container : public ADN_GUI_Container
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Tab_Container( E_WorkspaceElements workspaceElement, ADN_Data_Container& data );
    virtual ~ADN_Tab_Container();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    virtual void ChangeCurrentSubTab( int subTab );
    //@}

private:
    //! @name Member data
    //@{
    QTabWidget* pTabWidget_;
    //@}
};

#endif // __ADN_Tab_Container_h_
