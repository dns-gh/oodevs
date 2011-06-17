// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Dynamic_h
#define plugins_hla_Dynamic_h

#include <boost/noncopyable.hpp>
#include <memory>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Dynamic
    @brief  Dynamic
*/
// Created: SLI 2011-06-17
// =============================================================================
template< typename T >
class Dynamic : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Dynamic( const T& value )
        : value_  ( new T( value ) )
        , changed_( false )
    {
        // NOTHING
    }
    virtual ~Dynamic()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Update( const T& value )
    {
        value_.reset( new T( value ) );
        changed_ = true;
    }
    bool HasChanged() const
    {
        return changed_;
    }
    template< typename Serializer >
    void Serialize( Serializer& serializer ) const
    {
        serializer << *value_;
        changed_ = false;
    }
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< T > value_;
    mutable bool changed_;
    //@}
};

}
}

#endif // plugins_hla_Dynamic_h
