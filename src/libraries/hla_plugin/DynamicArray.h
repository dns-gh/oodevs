// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_DynamicArray_h
#define plugins_hla_DynamicArray_h

#include <boost/noncopyable.hpp>
#include <vector>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  DynamicArray
    @brief  Dynamic array
*/
// Created: SLI 2011-06-17
// =============================================================================
template< typename T >
class DynamicArray : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    DynamicArray()
        : changed_( false )
    {
        // NOTHING
    }
    virtual ~DynamicArray()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Update( const std::vector< T >& values )
    {
        values_ = values;
        changed_ = true;
    }
    bool HasChanged() const
    {
        return changed_;
    }
    template< typename Serializer >
    void Serialize( Serializer& serializer ) const
    {
        for( std::vector< T >::const_iterator it = values_.begin(); it != values_.end(); ++it )
            serializer << *it;
        changed_ = false;
    }
    //@}

private:
    //! @name Member data
    //@{
    mutable bool changed_;
    std::vector< T > values_;
    //@}
};

}
}

#endif // plugins_hla_DynamicArray_h
