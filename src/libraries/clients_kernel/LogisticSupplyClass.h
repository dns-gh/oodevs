// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LogisticSupplyClass_h_
#define __LogisticSupplyClass_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  LogisticSupplyClass
    @brief  LogisticSupplyClass
*/
// Created: JSR 2010-06-07
// =============================================================================
class LogisticSupplyClass
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticSupplyClass( xml::xistream& input );
    virtual ~LogisticSupplyClass();
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

#endif // __LogisticSupplyClass_h_
