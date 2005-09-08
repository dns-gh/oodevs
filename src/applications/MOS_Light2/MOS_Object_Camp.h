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

#ifndef __MOS_Object_Camp_h_
#define __MOS_Object_Camp_h_

#include "MOS_Object_ABC.h"

// =============================================================================
/** @class  MOS_Object_Camp
    @brief  MOS_Object_Camp
    @par    Using example
    @code
    MOS_Object_Camp;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_Object_Camp : public MOS_Object_ABC
{
    MT_COPYNOTALLOWED( MOS_Object_Camp );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Object_Camp( ASN1T_EnumObjectType eType );
             MOS_Object_Camp( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_Object_Camp();
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
#   include "MOS_Object_Camp.inl"
#endif

#endif // __MOS_Object_Camp_h_
