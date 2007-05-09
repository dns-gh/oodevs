// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InputBinaryWrapper_h_
#define __InputBinaryWrapper_h_

// =============================================================================
/** @class  InputBinaryWrapper
    @brief  InputBinaryWrapper
*/
// Created: AGE 2007-04-10
// =============================================================================
class InputBinaryWrapper
{

public:
    //! @name Constructors/Destructor
    //@{
             InputBinaryWrapper();
    virtual ~InputBinaryWrapper();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InputBinaryWrapper( const InputBinaryWrapper& );            //!< Copy constructor
    InputBinaryWrapper& operator=( const InputBinaryWrapper& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __InputBinaryWrapper_h_
