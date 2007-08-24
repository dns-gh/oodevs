// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageIdentifierFactory_h_
#define __MessageIdentifierFactory_h_


namespace tools
{
// =============================================================================
/** @class  MessageIdentifierFactory
    @brief  Message identifier factory
*/
// Created: AGE 2007-03-07
// =============================================================================
class MessageIdentifierFactory
{
public:
    //! @name Operations
    //@{
    template< typename Message >
    static unsigned long GetIdentifier( const Message& message );
    template< typename Message >
    static unsigned long GetIdentifier();
    //@}

private:
    //! @name Operations
    //@{
    static unsigned long Hash( const std::string& name )
    {
        unsigned long seed = 0;
        for(unsigned long i = 0; i < name.size(); ++i )
        {
            unsigned char current = name[ i ];
            seed ^= current + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
    //@}
};

// -----------------------------------------------------------------------------
// Name: MessageIdentifierFactory::GetIdentifier
// Created: AGE 2007-03-07
// -----------------------------------------------------------------------------
template< typename Message >
unsigned long MessageIdentifierFactory::GetIdentifier( const Message& message )
{
    return Hash( typeid( message ).name() );
}

// -----------------------------------------------------------------------------
// Name: MessageIdentifierFactory::GetIdentifier
// Created: AGE 2007-03-07
// -----------------------------------------------------------------------------
template< typename Message >
unsigned long MessageIdentifierFactory::GetIdentifier()
{
    return Hash( typeid( Message ).name() );
}

}

#endif // __MessageIdentifierFactory_h_
