// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ReportTemplate_h_
#define __ReportTemplate_h_


namespace xml
{
    class xistream;
}

namespace Common
{
    class MsgMissionParameters;
}

namespace plugins
{
namespace crossbow
{
    class ReportFactory;

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
    std::string RenderMessage( const Common::MsgMissionParameters& message ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ReportTemplate( const ReportTemplate& );            //!< Copy constructor
    ReportTemplate& operator=( const ReportTemplate& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    void ReadEnumeration( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string >         T_EnumerationValues;
    typedef std::vector< T_EnumerationValues > T_EnumerationParameters;
    //@}

private:
    //! @name Member data
    //@{
    const ReportFactory& factory_;
    unsigned long id_;
    std::string message_;
    T_EnumerationParameters enumerations_;
    //@}
};

}
}

#endif // __ReportTemplate_h_
