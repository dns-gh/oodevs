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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Platform.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:33 $
// $Revision: 9 $
// $Workfile: Platform.h $
//
// *****************************************************************************

#ifndef __TICExport_Platform_h_
#define __TICExport_Platform_h_

#include "TIC_Export.h"
#include <string>

namespace TIC
{
    class Platform;
}

namespace TICExport
{

// =============================================================================
/** @class  Platform
    @brief  Platform
    @par    Using example
    @code
    Platform;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class ENGINE_API Platform
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
             Platform();
    virtual ~Platform();
    //@}

    //! @name Accessors
    //@{
          unsigned int GetId            () const;
    const std::string& GetTypeName      () const;
          EPlatformState GetState       () const;
    const std::string& GetTypeEMAT6     () const;
    const std::string& GetTypeEMAT8     () const;
    const std::string& GetTypeLFRIL     () const;
    const std::string& GetTypeNNO       () const;
    const std::string& GetTypeProtection() const;
    const std::string& GetTypeVolume    () const;
          int          GetDirection     () const;
          unsigned int GetSpeed         () const;
          double       GetLatitude      () const;
          double       GetLongitude     () const;
          int          GetHeight        () const;

    const std::string& GetDevice           () const;
    const std::string& GetDeviceCategory   () const;
    const std::string& GetDeviceSubCategory() const;
    //@}

private:
    //! @name Internal use constructor
    //@{
    Platform( const TIC::Platform& platform );
    //@}

	//! @name Member data
    //@{
    const TIC::Platform* pPlatform_;
	//@}
};

} // end namespace TICExport

#endif // __TICExport_Platform_h_
