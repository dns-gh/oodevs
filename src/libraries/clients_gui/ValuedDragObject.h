// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValuedDragObject_h_
#define __ValuedDragObject_h_

namespace gui
{

// =============================================================================
/** @class  ValuedDragObject
    @brief  ValuedDragObject
*/
// Created: AGE 2007-10-15
// =============================================================================
class ValuedDragObject : public Q3StoredDrag
{
public:
    //! @name Statics
    //@{
    template< typename T >
    static bool Provides( QDropEvent* event );

    template< typename T >
    static T* GetValue( QDropEvent* event );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    template< typename T >
    explicit ValuedDragObject( const T* element, QWidget* dragSource = 0 );
    virtual ~ValuedDragObject() {}
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    static const char* MimeType( const T* /*element*/ )
    {
        return typeid( T ).name();
    }
    template< typename T >
    static const char* MimeType()
    {
        return typeid( T ).name();
    }
    //@}

    //! @name Copy/Assignment
    //@{
    ValuedDragObject( const ValuedDragObject& );            //!< Copy constructor
    ValuedDragObject& operator=( const ValuedDragObject& ); //!< Assignment operator
    //@}
};

// -----------------------------------------------------------------------------
// Name: ValuedDragObject constructor
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
template< typename T >
ValuedDragObject::ValuedDragObject( const T* element, QWidget* dragSource /* = 0*/ )
    : Q3StoredDrag( MimeType( element ), dragSource )
{
    const QByteArray pBytes( reinterpret_cast< const char* >( &element ), sizeof( T** ) );
    setEncodedData( pBytes );
}

// -----------------------------------------------------------------------------
// Name: ValuedDragObject::Provides
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
template< typename T >
bool ValuedDragObject::Provides( QDropEvent* event )
{
    return event && event->provides( MimeType< T >() );
}

// -----------------------------------------------------------------------------
// Name: ValuedDragObject::GetValue
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
template< typename T >
T* ValuedDragObject::GetValue( QDropEvent* event )
{
    if( Provides< T >( event ) )
    {
        QByteArray tmp = event->encodedData( MimeType< T >() );
        return *reinterpret_cast< T** >( tmp.data() );
    }
    return 0;
}

}

#endif // __ValuedDragObject_h_
