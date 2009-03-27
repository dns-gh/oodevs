// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectBuilder_ABC_h_
#define __MIL_ObjectBuilder_ABC_h_

class MIL_ObjectType_ABC;
class Object;

// =============================================================================
/** @class  MIL_ObjectBuilder_ABC
    @brief  MIL_ObjectBuilder_ABC
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            MIL_ObjectBuilder_ABC() {}
    virtual ~MIL_ObjectBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MIL_ObjectType_ABC& GetType() const = 0;
    virtual void Build( Object& object ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectBuilder_ABC( const MIL_ObjectBuilder_ABC& );            //!< Copy constructor
    MIL_ObjectBuilder_ABC& operator=( const MIL_ObjectBuilder_ABC& ); //!< Assignment operator
    //@}
};

#endif // __MIL_ObjectBuilder_ABC_h_
