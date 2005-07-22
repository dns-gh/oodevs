// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/Device.h $
// $Author: Sbo $
// $Modtime: 13/07/05 15:11 $
// $Revision: 2 $
// $Workfile: Device.h $
//
// *****************************************************************************

#ifndef __Device_h_
#define __Device_h_

namespace TIC
{

// =============================================================================
// @class  Device
// Created: SBO 2005-06-30
// =============================================================================
class Device
{
    MT_COPYNOTALLOWED( Device )

public:
    //! @name constant
    //@{
    static const Device  notDefined;
    //@}

public:
    //! @name Manager
    //@{
    static       void    Initialize();
    static       void    Terminate ();

    static const Device* Find      ( const std::string& strName );
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
        eNone                  ,
        eArme                  ,
        eEquipementNBC         ,
        eAeronef               ,
        eEquipementGenie       ,
        eVehicule              ,
        eEquipementElectronique,
        eBatiment              ,
        eAutreTypeEquipement   ,
    };

    typedef std::map< std::string, const Device*, sCaseInsensitiveLess > T_DeviceMap;
    typedef T_DeviceMap::const_iterator                                  CIT_DeviceMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     Device( const std::string& strName, E_Type nType, const std::string& strKey );
    ~Device();
    //@}

private:
    //! @name Member data
    //@{
    const std::string   strName_;
    const E_Type        nType_;
    const std::string   strKey_;
    //@}

    //! @name Static members
    //@{
    static T_DeviceMap  devices_;
    //@}
};

} // end namespace TIC

#include "Device.inl"

#endif // __Device_h_
