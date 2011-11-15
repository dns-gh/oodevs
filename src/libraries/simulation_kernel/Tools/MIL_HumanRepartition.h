// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_HumanRepartition_h_
#define __MIL_HumanRepartition_h_

#include "MIL.h"

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  MIL_HumanRepartition
    @brief  MIL_HumanRepartition
*/
// Created: MMC 2011-10-07
// =============================================================================
class MIL_HumanRepartition : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_HumanRepartition();
             MIL_HumanRepartition( const MIL_HumanRepartition& humanRepartition );
    explicit MIL_HumanRepartition( xml::xistream& xis );
    virtual ~MIL_HumanRepartition();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SendFullState( T& msg ) const;
    template< typename T >
    void UpdateNetwork( T& msg );
    bool HasChanged() const;
    bool IsExist() const;
    float GetMale() const;
    float GetFemale() const;
    float GetChildren() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    float male_;
    float female_;
    float children_;
    bool hasChanged_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_HumanRepartition )

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::SendFullState
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
template< typename T >
void MIL_HumanRepartition::SendFullState( T& msg ) const
{
    sword::HumanRepartition& repartition = *msg().mutable_repartition();
    repartition.set_male( male_ );
    repartition.set_female( female_ );
    repartition.set_children( children_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::UpdateNetwork
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
template< typename T >
void MIL_HumanRepartition::UpdateNetwork( T& msg )
{
    if( hasChanged_ )
    {
        SendFullState( msg );
        hasChanged_ = false;
    }
}

#endif // __MIL_HumanRepartition_h_
