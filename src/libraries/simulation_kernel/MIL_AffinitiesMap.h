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

#include <map>
#include "MIL.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace client
{
    class PopulationUpdate;
}

namespace sword
{
    class UnitMagicAction;
}

// =============================================================================
/** @class  MIL_AffinitiesMap
    @brief  MIL_AffinitiesMap
*/
// Created: ABR 2011-02-03
// =============================================================================
class MIL_AffinitiesMap : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_AffinitiesMap();
             MIL_AffinitiesMap( xml::xistream& xis );
    virtual ~MIL_AffinitiesMap();
    //@}

    //! @name Operations
    //@{
    void WriteODB( xml::xostream& xos ) const;
    void SendFullState( client::PopulationUpdate& msg ) const;
    void UpdateNetwork( client::PopulationUpdate& msg );
    void OnReceiveMsgChangeAffinities( const sword::UnitMagicAction& msg );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, float >     T_Affinities;
    typedef T_Affinities::iterator              IT_Affinities;
    typedef T_Affinities::const_iterator       CIT_Affinities;
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

#endif // __MIL_AffinitiesMap_h_
