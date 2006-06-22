// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Value_h_
#define __Value_h_

// =============================================================================
/** @class  Value
    @brief  Value
*/
// Created: AGE 2006-06-22
// =============================================================================
class Value
{

public:
    //! @name Constructors/Destructor
    //@{
             Value();
    virtual ~Value();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Value( const Value& );            //!< Copy constructor
    Value& operator=( const Value& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __Value_h_
