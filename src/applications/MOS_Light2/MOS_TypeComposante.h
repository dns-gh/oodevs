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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypeComposante.h $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: MOS_TypeComposante.h $
//
// *****************************************************************************

#ifndef __MOS_TypeComposante_h_
#define __MOS_TypeComposante_h_

#include "MOS_Types.h"


// =============================================================================
// Created: SBO 2005-08-03
// =============================================================================
class MOS_TypeComposante
{
    MT_COPYNOTALLOWED( MOS_TypeComposante );

public:
     MOS_TypeComposante( const std::string& strName, MT_InputArchive_ABC& archive );
    ~MOS_TypeComposante();

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

#include "MOS_TypeComposante.inl"

#endif // __MOS_TypeComposante_h_
