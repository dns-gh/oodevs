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

#ifndef __MOS_DynaObject_Camp_h_
#define __MOS_DynaObject_Camp_h_

#include "MOS_DynaObject_ABC.h"

// =============================================================================
/** @class  MOS_DynaObject_Camp
    @brief  MOS_DynaObject_Camp
    @par    Using example
    @code
    MOS_DynaObject_Camp;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_DynaObject_Camp : public MOS_DynaObject_ABC
{
    MT_COPYNOTALLOWED( MOS_DynaObject_Camp );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DynaObject_Camp();
             MOS_DynaObject_Camp( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_DynaObject_Camp();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MT_XXmlInputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
    uint GetTC2ID() const;
    //@}

    //! @name Modifiers
    //@{
    void SetTC2ID( uint nTC2ID );
    //@}

private:
    //! @name Member data
    //@{
    uint nTC2ID_;
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject_Camp.inl"
#endif

#endif // __MOS_DynaObject_Camp_h_
