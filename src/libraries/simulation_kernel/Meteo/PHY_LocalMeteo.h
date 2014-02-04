// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_LocalMeteo_h_
#define __PHY_LocalMeteo_h_

#include "meteo/Meteo.h"
#include <boost/serialization/split_member.hpp>
#include <memory>

namespace sword
{
    class MissionParameters;
}

namespace xml
{
    class xistream;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class PHY_RawVisionData;
class TER_World;

// =============================================================================
/** @class  PHY_LocalMeteo
    @brief  PHY_LocalMeteo
*/
// Created: SLG 2010-03-18
// =============================================================================
class PHY_LocalMeteo : public weather::Meteo
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_LocalMeteo();
             PHY_LocalMeteo( unsigned int id, xml::xistream& xis,
                 const weather::PHY_Lighting& light, unsigned int timeStep,
                 const std::shared_ptr< TER_World >& world );
             PHY_LocalMeteo( unsigned int id, const sword::MissionParameters& message,
                 const weather::PHY_Lighting& light, unsigned int timeStep,
                 const std::shared_ptr< TER_World >& world );
    virtual ~PHY_LocalMeteo();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateMeteoPatch( int date, PHY_RawVisionData& dataVision,
            const boost::shared_ptr< weather::Meteo >& meteo );
    virtual void Update( const sword::MissionParameters& msg );
    using weather::Meteo::Update;
    virtual bool IsPatched() const;
    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& ar, const unsigned int );
    void save( MIL_CheckPointOutArchive& ar, const unsigned int ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void LocalUpdate( const sword::MissionParameters& msg, bool isCreation = false );
    //@}

private:
    //! @name Member data
    //@{
    std::shared_ptr< TER_World > world_;
    int startTime_;
    int endTime_;
    MT_Vector2D upLeft_;
    MT_Vector2D downRight_;
    bool bIsPatched_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_LocalMeteo )

#endif // __PHY_LocalMeteo_h_
