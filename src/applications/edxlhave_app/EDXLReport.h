// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __edxl_EDXLReport_h_
#define __edxl_EDXLReport_h_

#include "ReportBuilder_ABC.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xostream;
}

namespace sword
{
    class ObjectAttributeMedicalTreatment;
}

namespace edxl
{
    class Model;
    class Publisher_ABC;
    class HospitalUpdateReport;

// =============================================================================
/** @class  EDXLReport
    @brief  EDXLReport
*/
// Created: JCR 2010-06-10
// =============================================================================
class EDXLReport : public ReportBuilder_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EDXLReport( const Model& model );
    virtual ~EDXLReport();
    //@}

    //! @name Operations
    //@{
    virtual void CreateReport( unsigned int id, const std::string& name, const sword::ObjectAttributeMedicalTreatment& medical );
    virtual void Publish( Publisher_ABC& publisher );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EDXLReport( const EDXLReport& );            //!< Copy constructor
    EDXLReport& operator=( const EDXLReport& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void PublishHospitalStatus( xml::xostream& xos );
    //@}

private:

    //! @name Types
    //@{
    typedef boost::shared_ptr< HospitalUpdateReport >   T_HospitalUpdateReportPtr;
    typedef std::map< unsigned int, T_HospitalUpdateReportPtr >  T_HospitalUpdateReportMap;
    typedef std::vector< T_HospitalUpdateReportPtr >    T_HospitalUpdateReportVector;
    //@}

private:
    //! @name Member data
    //@{
    T_HospitalUpdateReportMap reports_;
    const Model& model_;
    //@}
};

} // end namespace edxl

#endif // __EDXLReport_h_
