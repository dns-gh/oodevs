// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_IntoxicationEffect_h_
#define __MIL_IntoxicationEffect_h_

#include <boost/noncopyable.hpp>
#include <boost/serialization/split_member.hpp>
#include <vector>

class MIL_PopulationHumans;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_IntoxicationEffect
    @brief  MIL_IntoxicationEffect
*/
// Created: LGY 2011-03-31
// =============================================================================
class MIL_IntoxicationEffect : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_IntoxicationEffect( MIL_PopulationHumans& humans, double delay, unsigned int time );
             MIL_IntoxicationEffect();
    virtual ~MIL_IntoxicationEffect();
    //@}

    //! @name Operations
    //@{
    void Add( double woundedPercentage, double deadPercentage );
    void Update( unsigned int time );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< double, double > T_Effect;
    typedef std::vector< T_Effect >     T_Effects;
    //@}

private:
    //! @name Member data
    //@{
    double delay_;
    MIL_PopulationHumans* humans_;
    unsigned int time_;
    T_Effects effects_;
    //@}
};

#endif // __MIL_IntoxicationEffect_h_
