// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceCategory.h $
// $Author: Sbo $
// $Modtime: 30/06/05 16:02 $
// $Revision: 1 $
// $Workfile: DeviceCategory.h $
//
// *****************************************************************************

#ifndef __DeviceCategory_h_
#define __DeviceCategory_h_

namespace TIC
{

// =============================================================================
// @class  DeviceCategory
// Created: SBO 2005-06-30
// =============================================================================
class DeviceCategory
{
    MT_COPYNOTALLOWED( DeviceCategory )

public:
    //! @name constant
    //@{
    static const DeviceCategory  notDefined;
    //@}

public:
    //! @name Manager
    //@{
    static       void            Initialize();
    static       void            Terminate ();

    static const DeviceCategory* Find      ( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&   GetName   () const;
    const std::string&   GetKey    () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eNone,
    };

    typedef std::map< std::string, const DeviceCategory*, sCaseInsensitiveLess > T_DeviceCategoryMap;
    typedef T_DeviceCategoryMap::const_iterator                                  CIT_DeviceCategoryMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     DeviceCategory( const std::string& strName, E_Type nType, const std::string& strKey );
    ~DeviceCategory();
    //@}

private:
    //! @name Member data
    //@{
    const std::string   strName_;
    const E_Type        nType_;
    const std::string   strKey_;

    static T_DeviceCategoryMap  deviceCategories_;
    //@}
};

} // end namespace TIC

#include "DeviceCategory.inl"

#endif // __DeviceCategory_h_
