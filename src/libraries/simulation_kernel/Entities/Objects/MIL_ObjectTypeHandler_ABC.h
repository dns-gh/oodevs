// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectTypeHandler_ABC_h_
#define __MIL_ObjectTypeHandler_ABC_h_

class MIL_ObjectType_ABC;

// =============================================================================
/** @class  MIL_ObjectType_ABC
    @brief  MIL_ObjectType_ABC
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectTypeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            MIL_ObjectTypeHandler_ABC() {}
    virtual ~MIL_ObjectTypeHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MIL_ObjectType_ABC& GetType( const std::string& type ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectTypeHandler_ABC( const MIL_ObjectTypeHandler_ABC& );            //!< Copy constructor
    MIL_ObjectTypeHandler_ABC& operator=( const MIL_ObjectTypeHandler_ABC& ); //!< Assignment operator
    //@}
};

#endif // __MIL_ObjectType_ABC_h_
