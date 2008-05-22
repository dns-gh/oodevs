// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportingData_h_
#define __ReportingData_h_

namespace xml
{
    class xostream;
}

namespace dispatcher
{
    class Agent;
    class Automat;
}

namespace bml
{

// =============================================================================
/** @class  ReportingData
    @brief  ReportingData
*/
// Created: SBO 2008-05-22
// =============================================================================
class ReportingData
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ReportingData( const dispatcher::Agent& entity );
    explicit ReportingData( const dispatcher::Automat& entity );
    virtual ~ReportingData();
    //@}

    //! @name Operations
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const ReportingData& data );    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportingData( const ReportingData& );            //!< Copy constructor
    ReportingData& operator=( const ReportingData& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string reportDatetime_;
    std::string organisation_;
    //@}
};

}

#endif // __ReportingData_h_
