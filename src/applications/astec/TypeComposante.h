// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypeComposante.h $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: TypeComposante.h $
//
// *****************************************************************************

#ifndef __TypeComposante_h_
#define __TypeComposante_h_

#include "Types.h"


// =============================================================================
// Created: SBO 2005-08-03
// =============================================================================
class TypeComposante
{
    MT_COPYNOTALLOWED( TypeComposante );

public:
     TypeComposante( const std::string& strName, InputArchive& archive );
    ~TypeComposante();

    //! @name Accessors
    //@{
    const std::string& GetName       () const;
          uint         GetID         () const;
          bool         IsLogistic    () const;
          bool         HasMaintenance() const;
          bool         HasMedical    () const;
          bool         HasSupply     () const;
    //@}

private:
    const std::string  strName_;
    uint               nID_;
    bool               bHasMaintenance_;
    bool               bHasMedical_;
    bool               bHasSupply_;
};

#include "TypeComposante.inl"

#endif // __TypeComposante_h_
