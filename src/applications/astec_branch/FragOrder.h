// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FragOrder_h_
#define __FragOrder_h_

// =============================================================================
/** @class  FragOrder
    @brief  FragOrder
*/
// Created: AGE 2006-03-14
// =============================================================================
class FragOrder
{

public:
    //! @name Constructors/Destructor
    //@{
             FragOrder( const std::string& name, unsigned int id );
    virtual ~FragOrder();
    //@}

public:
    //! @name Member data
    //@{
    std::string name_;
    unsigned int id_;
    //@}
};

#endif // __FragOrder_h_
