// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Displayer_ABC_h_
#define __Displayer_ABC_h_

#include "VirtualTemplate.h"

class QString;

// =============================================================================
/** @class  Displayer_ABC
    @brief  Displayer_ABC
*/
// Created: AGE 2006-02-21
// =============================================================================
class Displayer_ABC : public VirtualTemplate< Displayer_ABC > 
{
    friend VirtualTemplate< Displayer_ABC >;

public:
    //! @name Constructors/Destructor
    //@{
             Displayer_ABC();
    virtual ~Displayer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Hide() = 0;
    template< typename T >
    Displayer_ABC& Display( const T& element );

    template< typename T >
    Displayer_ABC& Display( const char* name, const T& element );

    template< typename T >
    Displayer_ABC& Start( const T& element );
    template< typename T >
    Displayer_ABC& Add( const T& element );
    Displayer_ABC& End();

    template< typename T >
    void AddToDisplay( const T& element );
    void AddToDisplay( const QString& element );

    virtual Displayer_ABC& Group( const char* name ) { return SubItem( name ); };
    virtual Displayer_ABC& Item( const char* name ) { return SubItem( name ); };
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename T >
    void DisplayNotImplemented()
    {
        FormatterNotImplemented().Error( *this, typeid( T ) );
    }
    Displayer_ABC& NotToBeCalled( const char* ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Displayer_ABC( const Displayer_ABC& );            //!< Copy constructor
    Displayer_ABC& operator=( const Displayer_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void Call( const T& element );
    //@}

    //! @name Operations
    //@{
    virtual Displayer_ABC& SubItem( const char* name ) = 0;
    virtual void StartDisplay() = 0;
    virtual void DisplayFormatted( const QString& formatted ) = 0;
    virtual void EndDisplay() = 0;
    //@}
};

#include "Displayer_ABC.inl"

#endif // __Displayer_ABC_h_
