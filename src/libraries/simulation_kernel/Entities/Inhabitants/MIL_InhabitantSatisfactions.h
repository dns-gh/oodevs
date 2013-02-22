// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_InhabitantSatisfactions_h_
#define __MIL_InhabitantSatisfactions_h_

#include "MIL.h"
#include "tools/Map.h"

namespace client
{
    class PopulationUpdate;
}

namespace xml
{
    class xistream;
}

class PHY_ResourceNetworkType;

// =============================================================================
/** @class  MIL_InhabitantSatisfactions
    @brief  MIL_InhabitantSatisfactions
*/
// Created: JSR 2011-01-27
// =============================================================================
class MIL_InhabitantSatisfactions : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_InhabitantSatisfactions();
    explicit MIL_InhabitantSatisfactions( xml::xistream& xis );
    virtual ~MIL_InhabitantSatisfactions();
    //@}

    //! @name Operations
    //@{
    void SendFullState( client::PopulationUpdate& msg ) const;
    void UpdateNetwork( client::PopulationUpdate& msg ) const;
    void ComputeHealthSatisfaction( float healthCount );
    void SetLodgingSatisfaction( float occupationFactor );
    void ComputeMotivationSatisfactions( const std::map< std::string, unsigned int >& occupations, unsigned long living );
    void SetResourceSatisfaction( const PHY_ResourceNetworkType& resource, float satisfaction );
    void IncreaseSafety( float gainPerHour );
    void DecreaseSafety( float lossByFire );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, float >  T_Motivations;
    typedef tools::Map< const PHY_ResourceNetworkType*, float > T_Resources;
    //@}

private:
    //! @name Member data
    //@{
    float healthNeed_;
    float health_;
    float safety_;
    float lodging_;
    T_Motivations motivations_;
    T_Resources resources_;
    mutable float lastSafety_;
    mutable bool healthChanged_;
    mutable bool lodgingChanged_;
    mutable bool resourceChanged_;
    mutable bool motivationChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_InhabitantSatisfactions )

#endif // __MIL_InhabitantSatisfactions_h_
