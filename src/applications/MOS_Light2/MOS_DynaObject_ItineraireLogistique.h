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

#ifndef __MOS_DynaObject_ItineraireLogistique_h_
#define __MOS_DynaObject_ItineraireLogistique_h_

#include "MOS_DynaObject_ABC.h"

// =============================================================================
/** @class  MOS_DynaObject_ItineraireLogistique
    @brief  MOS_DynaObject_ItineraireLogistique
    @par    Using example
    @code
    MOS_DynaObject_ItineraireLogistique;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_DynaObject_ItineraireLogistique : public MOS_DynaObject_ABC
{
    MT_COPYNOTALLOWED( MOS_DynaObject_ItineraireLogistique );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DynaObject_ItineraireLogistique();
             MOS_DynaObject_ItineraireLogistique( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_DynaObject_ItineraireLogistique();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MT_XXmlInputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsEquipped  () const;
    uint GetFlow     () const;
    uint GetWidth    () const;
    uint GetLength   () const;
    uint GetMaxWeight() const;
    //@}

private:
    //! @name Member data
    //@{
    bool bEquipped_;
    uint nFlow_;
    uint nWidth_;
    uint nLength_;
    uint nMaxWeight_;
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject_ItineraireLogistique.inl"
#endif

#endif // __MOS_DynaObject_ItineraireLogistique_h_
