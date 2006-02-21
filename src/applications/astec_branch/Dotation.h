// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotation_h_
#define __Dotation_h_

class DotationType;

// =============================================================================
/** @class  Dotation
    @brief  Dotation
*/
// Created: AGE 2006-02-21
// =============================================================================
class Dotation
{

public:
    //! @name Constructors/Destructor
    //@{
             Dotation( const DotationType& type, unsigned int quantity );
    virtual ~Dotation();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Dotation( const Dotation& );            //!< Copy constructor
    Dotation& operator=( const Dotation& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

public:
    //! @name Member data
    //@{
    const DotationType& type_;
    unsigned int quantity_;
    //@}
};

#endif // __Dotation_h_
