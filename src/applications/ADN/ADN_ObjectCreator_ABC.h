// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ObjectCreator_ABC.h $
// $Author: Ape $
// $Modtime: 21/03/05 16:41 $
// $Revision: 1 $
// $Workfile: ADN_ObjectCreator_ABC.h $
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
    //@}

    //! @name Operations
    //@{
    virtual ADN_Ref_ABC* CreateObject() = 0;
    //@}
};


// =============================================================================
/** @class  ADN_DefaultObjectCreator
*/
// Created: AGN 2003-11-18
// =============================================================================
template< typename T >
class ADN_DefaultObjectCreator
: public ADN_ObjectCreator_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_DefaultObjectCreator() : ADN_ObjectCreator_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual ADN_Ref_ABC* CreateObject() { return new T(); }
    //@}
};

#endif // __ADN_ObjectCreator_ABC_h_
