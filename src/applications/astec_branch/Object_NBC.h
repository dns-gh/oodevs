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

#ifndef __Object_NBC_h_
#define __Object_NBC_h_

#include "Object_ABC.h"

// =============================================================================
/** @class  Object_NBC
    @brief  Object_NBC
    @par    Using example
    @code
    Object_NBC;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object_NBC : public Object_ABC
{
    MT_COPYNOTALLOWED( Object_NBC );

public:
    //! @name Constructors/Destructor
    //@{
             Object_NBC( ASN1T_EnumObjectType eType );
             Object_NBC( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~Object_NBC();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( InputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
    uint GetAgentNbcId() const;
    //@}

    //! @name Modifiers
    //@{
    void SetAgentNbcId( uint nAgentNbcId );
    //@}

private:
    //! @name Member data
    //@{
    uint nAgentNbcId_;
    //@}
};

#   include "Object_NBC.inl"

#endif // __Object_NBC_h_
