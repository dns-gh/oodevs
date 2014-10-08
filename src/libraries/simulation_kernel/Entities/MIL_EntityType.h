// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __MIL_EntityType_h_
#define __MIL_EntityType_h_

class DEC_Model_ABC;

// =============================================================================
/** @class  MIL_EntityType
    @brief  The "type" of a MIL Entity_ABC instance
*/
// Created: FAI 2014-10-07
// =============================================================================
class MIL_EntityType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~MIL_EntityType() = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetID() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual std::string GetArchetypeName() const = 0;
    virtual const DEC_Model_ABC& GetModel() const = 0;
    //@}

protected:
    //! @name Constructors
    MIL_EntityType();
    //@}
};

#endif // __MIL_EntityType_h_
