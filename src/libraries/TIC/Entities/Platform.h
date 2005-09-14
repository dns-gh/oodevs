// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Platform.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:10 $
// $Revision: 20 $
// $Workfile: Platform.h $
//
// *****************************************************************************

#ifndef __Platform_h_
#define __Platform_h_

#include "Types.h"
#include "Entities/ConcreteEntity.h"
#include "Entities/Types/PawnType.h"

namespace TIC
{
    class PawnType;
    class PawnPlatform;
    class Pawn;
    class Position;
    class Device;
    class DeviceCategory;
    class DeviceSubCategory;

// =============================================================================
/** @class  Platform
    @brief  Platform
    @par    Using example
    @code
    Platform;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class Platform : public ConcreteEntity
{
    friend class Pawn;

public:
    //! @name Enums
    //@{
    enum EPlatformState
    {
        ePlatformStatePrisoner,
        ePlatformStateDestroyed,
        ePlatformStateFixable,
        ePlatformStateFixing,
        ePlatformStateOk,
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Platform( const PawnPlatform& platformType, const Pawn& pawn );
    virtual ~Platform();
    //@}

    //! @name Facade Accessors
    //@{
          T_EntityId         GetId               () const;
    const std::string&       GetTypeName         () const;
          EPlatformState     GetState            () const;
    const std::string&       GetEMAT6            () const;
    const std::string&       GetEMAT8            () const;
    const std::string&       GetLFRIL            () const;
    const std::string&       GetNNO              () const;
    const std::string&       GetProtection       () const;
    const std::string&       GetVolume           () const;
          bool               IsEmbarquable       () const;

    // natures
    const Device&            GetDevice           () const;
    const DeviceCategory&    GetDeviceCategory   () const;
    const DeviceSubCategory& GetDeviceSubCategory() const;
    //@}

    //! @name Other Accessors
    //@{
    bool MustBeDisaggregated() const;
    bool CanMove            () const;
    //@}

    //! @name Position management
    //@{
    void Follow( const Pawn&     pawn );
    void Follow( const Platform& platform );
    void MoveTo( const Position& position );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Platform( const Platform& );            //!< Copy constructor
    Platform& operator=( const Platform& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId          nId_;
    const PawnPlatform* pType_;
    EPlatformState      eState_;
    bool                bConfigDisaggreg_; // if disaggregation has been specified in config file
    const Pawn*         pPawn_;
    //@}

    //! @name Static members
    //@{
    static const double rMaxDistanceForSmoothMove_;
    static uint         nNextId_;
    //@}
};

} // end namespace TIC

#include "Platform.inl"

#endif // __Platform_h_
