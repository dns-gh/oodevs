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

#ifndef __Object_ROTA_h_
#define __Object_ROTA_h_

#include "Object_ABC.h"

// =============================================================================
/** @class  Object_ROTA
    @brief  Object_ROTA
    @par    Using example
    @code
    Object_ROTA;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object_ROTA : public Object_ABC
{
    MT_COPYNOTALLOWED( Object_ROTA );

public:
    //! @name Constructors/Destructor
    //@{
             Object_ROTA( ASN1T_EnumObjectType eType = EnumObjectType::rota );
             Object_ROTA( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~Object_ROTA();
    //@}

    //! @name Operations
    //@{
//    void ReadODB ( InputArchive&  archive );
//    void WriteODB( MT_XXmlOutputArchive& archive ) const;
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

#   include "Object_ROTA.inl"

#endif // __Object_ROTA_h_
