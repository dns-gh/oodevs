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

#ifndef __MOS_Object_ROTA_h_
#define __MOS_Object_ROTA_h_

#include "MOS_Object_ABC.h"

// =============================================================================
/** @class  MOS_Object_ROTA
    @brief  MOS_Object_ROTA
    @par    Using example
    @code
    MOS_Object_ROTA;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_Object_ROTA : public MOS_Object_ABC
{
    MT_COPYNOTALLOWED( MOS_Object_ROTA );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Object_ROTA( ASN1T_EnumObjectType eType = EnumObjectType::rota );
             MOS_Object_ROTA( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_Object_ROTA();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MOS_InputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
          uint                 GetDanger   () const;
    const std::vector< uint >& GetNBCAgents() const;
    //@}

    //! @name Modifiers
    //@{
    void SetDanger  ( uint nDanger   );
    void AddNBCAgent( uint nNBCAgent );
    //@}

private:
    //! @name Member data
    //@{
    uint                nDanger_;
    std::vector< uint > nbcAgents_;
    //@}
};

#   include "MOS_Object_ROTA.inl"

#endif // __MOS_Object_ROTA_h_
