// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RenderingContext_ABC_h_
#define __RenderingContext_ABC_h_

#include <stdexcept>

namespace svg
{
    class Property_ABC;
    class References_ABC;

// =============================================================================
/** @class  RenderingContext_ABC
    @brief  Rendering context definition
*/
// Created: AGE 2006-08-14
// =============================================================================
class RenderingContext_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RenderingContext_ABC() {};
    virtual ~RenderingContext_ABC() {};
    //@}

    // $$$$ AGE 2006-09-11: Perf issues with std::map< std::string >...
    //! @name Types
    //@{
    enum E_Properties
    {
        fill,
        fillRule,
        fillOpacity,
        stroke,
        strokeWidth,
        strokeOpacity,
        strokeDasharray,
        color,
        fontFamily,
        fontWeight,
        fontSize,
        textAnchor,

        nbrProperty
    };

    //@}

    //! @name Operations
    //@{
    virtual float ExpectedPrecision() const = 0;
    virtual float Pixels() const = 0;
    virtual float LineWidthFactor() const = 0;

    virtual bool IsPickingMode() const = 0;
    virtual void EnablePickingMode( float lineWithFactor = 1.f ) = 0;
    virtual void DisablePickingMode() = 0;

    virtual bool SetupFill( References_ABC& references ) const = 0;
    virtual bool SetupStroke( References_ABC& references ) const = 0;

    virtual void PushProperty( E_Properties name, Property_ABC& value ) = 0;
    virtual void PopProperty ( E_Properties name ) = 0;

    template< typename T >
    const T& GetProperty( E_Properties name ) const;
    virtual const Property_ABC* FindProperty( E_Properties name ) const = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: RenderingContext_ABC::GetProperty
// Created: AGE 2006-08-14
// -----------------------------------------------------------------------------
template< typename T >
const T& RenderingContext_ABC::GetProperty( E_Properties name ) const
{
    const T* property = static_cast< const T* >( FindProperty( name ) );
    return *property;
}

}

#endif // __RenderingContext_ABC_h_
