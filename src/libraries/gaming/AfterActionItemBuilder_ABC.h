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

// =============================================================================
/** @class  AfterActionItemBuilder_ABC
    @brief  AfterActionItemBuilder_ABC
*/
// Created: AGE 2007-09-18
// =============================================================================
class AfterActionItemBuilder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionItemBuilder_ABC() {};
    virtual ~AfterActionItemBuilder_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Start( const std::string& name ) = 0;
    virtual void AddInput( const std::string& type ) = 0;
    virtual void AddOutput( const std::string& type ) = 0;
    virtual void AddParameter( const std::string& type, const std::string& name ) = 0;
    //@}
};

#endif // __AfterActionItemBuilder_ABC_h_
