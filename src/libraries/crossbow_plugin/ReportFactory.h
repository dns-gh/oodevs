// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportFactory_h_
#define __ReportFactory_h_

#include "protocol/protocol.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
    class Model_ABC;
}

namespace plugins
{
namespace crossbow
{
    class ReportTemplate;

// =============================================================================
/** @class  ReportFactory
    @brief  ReportFactory
*/
// Created: SBO 2006-12-07
// =============================================================================
class ReportFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportFactory( const dispatcher::Config& config, const dispatcher::Model_ABC& model );
    virtual ~ReportFactory();
    //@}

    //! @name Operations
    //@{
    std::string CreateMessage( const sword::Report& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportFactory( const ReportFactory& );            //!< Copy constructor
    ReportFactory& operator=( const ReportFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadReport( xml::xistream& xis );
    friend class ReportTemplate;
    std::string RenderParameter( const sword::MsgMissionParameter& value ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, const ReportTemplate* > T_Templates;
    typedef T_Templates::const_iterator                    CIT_Templates;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    T_Templates templates_;
    //@}
};

}
}

#endif // __ReportFactory_h_
