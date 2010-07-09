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
#include <qstring.h>
#pragma warning( pop )
#include <vector>

namespace xml
{
    class xistream;
}

class ReportFactory;
namespace Common
{
    class MsgMissionParameters;
}

// =============================================================================
/** @class  ReportTemplate
    @brief  ReportTemplate
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
    QString RenderMessage( const Common::MsgMissionParameters& asn ) const;
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
