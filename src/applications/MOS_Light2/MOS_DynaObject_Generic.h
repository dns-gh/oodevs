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

#ifndef __MOS_DynaObject_Generic_h_
#define __MOS_DynaObject_Generic_h_

#include "MOS_DynaObject_ABC.h"

// =============================================================================
/** @class  MOS_DynaObject_Generic
    @brief  MOS_DynaObject_Generic
    @par    Using example
    @code
    MOS_DynaObject_Generic;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_DynaObject_Generic : public MOS_DynaObject_ABC
{
    MT_COPYNOTALLOWED( MOS_DynaObject_Generic );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DynaObject_Generic();
             MOS_DynaObject_Generic( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_DynaObject_Generic();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MT_XXmlInputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}
};

#endif // __MOS_DynaObject_Generic_h_
