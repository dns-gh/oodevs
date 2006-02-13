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

#ifndef __Object_Generic_h_
#define __Object_Generic_h_

#include "Object_ABC.h"

// =============================================================================
/** @class  Object_Generic
    @brief  Object_Generic
    @par    Using example
    @code
    Object_Generic;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object_Generic : public Object_ABC
{
    MT_COPYNOTALLOWED( Object_Generic );

public:
    //! @name Constructors/Destructor
    //@{
             Object_Generic( ASN1T_EnumObjectType eType );
             Object_Generic( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~Object_Generic();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( InputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}
};

#endif // __Object_Generic_h_
