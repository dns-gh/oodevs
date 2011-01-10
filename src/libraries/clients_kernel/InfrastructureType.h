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

#include <string>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xibufferstream;
    class xistream;
};

namespace kernel
{

// =============================================================================
/** @class  InfrastructureType
@brief  InfrastructureType
*/
// Created: SLG 2010-12-29
// =============================================================================
class InfrastructureType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InfrastructureType( xml::xistream& xis );
    virtual ~InfrastructureType();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< xml::xibufferstream > >  T_Capacities;
    typedef T_Capacities::const_iterator                                       CIT_Capacities;
    //@}


    //! @name Operations
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    xml::xistream* FindCapacity( const std::string& capacity ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfrastructureType( const InfrastructureType& );            //!< Copy constructor
    InfrastructureType& operator=( const InfrastructureType& ); //!< Assignment operator
    //@}
    //! @name Helper
    //@{
    void ReadCapacities( const std::string& capacity, xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    T_Capacities capacities_;
    static unsigned int nNextID_;
    //@}
};

}

#endif // __InfrastructureType_h_
