// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPositions_h_
#define __PopulationPositions_h_

#include "clients_kernel/Positions.h"
class Population;

// =============================================================================
/** @class  PopulationPositions
    @brief  PopulationPositions
*/
// Created: AGE 2006-03-23
// =============================================================================
class PopulationPositions : public kernel::Positions
{
    
public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationPositions( const Population& popu ); // $$$$ AGE 2006-04-10: 
    virtual ~PopulationPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationPositions( const PopulationPositions& );            //!< Copy constructor
    PopulationPositions& operator=( const PopulationPositions& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Population& population_;
    //@}
};

#endif // __PopulationPositions_h_
