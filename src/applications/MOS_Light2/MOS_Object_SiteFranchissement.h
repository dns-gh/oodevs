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

#ifndef __MOS_Object_SiteFranchissement_h_
#define __MOS_Object_SiteFranchissement_h_

#include "MOS_Object_ABC.h"

// =============================================================================
/** @class  MOS_Object_SiteFranchissement
    @brief  MOS_Object_SiteFranchissement
    @par    Using example
    @code
    MOS_Object_SiteFranchissement;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_Object_SiteFranchissement : public MOS_Object_ABC
{
    MT_COPYNOTALLOWED( MOS_Object_SiteFranchissement );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Object_SiteFranchissement( ASN1T_EnumObjectType eType = EnumObjectType::site_franchissement );
             MOS_Object_SiteFranchissement( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_Object_SiteFranchissement();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MOS_InputArchive&  archive );
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

#ifdef MOS_USE_INLINE
#   include "MOS_Object_SiteFranchissement.inl"
#endif

#endif // __MOS_Object_SiteFranchissement_h_
