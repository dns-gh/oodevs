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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PlatformType.h $
// $Author: Sbo $
// $Modtime: 7/07/05 15:32 $
// $Revision: 8 $
// $Workfile: PlatformType.h $
//
// *****************************************************************************

#ifndef __PlatformType_h_
#define __PlatformType_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace TIC
{
    class Device;
    class DeviceCategory;
    class DeviceSubCategory;

// =============================================================================
/** @class  PlatformType
    @brief  PlatformType
    @par    Using example
    @code
    PlatformType;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class PlatformType
{
    MT_COPYNOTALLOWED( PlatformType );

public:
    //! @name Manager
    //@{
    static       void          Initialize  ( const std::string& strPlatformFile );
    static       void          LoadNatures ( const std::string& strFile );
    static       void          Terminate   ();

    static const PlatformType* Find        ( const std::string&   strName    );
    static const PlatformType* Find        ( ASN1T_TypeEquipement asnId      );

    static       void          SetBlackList( const std::string& strBlackList );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName           () const;
          uint         GetId             () const;
    const std::string& GetEMAT6          () const;
    const std::string& GetEMAT8          () const;
    const std::string& GetLFRIL          () const;
    const std::string& GetNNO            () const;
    const std::string& GetProtection     () const;
    const std::string& GetVolume         () const;
          T_Speed      GetMaxSpeed       () const;
          bool         CanBeDisaggregated() const;

    // natures
    const Device&            GetDevice           () const;
    const DeviceCategory&    GetDeviceCategory   () const;
    const DeviceSubCategory& GetDeviceSubCategory() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PlatformType( const std::string& strName, XmlInputArchive& archive );
    virtual ~PlatformType();
    //@}

    //! @name Internal initialization operations
    //@{
    static void LoadPlatforms( const std::string& strFile );
    //@}

    //! @name Blacklist management
    //@{
    static void FilterDevices ( XmlInputArchive& archive );
    static void Filter        ( const Device&    device  );
    static void Filter        ( const Device&    device, const std::string& strCat );
    static void Filter        ( const Device&    device, const std::string& strCat, const std::string& strSubCat );
    static void FilterPlatform( XmlInputArchive& archive );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PlatformType*, sCaseInsensitiveLess > T_PlatformTypeMap;
    typedef T_PlatformTypeMap::iterator                                        IT_PlatformTypeMap;
    typedef T_PlatformTypeMap::const_iterator                                  CIT_PlatformTypeMap;
    //@}

private:
    //! @name Member data
    //@{
    std::string   strName_;
	uint          nId_;
    T_Speed       nMaxSpeed_;

    // disaggregation
    mutable bool  bCanBeDisaggregated_;

    // normalized codes
    std::string   strEMAT6_;
    std::string   strEMAT8_;
    std::string   strLFRIL_;
    std::string   strNNO_;
    std::string   strProtection_;
    std::string   strVolume_;

    // natures
    const Device*            pDevice_;
    const DeviceCategory*    pDeviceCategory_;
    const DeviceSubCategory* pDeviceSubCategory_;

    // global type list
    static T_PlatformTypeMap platformTypes_;
    //@}
};

} // end namespace TIC

#include "PlatformType.inl"

#endif // __PlatformType_h_
