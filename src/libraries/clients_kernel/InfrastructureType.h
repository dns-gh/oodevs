// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __KernelInfrastructureType_h_
#define __KernelInfrastructureType_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
// =============================================================================
/** @class  InfrastructureType
@brief  InfrastructureType
*/
// Created: SLG 2010-12-29
// =============================================================================
class InfrastructureType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InfrastructureType( xml::xistream& xis );
    virtual ~InfrastructureType();
    //@}

public:
    //! @name Operations
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    const std::string& GetSymbol() const;
    bool IsMedical() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    std::string symbol_;
    bool medical_;
    static unsigned int nNextID_;
    //@}
};

}

#endif // __InfrastructureType_h_
