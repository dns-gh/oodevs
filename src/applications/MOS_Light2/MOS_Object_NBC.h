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

#ifndef __MOS_Object_NBC_h_
#define __MOS_Object_NBC_h_

#include "MOS_Object_ABC.h"

// =============================================================================
/** @class  MOS_Object_NBC
    @brief  MOS_Object_NBC
    @par    Using example
    @code
    MOS_Object_NBC;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_Object_NBC : public MOS_Object_ABC
{
    MT_COPYNOTALLOWED( MOS_Object_NBC );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Object_NBC( ASN1T_EnumObjectType eType );
             MOS_Object_NBC( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_Object_NBC();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MOS_InputArchive&  archive );
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

#   include "MOS_Object_NBC.inl"

#endif // __MOS_Object_NBC_h_
