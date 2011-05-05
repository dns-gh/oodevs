// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __disptacher_plugins_edxl_ReportBuilder_ABC_h_
#define __disptacher_plugins_edxl_ReportBuilder_ABC_h_

namespace sword
{
    class ObjectAttributeMedicalTreatment;
}

namespace edxl
{
    class Publisher_ABC;

// =============================================================================
/** @class  ReportBuilder_ABC
    @brief  ReportBuilder_ABC
*/
// Created: JCR 2010-06-10
// =============================================================================
class ReportBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportBuilder_ABC() {}
    virtual ~ReportBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void CreateReport( unsigned int id, const std::string& name, const sword::ObjectAttributeMedicalTreatment& medical ) = 0;
    virtual void Publish( Publisher_ABC& publisher ) = 0;
    //@}
};

} // end namespace edxl

#endif // __disptacher_plugins_edxl_ReportBuilder_ABC_h_
