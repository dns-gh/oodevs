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

#include "tools/VirtualTemplate.h"
#include <boost/noncopyable.hpp>

class QString;

namespace kernel
{

// =============================================================================
/** @class  Displayer_ABC
    @brief  Displayer_ABC
*/
// Created: AGE 2006-02-21
// =============================================================================
class Displayer_ABC : public tools::VirtualTemplate< Displayer_ABC >
                    , private boost::noncopyable
{
    friend tools::VirtualTemplate< Displayer_ABC >;

public:
    //! @name Constructors/Destructor
    //@{
             Displayer_ABC();
    virtual ~Displayer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Clear() = 0;
    virtual void Hide() = 0;
    template< typename T >
    Displayer_ABC& Display( const T& element );

    template< typename T >
    Displayer_ABC& Display( const QString& name, const T& element );

    template< typename T >
    Displayer_ABC& Start( const T& element );
    template< typename T >
    Displayer_ABC& Add( const T& element );
    Displayer_ABC& End();

    template< typename T >
    void AddToDisplay( const T& element );
    void AddToDisplay( const QString& element );

    virtual Displayer_ABC& Group( const QString& name ) { return SubItem( name ); };
    virtual Displayer_ABC& Item( const QString& name ) { return SubItem( name ); };
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
    //! @name Copy/Assignment
    //@{
    Displayer_ABC( const Displayer_ABC& );            //!< Copy constructor
    Displayer_ABC& operator=( const Displayer_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void Call( const T& element );
    //@}

    //! @name Operations
    //@{
    virtual Displayer_ABC& SubItem( const QString& name ) = 0;
    virtual void StartDisplay() = 0;
    virtual void DisplayFormatted( const QString& formatted ) = 0;
    virtual void EndDisplay() = 0;
    //@}
};

}

#include "Displayer_ABC.inl"

#endif // __Displayer_ABC_h_
