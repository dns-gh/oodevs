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

#ifndef __MOS_DynaObject_SiteFranchissement_h_
#define __MOS_DynaObject_SiteFranchissement_h_

#include "MOS_DynaObject_ABC.h"

// =============================================================================
/** @class  MOS_DynaObject_SiteFranchissement
    @brief  MOS_DynaObject_SiteFranchissement
    @par    Using example
    @code
    MOS_DynaObject_SiteFranchissement;
    @endcode
*/
// Created: SBO 2005-09-02
// =============================================================================
class MOS_DynaObject_SiteFranchissement : public MOS_DynaObject_ABC
{
    MT_COPYNOTALLOWED( MOS_DynaObject_SiteFranchissement );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_DynaObject_SiteFranchissement();
             MOS_DynaObject_SiteFranchissement( const ASN1T_MsgObjectCreation& asnMsg );
    virtual ~MOS_DynaObject_SiteFranchissement();
    //@}

    //! @name Operations
    //@{
    void ReadODB ( MT_XXmlInputArchive&  archive );
    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
    uint GetLargeur        () const;
    uint GetProfondeur     () const;
    uint GetVitesseCourant () const;
    bool AreBergesAAmenager() const;
    //@}

    //! @name Modifiers
    //@{
    void SetParameters( uint nWidth, uint nDepth, uint nFlow, bool bConstruct );
    //@}

private:
    //! @name Member data
    //@{
    uint nLargeur_;
    uint nProfondeur_;
    uint nVitesseCourant_;
    bool bBergesAAmenager_;
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject_SiteFranchissement.inl"
#endif

#endif // __MOS_DynaObject_SiteFranchissement_h_
