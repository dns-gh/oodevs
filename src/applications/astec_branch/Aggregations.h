// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Aggregations_h_
#define __Aggregations_h_

#include "Extension_ABC.h"

class GlTools_ABC;
class Agent;

// =============================================================================
/** @class  Aggregations
    @brief  Aggregations
*/
// Created: AGE 2006-04-11
// =============================================================================
class Aggregations : public Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Aggregations( const Agent& automat );
    virtual ~Aggregations();
    //@}

    //! @name Operations
    //@{
    void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Aggregations( const Aggregations& );            //!< Copy constructor
    Aggregations& operator=( const Aggregations& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Agent& automat_;
    //@}
};

#endif // __Aggregations_h_
