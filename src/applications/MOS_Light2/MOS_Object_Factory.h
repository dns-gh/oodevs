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

#ifndef __MOS_Object_Factory_h_
#define __MOS_Object_Factory_h_

#include "MOS_ASN_Types.h"

class MOS_Object_ABC;

// =============================================================================
/** @class  MOS_Object_Factory
    @brief  MOS_Object_Factory
    @par    Using example
    @code
    MOS_Object_Factory;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_Object_Factory
{
    MT_COPYNOTALLOWED( MOS_Object_Factory );

public:
    //! @name Operations
    //@{
    static MOS_Object_ABC* Create(       ASN1T_EnumObjectType     eType   );
    static MOS_Object_ABC* Create(       MOS_InputArchive&     archive );
    static MOS_Object_ABC* Create( const ASN1T_MsgObjectCreation& asnMsg  );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             MOS_Object_Factory();
    virtual ~MOS_Object_Factory();
    //@}
};


#endif // __MOS_Object_Factory_h_
