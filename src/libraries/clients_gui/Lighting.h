// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lighting_h_
#define __Lighting_h_

#include "graphics/Lighting_ABC.h"
class FixedLighting;

namespace gui
{

// =============================================================================
/** @class  Lighting
    @brief  Lighting
*/
// Created: AGE 2007-01-02
// =============================================================================
class Lighting : public Lighting_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Lighting();
    virtual ~Lighting();
    //@}

    //! @name Operations
    //@{
    void SetAmbient( float r, float g, float b );
    void SetDiffuse( float r, float g, float b );
    void SetFixedLighting( const geometry::Vector3f& direction );

    virtual void Set();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Lighting( const Lighting& );            //!< Copy constructor
    Lighting& operator=( const Lighting& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    Lighting_ABC* current_;
    std::auto_ptr< FixedLighting > fixed_;
    //@}
};

}

#endif // __Lighting_h_
