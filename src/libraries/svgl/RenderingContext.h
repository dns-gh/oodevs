// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RenderingContext_h_
#define __RenderingContext_h_

#include "RenderingContext_ABC.h"
#include "BoundingBox.h"
#include <map>
#include <vector>

namespace svg
{

// =============================================================================
/** @class  RenderingContext
    @brief  RenderingContext
*/
// Created: AGE 2006-08-15
// =============================================================================
class RenderingContext : public RenderingContext_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RenderingContext();
    explicit RenderingContext( float precision );
    virtual ~RenderingContext();
    //@}

    //! @name Modifiers
    //@{
    void SetViewport( const geometry::BoundingBox& viewport, unsigned w, unsigned h );
    //@}

    //! @name Operations
    //@{
    virtual float ExpectedPrecision() const;
    virtual float Pixels() const;
    virtual bool IsPickingMode() const;
    virtual float LineWidthFactor() const;
    virtual void EnablePickingMode( float lineWithFactor = 1.f );
    virtual void DisablePickingMode();

    virtual bool SetupFill( References_ABC& references ) const;
    virtual bool SetupStroke( References_ABC& references ) const;

    virtual void PushProperty( E_Properties name, Property_ABC& value );
    virtual void PopProperty ( E_Properties name );
    virtual const Property_ABC* FindProperty( E_Properties name ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RenderingContext( const RenderingContext& );            //!< Copy constructor
    RenderingContext& operator=( const RenderingContext& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Property_ABC* >     T_Properties;
    typedef std::vector< T_Properties >      T_PropertyMap;
    typedef T_PropertyMap::const_iterator  CIT_PropertyMap;
    //@}

    //! @name Helpers
    //@{
    void PushDefaultValues();
    //@}

private:
    //! @name Member data
    //@{
    geometry::BoundingBox viewport_;
    float pixels_;
    float expectedPrecision_;
    T_PropertyMap properties_;
    bool pickingMode_;
    float lineWithFactor_;
    //@}
};

}

#endif // __RenderingContext_h_
