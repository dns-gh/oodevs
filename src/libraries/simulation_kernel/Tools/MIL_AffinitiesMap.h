// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AffinitiesMap_h_
#define __MIL_AffinitiesMap_h_

#include "MIL.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace sword
{
    class UnitMagicAction;
}

class MIL_Config;
// =============================================================================
/** @class  MIL_AffinitiesMap
    @brief  MIL_AffinitiesMap
*/
// Created: ABR 2011-02-03
// =============================================================================
class MIL_AffinitiesMap : private boost::noncopyable
{

public:
    //! @name Static public Member data
    //@{
    static double interrogateDelayForMinimumAffinity_;
    static double interrogateDelayForNeutralAffinity_;
    static double interrogateDelayForMaximumAffinity_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_AffinitiesMap();
    explicit MIL_AffinitiesMap( xml::xistream& xis );
    virtual ~MIL_AffinitiesMap();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SendFullState( T& msg ) const;
    template< typename T >
    void UpdateNetwork( T& msg );
    void OnReceiveMsgChangeAffinities( const sword::UnitMagicAction& msg );
    bool HasChanged() const;
    bool IsEmpty() const;
    float GetAffinity( unsigned long teamID ) const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, float > T_Affinities;
    //@}

    //! @name Helpers
    //@{
    void ReadAffinity( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_Affinities affinities_;
    bool hasChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AffinitiesMap )

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::SendFullState
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
template< typename T >
void MIL_AffinitiesMap::SendFullState( T& msg ) const
{
    for( auto it = affinities_.begin(); it != affinities_.end(); ++it )
    {
        sword::PartyAdhesion& adhesion = *msg().mutable_adhesions()->add_adhesion();
        adhesion.mutable_party()->set_id( it->first );
        adhesion.set_value( it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AffinitiesMap::UpdateNetwork
// Created: ABR 2011-02-03
// -----------------------------------------------------------------------------
template< typename T >
void MIL_AffinitiesMap::UpdateNetwork( T& msg )
{
    if( hasChanged_ )
    {
        msg().mutable_adhesions();
        SendFullState( msg );
        hasChanged_ = false;
    }
}

#endif // __MIL_AffinitiesMap_h_
