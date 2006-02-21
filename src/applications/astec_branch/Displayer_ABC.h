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
    template< typename T >
    void Display( const T& element );

    template< typename T >
    void AddToDisplay( const T& element );
    void AddToDisplay( const QString& element );
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
    virtual void StartDisplay() = 0;
    virtual void DisplayFormatted( const QString& formatted ) = 0;
    virtual void EndDisplay() = 0;
    //@}
};

#include "Displayer_ABC.inl"

#endif // __Displayer_ABC_h_
