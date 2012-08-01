// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ObjectCreator_ABC_h_
#define __ADN_ObjectCreator_ABC_h_

class ADN_Ref_ABC;

// =============================================================================
/** @class  ADN_ObjectCreator_ABC
*/
// Created: AGN 2003-11-18
// =============================================================================
class ADN_ObjectCreator_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ObjectCreator_ABC()  {}
    virtual ~ADN_ObjectCreator_ABC()  {}
    //@}

    //! @name Operations
    //@{
    virtual ADN_Ref_ABC* CreateObject() = 0;
    virtual ADN_Ref_ABC* CreateObjectAsCopyOf( void* ) = 0;
    //@}
};

#endif // __ADN_ObjectCreator_ABC_h_
