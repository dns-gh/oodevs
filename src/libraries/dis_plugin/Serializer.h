// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Serializer_h_
#define __Serializer_h_

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  Serializer
    @brief  Serializer
*/
// Created: AGE 2008-03-10
// =============================================================================
class Serializer
{

public:
    //! @name Constructors/Destructor
    //@{
    template< unsigned N >
    explicit Serializer( char(&data)[N] )
        : data_( data )
        , size_( N )
        , offset_( 0 ) {}
    virtual ~Serializer();
    //@}

    //! @name Operations
    //@{
    Serializer& operator<<( const float& r );
    Serializer& operator<<( const double& r );
    Serializer& operator<<( const char& c );
    Serializer& operator<<( const unsigned char& c );
    Serializer& operator<<( const short& n );
    Serializer& operator<<( const unsigned short& n );
    Serializer& operator<<( const long& n );
    Serializer& operator<<( const unsigned long& n );

    template< typename T, unsigned N >
    Serializer& operator<<( const T (&buffer)[N] )
    {
        for( unsigned i = 0; i < N; ++i )
            *this << buffer[i];
        return *this;
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Serializer( const Serializer& );            //!< Copy constructor
    Serializer& operator=( const Serializer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void Write( const T& data, unsigned size );
    //@}

private:
    //! @name Member data
    //@{
    char* data_;
    unsigned size_;
    unsigned offset_;
    //@}
};

}
}

#endif // __Serializer_h_
