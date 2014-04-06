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
#include <boost/shared_ptr.hpp>
#include <map>
#include <memory>

namespace geometry
{
    template< typename T > class Point2;
    typedef Point2< float > Point2f;
}

namespace sword
{
    class MagicAction;
    class MagicActionAck;
}

namespace tools
{
    class Path;
}

namespace weather
{
    class Meteo;
}

namespace xml
{
    class xistream;
}

class MT_Ellipse;
class PHY_Ephemeride;
class PHY_GlobalMeteo;
class PHY_LocalMeteo;
class PHY_RawVisionData;
class PHY_IndirectFireDotationClass;
class TER_World;

//*****************************************************************************
// Created: JVT 02-10-21
// Last modified: JVT 04-03-25
//*****************************************************************************
class PHY_MeteoDataManager : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             PHY_MeteoDataManager();
             PHY_MeteoDataManager( const boost::shared_ptr< TER_World >& world,
                 xml::xistream& xis, const tools::Path& detectionFile,
                 uint32_t now, uint32_t tickDuration );
    virtual ~PHY_MeteoDataManager();
    //@}

    //! @name Raw Data management
    //@{
    virtual const PHY_RawVisionData& GetRawVisionData() const;
    virtual const PHY_Ephemeride& GetEphemeride() const;
    virtual void Update( unsigned int date );
    void SendStateToNewClient();

    // Creates and registers a new local weather object from an XML definition.
    // The creation notification will be sent during the next Update().
    // Returned instance is const, use PHY_MeteoDataManager API to modify it.
    boost::shared_ptr< const weather::Meteo > AddLocalWeather( xml::xistream& xis );
    boost::shared_ptr< const weather::Meteo > GetLocalWeather(
        const geometry::Point2f& position, 
        const boost::shared_ptr< const weather::Meteo >& pMeteo ) const;
    boost::shared_ptr< const weather::Meteo > GetGlobalWeather() const;
    // Unregisters a local weather, returns false if the entity cannot be found.
    // The destruction message is sent immediately.
    bool RemoveLocalWeather( uint32_t id );
    //@}

    //! @name Weather effects
    //@{
    void RegisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    void UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory );
    //@}

    //! @name Network
    //@{
    virtual void OnReceiveMsgMeteo( const sword::MagicAction& msg, sword::MagicActionAck& ack, unsigned context );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteWeather( xml::xostream& xos, uint32_t now ) const;
    //@}

private:
    //! @name Registration
    //@{
    virtual void AddMeteo( const boost::shared_ptr< PHY_LocalMeteo >& element );
    //@}

    //! @name Helpers
    //@{
    boost::shared_ptr< const weather::Meteo > InternalAddLocalWeather( xml::xistream& );
    void ReadPatchGlobal( xml::xistream& xis );
    void UpdateGlobalWeather( const sword::MagicAction& msg );
    void ManageLocalWeather( const sword::MagicAction& msg, sword::MagicActionAck& ack );
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
    boost::shared_ptr< TER_World > world_;
    boost::shared_ptr< PHY_Ephemeride > pEphemeride_;
    boost::shared_ptr< PHY_GlobalMeteo > pGlobalMeteo_;
    PHY_RawVisionData* pRawData_;
    std::map< uint32_t, boost::shared_ptr< PHY_LocalMeteo > > meteos_;
    const uint32_t tickDuration_;
    static unsigned int localCounter_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MeteoDataManager )

#endif // __PHY_MeteoDataManager_h_
