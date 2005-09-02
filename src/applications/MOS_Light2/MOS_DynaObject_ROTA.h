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

#ifndef __MOS_DynaObject_ROTA_h_
#define __MOS_DynaObject_ROTA_h_

#include "MOS_DynaObject_ABC.h"

// =============================================================================
/** @class  MOS_DynaObject_ROTA
    @brief  MOS_DynaObject_ROTA
    @par    Using example
    @code
    MOS_DynaObject_ROTA;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_DynaObject_ROTA : public MOS_DynaObject_ABC
{
    MT_COPYNOTALLOWED( MOS_DynaObject_ROTA );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DynaObject_ROTA();
             MOS_DynaObject_ROTA( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_DynaObject_ROTA();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MT_XXmlInputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
          uint                 GetDanger   () const;
    const std::vector< uint >& GetNbcAgents() const;
    //@}

private:
    //! @name Member data
    //@{
    uint                nDanger_;
    std::vector< uint > nbcAgents_;
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject_ROTA.inl"
#endif

#endif // __MOS_DynaObject_ROTA_h_
