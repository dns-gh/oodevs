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

#ifndef __Object_ItineraireLogistique_h_
#define __Object_ItineraireLogistique_h_

#include "Object_ABC.h"

// =============================================================================
/** @class  Object_ItineraireLogistique
    @brief  Object_ItineraireLogistique
    @par    Using example
    @code
    Object_ItineraireLogistique;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object_ItineraireLogistique : public Object_ABC
{
    MT_COPYNOTALLOWED( Object_ItineraireLogistique );

public:
    //! @name Constructors/Destructor
    //@{
             Object_ItineraireLogistique( ASN1T_EnumObjectType eType = EnumObjectType::itineraire_logistique );
             Object_ItineraireLogistique( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~Object_ItineraireLogistique();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( InputArchive&  archive );
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

#   include "Object_ItineraireLogistique.inl"

#endif // __Object_ItineraireLogistique_h_
