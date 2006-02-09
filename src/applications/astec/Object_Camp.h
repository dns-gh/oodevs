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

#ifndef __Object_Camp_h_
#define __Object_Camp_h_

#include "Object_ABC.h"

// =============================================================================
/** @class  Object_Camp
    @brief  Object_Camp
    @par    Using example
    @code
    Object_Camp;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object_Camp : public Object_ABC
{
    MT_COPYNOTALLOWED( Object_Camp );

public:
    //! @name Constructors/Destructor
    //@{
             Object_Camp( ASN1T_EnumObjectType eType );
             Object_Camp( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~Object_Camp();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( InputArchive&  archive );
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

#   include "Object_Camp.inl"

#endif // __Object_Camp_h_
