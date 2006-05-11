// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InternalLinks_h_
#define __InternalLinks_h_

// =============================================================================
/** @class  InternalLinks
    @brief  InternalLinks
*/
// Created: AGE 2006-05-11
// =============================================================================
class InternalLinks
{
public:
    //! @name Operations
    //@{
    template< typename T >
    static QString CreateLink( const T& entity, const QString& message )
    {
        return Embed( message, GetClass( entity ), GetIdentifier( entity ) );
    }
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    static QString GetClass( const T& )
    {
        return Cleanup( typeid( T ).name() );
    }

    template< typename T >
    static QString GetIdentifier( const T& entity )
    {
        return GetAddress( &entity );// $$$$ AGE 2006-05-11: => "Portable link" ? qui prendrait l'ID ?
    }
    static QString Cleanup( const QString& className );
    static QString GetAddress( const void* add );
    static QString Embed( const QString& message, const QString& classId, const QString& id );
    //@}
};

#endif // __InternalLinks_h_
