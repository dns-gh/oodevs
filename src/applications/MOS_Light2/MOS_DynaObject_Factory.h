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

#ifndef __MOS_DynaObject_Factory_h_
#define __MOS_DynaObject_Factory_h_

#include "MOS_ASN_Types.h"

class MOS_DynaObject_ABC;

// =============================================================================
/** @class  MOS_DynaObject_Factory
    @brief  MOS_DynaObject_Factory
    @par    Using example
    @code
    MOS_DynaObject_Factory;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_DynaObject_Factory
{
    MT_COPYNOTALLOWED( MOS_DynaObject_Factory );

public:
    //! @name Operations
    //@{
    static MOS_DynaObject_ABC* Create(       ASN1T_EnumObjectType     eType   );
    static MOS_DynaObject_ABC* Create(       MT_XXmlInputArchive&     archive );
    static MOS_DynaObject_ABC* Create( const ASN1T_MsgObjectCreation& asnMsg  );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             MOS_DynaObject_Factory();
    virtual ~MOS_DynaObject_Factory();
    //@}
};


#endif // __MOS_DynaObject_Factory_h_
