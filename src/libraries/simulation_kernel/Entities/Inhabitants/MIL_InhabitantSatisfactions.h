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

#include <boost/noncopyable.hpp>
#include "MIL.h"

namespace client
{
    class PopulationUpdate;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  MIL_InhabitantSatisfactions
    @brief  MIL_InhabitantSatisfactions
*/
// Created: JSR 2011-01-27
// =============================================================================
class MIL_InhabitantSatisfactions : boost::noncopyable
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
    void ComputeLodgingSatisfaction( unsigned long living, unsigned int totalOccupation );
    void ComputeMotivationSatisfactions( const std::map< std::string, unsigned int >& occupations, unsigned long living );
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
    typedef T_Motivations::const_iterator CIT_Motivations;
    //@}

private:
    //! @name Member data
    //@{
    float healthNeed_;
    float health_;
    float safety_;
    float lodging_;
    T_Motivations motivations_;
    mutable bool healthChanged_;
    mutable float lastSafety_;
    mutable bool lodgingChanged_;
    mutable bool motivationChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_InhabitantSatisfactions )

#endif // __MIL_InhabitantSatisfactions_h_
