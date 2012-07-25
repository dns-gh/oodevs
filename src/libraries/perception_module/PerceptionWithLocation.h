// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_WITH_LOCATION_H
#define SWORD_PERCEPTION_PERCEPTION_WITH_LOCATION_H

#include "Perception_ABC.h"
#include <vector>
#include <memory>
#include <boost/shared_ptr.hpp>

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  PerceptionWithLocation
    @brief  Perception with location
*/
// Created: LDC 2009-07-29
// =============================================================================
template< typename T >
class PerceptionWithLocation : public Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionWithLocation();
    virtual ~PerceptionWithLocation();
    //@}

    //! @name Operations
    //@{
    void Add( std::auto_ptr< T > pLocation );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PerceptionWithLocation( const PerceptionWithLocation& );            //!< Copy constructor
    PerceptionWithLocation& operator=( const PerceptionWithLocation& ); //!< Assignment operator
    //@}

protected:
    //! @name Types
    //@{
    typedef boost::shared_ptr< T > T_Reco;
    typedef std::vector< T_Reco > T_RecoVector;
    //@}

    //! @name Member data
    //@{
    T_RecoVector recos_;
    //@}
};

}
}

#include "PerceptionWithLocation.inl"

#endif // SWORD_PERCEPTION_PERCEPTION_WITH_LOCATION_H
