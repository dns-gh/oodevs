// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportTemplate_h_
#define __ReportTemplate_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#include <vector>

namespace sword
{
    class Report;
}

namespace kernel
{
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

class ReportFactory;

// =============================================================================
/** @class  ReportTemplate
    @brief  Report template
*/
// Created: SBO 2006-12-07
// =============================================================================
class ReportTemplate
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportTemplate( xml::xistream& xis, const ReportFactory& factory );
    virtual ~ReportTemplate();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    QString RenderMessage( const kernel::Entity_ABC* entity, const sword::Report& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportTemplate( const ReportTemplate& );            //!< Copy constructor
    ReportTemplate& operator=( const ReportTemplate& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    void ReadEnumeration( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< QString >             T_EnumerationValues;
    typedef std::vector< T_EnumerationValues > T_EnumerationParameters;
    //@}

private:
    //! @name Member data
    //@{
    const ReportFactory& factory_;
    unsigned long id_;
    QString message_;
    T_EnumerationParameters enumerations_;
    //@}
};

#endif // __ReportTemplate_h_
