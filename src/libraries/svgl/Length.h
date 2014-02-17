// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Length_h_
#define __Length_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  Length
    @brief  Length
*/
// Created: AGE 2006-09-04
// =============================================================================
class Length : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Length( const std::string& width );
    virtual ~Length();
    //@}

    //! @name Operations
    //@{
    virtual Length& Clone() const;
    virtual void ReachTop( const RenderingContext_ABC& );
    virtual void LeaveTop( const RenderingContext_ABC& context );
    virtual void SetupLineWidth() const;

    float GetWidth() const;
    bool  IsFixed() const;
    //@}

private:
    //! @name Member data
    //@{
    float baseWidth_;
    float pixelWidth_;
    bool fixed_;
    float factor_;
    //@}
};

}

#endif // __Length_h_
