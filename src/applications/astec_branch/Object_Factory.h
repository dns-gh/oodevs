// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Object_Factory_h_
#define __Object_Factory_h_

#include "ASN_Types.h"

class Object_ABC;

// =============================================================================
/** @class  Object_Factory
    @brief  Object_Factory
    @par    Using example
    @code
    Object_Factory;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object_Factory
{
    MT_COPYNOTALLOWED( Object_Factory );

public:
    //! @name Operations
    //@{
    static Object_ABC* Create(       ASN1T_EnumObjectType     eType   );
    static Object_ABC* Create(       InputArchive&     archive );
    static Object_ABC* Create( const ASN1T_MsgObjectCreation& asnMsg  );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             Object_Factory();
    virtual ~Object_Factory();
    //@}
};


#endif // __Object_Factory_h_
