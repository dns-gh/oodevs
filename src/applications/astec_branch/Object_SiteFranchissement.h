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

#ifndef __Object_SiteFranchissement_h_
#define __Object_SiteFranchissement_h_

#include "Object_ABC.h"

// =============================================================================
/** @class  Object_SiteFranchissement
    @brief  Object_SiteFranchissement
    @par    Using example
    @code
    Object_SiteFranchissement;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class Object_SiteFranchissement : public Object_ABC
{
    MT_COPYNOTALLOWED( Object_SiteFranchissement );

public:
    //! @name Constructors/Destructor
    //@{
             Object_SiteFranchissement( ASN1T_EnumObjectType eType = EnumObjectType::site_franchissement );
             Object_SiteFranchissement( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~Object_SiteFranchissement();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( InputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
    uint GetWidth     () const;
    uint GetDepth     () const;
    uint GetSpeed     () const;
    bool MustConstruct() const;
    //@}

    //! @name Modifiers
    //@{
    void SetWidth    ( uint nWidth );
    void SetDepth    ( uint nDepth );
    void SetSpeed    ( uint nSpeed );
    void SetConstruct( bool bConstruct );
    //@}

private:
    //! @name Member data
    //@{
    uint nWidth_;
    uint nDepth_;
    uint nSpeed_;
    bool bConstruct_;
    //@}
};

#   include "Object_SiteFranchissement.inl"

#endif // __Object_SiteFranchissement_h_
