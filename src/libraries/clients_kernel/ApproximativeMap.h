// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ApproximativeMap_h_
#define __ApproximativeMap_h_

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

namespace kernel
{

namespace internal
{
    int Find( const std::string& key, const std::vector< std::string >& candidates, int threshold );
}

// =============================================================================
/** @class  ApproximativeMap
    @brief  Approximative map
*/
// Created: AGE 2008-04-04
// =============================================================================
template< typename T >
class ApproximativeMap : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ApproximativeMap( int defaultThreshold = 0 );
    virtual ~ApproximativeMap();
    //@}

    //! @name Operations
    //@{
    void Add( const std::string& key, const T& value );
    const T* Find( const std::string& key ) const;
    const T* Find( const std::string& key, int threshold ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::string > keys_;
    std::vector< T > values_;
    int defaultThreshold_;
    //@}
};

}

#include "ApproximativeMap.inl"

#endif // __ApproximativeMap_h_
