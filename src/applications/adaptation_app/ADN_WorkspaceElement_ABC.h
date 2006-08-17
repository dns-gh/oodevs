// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_WorkspaceElement_ABC.h $
// $Author: Ape $
// $Modtime: 4/04/05 10:56 $
// $Revision: 5 $
// $Workfile: ADN_WorkspaceElement_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_WorkspaceElement_ABC_h_
#define __ADN_WorkspaceElement_ABC_h_

#include "ADN_Data_ABC.h"
#include "ADN_GUI_ABC.h"


// =============================================================================
/** @class  ADN_WorkspaceElement_ABC
    @brief  ADN_WorkspaceElement_ABC
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_WorkspaceElement_ABC
{
    MT_COPYNOTALLOWED( ADN_WorkspaceElement_ABC )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_WorkspaceElement_ABC( const char* szName ) : strName_( szName ) {};
    virtual ~ADN_WorkspaceElement_ABC() {};
    //@}

    //! @name Accessors
    //@{
    virtual ADN_Data_ABC& GetDataABC() = 0;
    virtual ADN_GUI_ABC&  GetGuiABC() = 0;

    const QString& GetName() { return strName_; };
    //@}

private:
    const QString strName_;
};


#endif // __ADN_WorkspaceElement_ABC_h_
