// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_MissionParameterVisitor_ABC_h_
#define __MIL_MissionParameterVisitor_ABC_h_

class MIL_MissionParameter_ABC;
class MIL_OrderTypeParameter;

// =============================================================================
/** @class  MIL_MissionParameterVisitor_ABC
    @brief  MIL Mission parameter visitor definition
*/
// Created: LDC 2009-08-11
// =============================================================================
class MIL_MissionParameterVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_MissionParameterVisitor_ABC() {}
    virtual ~MIL_MissionParameterVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Accept( const std::string& dianame, const MIL_OrderTypeParameter& type, MIL_MissionParameter_ABC& element ) = 0;
    //@}
};

#endif // __MIL_MissionParameterVisitor_ABC_h_
