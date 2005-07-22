// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnType.h $
// $Author: Sbo $
// $Modtime: 7/07/05 15:10 $
// $Revision: 4 $
// $Workfile: PawnType.h $
//
// *****************************************************************************

#ifndef __PawnType_h_
#define __PawnType_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace TIC
{
    class NatureLevel;
    class NatureCategory;
    class NatureSpecialization;
    class NatureWeapon;
    class NatureMobility;
    class NatureQualifier;
    class NatureCapacity;
    class PawnPlatform;


// =============================================================================
/** @class  PawnType
    @brief  PawnType
    @par    Using example
    @code
    PawnType;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class PawnType
{
    MT_COPYNOTALLOWED( PawnType );

    friend class Pawn;

public:
    //! @name Manager
    //@{
    static void                 Initialize             ( const std::string& strScipioPawnConfigFile );
    static void                 Terminate              ();

    static const PawnType*      Find                   ( const std::string& strName );
    static const PawnType*      Find                   ( ASN1T_TypePion     asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&          GetName                () const;
          uint                  GetId                  () const;
    const NatureLevel&          GetNatureLevel         () const;
    const NatureCategory&       GetNatureCategory      () const;
    const NatureSpecialization& GetNatureSpecialization() const;
    const NatureWeapon&         GetNatureWeapon        () const;
    const NatureMobility&       GetNatureMobility      () const;
    const NatureQualifier&      GetNatureQualifier     () const;
    const NatureCapacity&       GetNatureCapacity      () const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PawnType( const std::string& strName, XmlInputArchive& archive );
    virtual ~PawnType();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, const PawnType*, sCaseInsensitiveLess > T_PawnTypeMap;
    typedef T_PawnTypeMap::const_iterator                                  CIT_PawnTypeMap;

    typedef std::multiset< const PawnPlatform* >    T_PawnPlatformMultiSet;
    typedef T_PawnPlatformMultiSet::const_iterator  CIT_PawnPlatformMultiSet;
    //@}

private:
    //! @name Member data
    //@{
    std::string                   strName_;
	uint                          nId_;

    const NatureLevel*            pLevel_;
    const NatureCategory*         pCategory_;
    const NatureSpecialization*   pSpecialization_;
    const NatureWeapon*           pWeapon_;
    const NatureMobility*         pMobility_;
    const NatureQualifier*        pQualifier_;
    const NatureCapacity*         pCapacity_;

    T_PawnPlatformMultiSet        platformTypes_;

    // global type list
    static T_PawnTypeMap          pawnTypes_;
    //@}
};

} // end namespace TIC

#include "PawnType.inl"

#endif // __PawnType_h_
