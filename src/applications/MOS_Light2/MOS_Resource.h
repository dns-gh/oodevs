// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_Resource_h_
#define __MOS_Resource_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MOS_Resource
    @brief  MOS_Resource
    @par    Using example
    @code
    MOS_Resource;
    @endcode
*/
// Created: HME 2005-11-04
// =============================================================================
class MOS_Resource
{

public:
    //! @name Constructors/Destructor
    //@{
    MOS_Resource( uint nID, std::string strName, std::string strDot );
    virtual ~MOS_Resource();
    
    const uint              GetId() const;
    const std::string       GetName() const;
    const std::string       GetDotationName() const;

private:
    std::string strName_;
    std::string strDot_;
    uint        nId_;
};



#include "MOS_Resource.inl"

#endif // __MOS_Resource_h_
