// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Resource_h_
#define __Resource_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  Resource
    @brief  Resource
    @par    Using example
    @code
    Resource;
    @endcode
*/
// Created: HME 2005-11-04
// =============================================================================
class Resource
{

public:
    //! @name Constructors/Destructor
    //@{
    Resource( uint nID, std::string strName, std::string strDot );
    virtual ~Resource();
    
    const uint              GetId() const;
    const std::string       GetName() const;
    const std::string       GetDotationName() const;

private:
    std::string strName_;
    std::string strDot_;
    uint        nId_;
};



#include "Resource.inl"

#endif // __Resource_h_
