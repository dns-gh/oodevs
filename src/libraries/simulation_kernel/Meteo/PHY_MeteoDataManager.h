// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_MeteoDataManager_h_
#define __PHY_MeteoDataManager_h_

#include "meteo/MeteoManager_ABC.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/PHY_Meteo.h"
#include "PHY_Ephemeride.h"
#include "Tools/MIL_IDManager.h"

namespace sword
{
    class MagicAction;
}

namespace xml
{
    class xistream;
}

class PHY_GlobalMeteo;
class MIL_Config;
class PHY_RawVisionData;
class MT_Ellipse;
class PHY_IndirectFireDotationClass;

//*****************************************************************************
// Created: JVT 02-10-21
// Last modified: JVT 04-03-25
//*****************************************************************************
class PHY_MeteoDataManager : private boost::noncopyable
                           , public weather::MeteoManager_ABC
{
    friend class PHY_Meteo; // For UnregisterMeteo

public:
    //! @name Constructor/Destructor
    //@{
    explicit PHY_MeteoDataManager( MIL_Config& config );
    virtual ~PHY_MeteoDataManager();
    //@}

    //! @name Raw Data management
    //@{
    virtual const PHY_RawVisionData& GetRawVisionData() const;
    virtual const PHY_Ephemeride& GetEphemeride() const;
    virtual const weather::PHY_Lighting& GetLighting () const;
    virtual void Update( unsigned int date );
    void SendStateToNewClient();
    //@}

    //! @name Weather effects
    //@{
    void RegisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    //@}

    //! @name Network
    //@{
    virtual void OnReceiveMsgMeteo( const sword::MagicAction& msg );
    //@}

private:
    //! @name Registration
    //@{
    virtual void RegisterMeteo( weather::PHY_Meteo& element );
    virtual void UnregisterMeteo( weather::PHY_Meteo& element );
    //@}

    //! @name Helpers
    //@{
    void ReadPatchLocal( xml::xistream& xis );
    void ReadPatchGlobal( xml::xistream& xis );
    void Initialize( xml::xistream& xis, MIL_Config& config );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< weather::PHY_Meteo* > T_MeteoSet;
    typedef T_MeteoSet::iterator            IT_MeteoSet;
    typedef T_MeteoSet::const_iterator      CIT_MeteoSet;
    //@}

private:
    //! @name Init
    //@{
    void InitializeGlobalMeteo( xml::xistream& xis );
    void InitializeLocalMeteos( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    PHY_Ephemeride* pEphemeride_;
    PHY_GlobalMeteo* pGlobalMeteo_;
    T_MeteoSet meteos_;
    PHY_RawVisionData* pRawData_;
    static MIL_IDManager idManager_;
    //@}
};


#include "PHY_MeteoDataManager.inl"

#endif // __PHY_MeteoDataManager_h_
