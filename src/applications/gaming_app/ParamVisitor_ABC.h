// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamVisitor_ABC_h_
#define __ParamVisitor_ABC_h_

class Param_ABC;

// =============================================================================
/** @class  ParamVisitor_ABC
    @brief  ParamVisitor_ABC
*/
// Created: SBO 2007-04-27
// =============================================================================
class ParamVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamVisitor_ABC() {}
    virtual ~ParamVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const Param_ABC& param ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamVisitor_ABC( const ParamVisitor_ABC& );            //!< Copy constructor
    ParamVisitor_ABC& operator=( const ParamVisitor_ABC& ); //!< Assignment operator
    //@}
};

#endif // __ParamVisitor_ABC_h_
