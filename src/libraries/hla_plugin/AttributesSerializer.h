// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AttributesSerializer_h
#define plugins_hla_AttributesSerializer_h

#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace hla
{
    class UpdateFunctor_ABC;
    class Serializer_ABC;
}

namespace plugins
{
namespace hla
{
    template< typename T >
    struct Wrapper
    {
    public:
        explicit Wrapper( const T& value )
            : value_( value )
        {}
        template< typename Archive >
        void Serialize( Archive& serializer ) const
        {
            serializer << value_;
        }
    private:
        T value_;
    };
    template< typename T, typename SzT = uint32_t >
    struct VectorWrapper
    {
    public:
        typedef SzT SizeType;
        typedef std::vector< T > VectorType;

        explicit VectorWrapper( const VectorType& value )
            : value_( value )
        {}
        template< typename Archive >
        void Serialize( Archive& serializer ) const
        {
            serializer << static_cast< SizeType >( value_.size() );
            serializer << value_;
        }
        const VectorType& operator*() const
        {
            return value_;
        }
    private:
        VectorType value_;
    };

// =============================================================================
/** @class  AttributesSerializer
    @brief  Attributes serializer
*/
// Created: SLI 2011-06-20
// =============================================================================
class AttributesSerializer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AttributesSerializer();
    virtual ~AttributesSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const;
    template< typename T >
    void Register( const std::string& name, const T& value )
    {
        boost::shared_ptr< T > copy( new T( value ) );
        T_Serializer serializer = boost::bind( &T::Serialize< ::hla::Serializer_ABC >, copy, _1 );
        attributes_.push_back( std::make_pair( name, serializer ) );
    }
    template< typename T, typename F >
    void Register( const std::string& name, const T& value, const F& szr )
    {
        boost::shared_ptr< T > copy( new T( value ) );
        T_Serializer serializer = boost::bind< void >( szr, copy, _1 );
        attributes_.push_back( std::make_pair( name, serializer ) );
    }
    template< typename T >
    void Update( const std::string& name, const T& value )
    {
        boost::shared_ptr< T > copy( new T( value ) );
        T_Serializer serializer = boost::bind( &T::Serialize< ::hla::Serializer_ABC >, copy, _1 );
        for( T_Attributes::iterator it = attributes_.begin(); it != attributes_.end(); ++it )
            if( it->first == name )
                it->second = serializer;
        updates_.push_back( name );
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::function< void( ::hla::Serializer_ABC& ) > T_Serializer;
    typedef std::pair< std::string, T_Serializer > T_Attribute;
    typedef std::vector< T_Attribute > T_Attributes;
    typedef std::vector< std::string > T_Updates;
    //@}

private:
    //! @name Member data
    //@{
    T_Attributes attributes_;
    mutable T_Updates updates_;
    //@}
};

}
}

#endif // __AttributesSerializer_h_
