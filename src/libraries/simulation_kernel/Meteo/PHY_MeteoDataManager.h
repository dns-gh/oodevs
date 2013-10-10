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

#include "MIL.h"
#include "meteo/MeteoManager_ABC.h"

namespace client
{
    class MagicActionAck;
}

namespace sword
{
    class MagicAction;
}

namespace xml
{
    class xistream;
}

class MIL_Config;
class MT_Ellipse;
class PHY_Ephemeride;
class PHY_GlobalMeteo;
class PHY_RawVisionData;
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
             PHY_MeteoDataManager();
    explicit PHY_MeteoDataManager( MIL_Config& config );
    virtual ~PHY_MeteoDataManager();
    //@}

    //! @name Operations
    //@{
    static void Initialize();
    //@}

    //! @name Raw Data management
    //@{
    virtual const PHY_RawVisionData& GetRawVisionData() const;
    virtual const PHY_Ephemeride& GetEphemeride() const;
    virtual void Update( unsigned int date );
    void SendStateToNewClient();
    boost::shared_ptr< weather::Meteo > GetLocalWeather( const geometry::Point2f& position, boost::shared_ptr< weather::Meteo > pMeteo ) const;
    //@}

    //! @name Weather effects
    //@{
    void RegisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    //@}

    //! @name Network
    //@{
    virtual void OnReceiveMsgMeteo( const sword::MagicAction& msg, unsigned context, unsigned client );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteWeather( xml::xostream& xos ) const;
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
    void Load( xml::xistream& xis, MIL_Config& config );
    void UpdateGlobalWeather( const sword::MagicAction& msg );
    void ManageLocalWeather( const sword::MagicAction& msg, client::MagicActionAck& ack );
    void RemoveLocalWeather( const sword::MagicAction& msg );
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
    static unsigned int localCounter_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MeteoDataManager )

#endif // __PHY_MeteoDataManager_h_
