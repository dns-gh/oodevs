// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterXsl_h_
#define __FilterXsl_h_

#include "Filter.h"

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class QLineEdit;

// =============================================================================
/** @class  FilterXsl
    @brief  FilterXsl
*/
// Created: ABR 2011-06-17
// =============================================================================
class FilterXsl : public Filter
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterXsl( xml::xistream& xis, const tools::ExerciseConfig& config );
    virtual ~FilterXsl();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual const std::string GetName() const;
    virtual QWidget* CreateParametersWidget( QWidget* parent );
    virtual bool IsValid() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowse();
    void OnTextChanged();
    //@}

private:
    //! @name Member data
    //@{
    const std::string xsl_;
    const std::string xslFile_;
    const std::string inputFile_;
    const std::string exerciseFile_;
    const std::string outputExtension_;
    std::string       outputFile_;
    QLineEdit*        output_;
    //@}
};

#endif // __FilterXsl_h_
