// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionItemBuilder_ABC_h_
#define __AfterActionItemBuilder_ABC_h_

class AfterActionItem_ABC;

// =============================================================================
/** @class  AfterActionBuilder_ABC
    @brief  AfterActionBuilder_ABC
    // $$$$ AGE 2007-09-27: More like a visitor
*/
// Created: AGE 2007-09-18
// =============================================================================
class AfterActionBuilder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionBuilder_ABC() {};
    virtual ~AfterActionBuilder_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Start( const std::string& name ) = 0;
    virtual void AddInput( const std::string& type ) = 0;
    virtual void AddOutput( const std::string& type ) = 0;
    virtual void AddParameter( const std::string& type, const std::string& name ) = 0;
    virtual void Connect( AfterActionItem_ABC* target, int targetSlot ) = 0;
    //@}
};

#endif // __AfterActionItemBuilder_ABC_h_
