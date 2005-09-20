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

#ifndef __MOS_Object_ItineraireLogistique_h_
#define __MOS_Object_ItineraireLogistique_h_

#include "MOS_Object_ABC.h"

// =============================================================================
/** @class  MOS_Object_ItineraireLogistique
    @brief  MOS_Object_ItineraireLogistique
    @par    Using example
    @code
    MOS_Object_ItineraireLogistique;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_Object_ItineraireLogistique : public MOS_Object_ABC
{
    MT_COPYNOTALLOWED( MOS_Object_ItineraireLogistique );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Object_ItineraireLogistique( ASN1T_EnumObjectType eType = EnumObjectType::itineraire_logistique );
             MOS_Object_ItineraireLogistique( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_Object_ItineraireLogistique();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MOS_InputArchive&  archive );
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

    //! @name Modifiers
    //@{
    void SetFlow     ( uint nFlow      );
    void SetWidth    ( uint nWidth     );
    void SetLength   ( uint nLength    );
    void SetMaxWeight( uint nMaxWeight );
    void SetEquipped ( bool bEquipped  );
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

#   include "MOS_Object_ItineraireLogistique.inl"

#endif // __MOS_Object_ItineraireLogistique_h_
