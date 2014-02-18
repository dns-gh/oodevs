// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Color_h_
#define __Color_h_

#include "Paint_ABC.h"

namespace svg
{

// =============================================================================
/** @class  Color
    @brief  Color
*/
// Created: AGE 2006-08-15
// =============================================================================
class Color : public Paint_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Color( const std::string& name );
    virtual ~Color();
    //@}

    //! @name Modifiers
    //@{
    void Set( float r, float g, float b );
    //@}

    //! @name Operations
    //@{
    virtual bool Setup( References_ABC& references, float opacity ) const;
    virtual void ReachTop( const RenderingContext_ABC& context );
    virtual void LeaveTop( const RenderingContext_ABC& context );
    virtual Color& Clone() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Color( const Color& );            //!< Copy constructor
    Color& operator=( const Color& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    std::string RemoveNonNumeric( const std::string& in );
    //@}

private:
    //! @name Member data
    //@{
    float r_, g_, b_;
    bool pickingMode_;
    const RenderingContext_ABC* context_;
    //@}
};

}

#endif // __Color_h_
