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

#ifndef __MOS_Object_Generic_h_
#define __MOS_Object_Generic_h_

#include "MOS_Object_ABC.h"

// =============================================================================
/** @class  MOS_Object_Generic
    @brief  MOS_Object_Generic
    @par    Using example
    @code
    MOS_Object_Generic;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_Object_Generic : public MOS_Object_ABC
{
    MT_COPYNOTALLOWED( MOS_Object_Generic );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Object_Generic( ASN1T_EnumObjectType eType );
             MOS_Object_Generic( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_Object_Generic();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MOS_InputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}
};

#endif // __MOS_Object_Generic_h_
