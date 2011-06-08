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
#include "meteo/Meteo.h"
#include "meteo/PHY_GlobalMeteo.h"
#include "PHY_Ephemeride.h"

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
    virtual void AddMeteo( weather::Meteo& element );
    //@}

    //! @name Helpers
    //@{
    void ReadPatchLocal( xml::xistream& xis );
    void ReadPatchGlobal( xml::xistream& xis );
    void Initialize( xml::xistream& xis, MIL_Config& config );
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
    PHY_RawVisionData* pRawData_;
    //@}
};


#include "PHY_MeteoDataManager.inl"

#endif // __PHY_MeteoDataManager_h_
