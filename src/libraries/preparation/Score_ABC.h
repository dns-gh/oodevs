// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Score_ABC_h_
#define __Score_ABC_h_

namespace xml
{
    class xostream;
}

class IndicatorVariablesVisitor_ABC;

// =============================================================================
/** @class  Score_ABC
    @brief  Score_ABC
*/
// Created: SBO 2009-04-16
// =============================================================================
class Score_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Score_ABC() {}
    virtual ~Score_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const = 0;
    virtual QString GetFormula() const = 0;
    virtual void Accept( IndicatorVariablesVisitor_ABC& visitor ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Score_ABC( const Score_ABC& );            //!< Copy constructor
    Score_ABC& operator=( const Score_ABC& ); //!< Assignment operator
    //@}
};

#endif // __Score_ABC_h_
