// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __VolumeType_h_
#define __VolumeType_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  VolumeType
    @brief  VolumeType
*/
// Created: JSR 2010-06-07
// =============================================================================
class VolumeType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit VolumeType( xml::xistream& input, unsigned int id );
    virtual ~VolumeType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    unsigned int GetId() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned int id_;
    //@}
};

}

#endif // __VolumeType_h_
