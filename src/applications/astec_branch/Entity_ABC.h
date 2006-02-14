// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Entity_ABC_h_
#define __Entity_ABC_h_

#include "Extendable.h"
#include "InterfaceContainer.h"
#include "Updatable_ABC.h"
#include "Extension_ABC.h"

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity base class
*/
// Created: AGE 2006-02-07
// =============================================================================
class Entity_ABC : public Extendable< Extension_ABC >
                 , public InterfaceContainer< Extension_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC();
    virtual ~Entity_ABC();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void Update( const T& updateMessage ) 
    {
        const unsigned int applied = Apply( Updatable_ABC< T >::Update, updateMessage );
        if( ! applied )
            throw std::runtime_error( "Nothing could be updated with message type " + std::string( typeid( T ).name() ) );
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Entity_ABC( const Entity_ABC& );            //!< Copy constructor
    Entity_ABC& operator=( const Entity_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddExtension( Extension_ABC*& where, Extension_ABC* ext );
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __Entity_ABC_h_
