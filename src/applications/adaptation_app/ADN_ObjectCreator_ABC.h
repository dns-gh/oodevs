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
    virtual void Polish() {}
    //@}
};

// =============================================================================
/** @class  ADN_DefaultObjectCreator
*/
// Created: AGN 2003-11-18
// =============================================================================
template< typename T >
class ADN_DefaultObjectCreator : public ADN_ObjectCreator_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_DefaultObjectCreator() {}
    virtual ~ADN_DefaultObjectCreator() {}
    //@}

    //! @name Operations
    //@{
    virtual ADN_Ref_ABC* CreateObject() { return new T(); }
    //@}
};

#endif // __ADN_ObjectCreator_ABC_h_
