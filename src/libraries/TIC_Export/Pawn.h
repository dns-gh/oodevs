// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Pawn.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:32 $
// $Revision: 17 $
// $Workfile: Pawn.h $
//
// *****************************************************************************

#ifndef __TICExport_Pawn_h_
#define __TICExport_Pawn_h_

#include <string>

namespace TIC
{
    class Pawn;
}

namespace TICExport
{

    class Platform;

// =============================================================================
/** @class  Pawn
    @brief  Pawn
    @par    Using example
    @code
    Pawn;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class Pawn
{
    friend class Automat;

public:
    //! @name Types
    //@{
    enum EState
    {
        eStateOperational,
        eStateFixable,
        eStateDestroyed
    };
    //@}

public:
	//! @name Constructors/Destructor
    //@{
             Pawn();
    virtual ~Pawn();
    //@}

    //! @name Accessors
    //@{
          unsigned int GetId                  () const;
    const std::string& GetNatureName          () const;
    const std::string& GetNatureLevel         () const;
    const std::string& GetNatureCategory      () const;
    const std::string& GetNatureSpecialization() const;
    const std::string& GetNatureWeapon        () const;
    const std::string& GetNatureMobility      () const;
    const std::string& GetNatureQualifier     () const;
    const std::string& GetNatureCapacity      () const;
          int          GetDirection           () const;
          unsigned int GetSpeed               () const;
          double       GetLatitude            () const;
          double       GetLongitude           () const;
          int          GetHeight              () const;
          EState       GetState               () const;
          bool         IsPc                   () const;
    //@}

    //! @name Disaggregation
    //@{
          bool         IsAggregated           () const;

          unsigned int GetPlatformCount       () const;
    const Platform     GetPlatformAt          ( unsigned int nIdx ) const;
    //@}

private:
    //! @name Internal use constructor
    //@{
    Pawn( const TIC::Pawn& pawn );
    //@}

	//! @name Member data
    //@{
    const TIC::Pawn* pPawn_;
	//@}
};

} // end namespace TICExport

#endif // __TICExport_Pawn_h_
