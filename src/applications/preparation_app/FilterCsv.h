// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterCsv_h_
#define __FilterCsv_h_

#include "Filter_ABC.h"
#include "Progress_ABC.h"

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
    class RichLineEdit;
}

class Model;
class CsvExport;

// =============================================================================
/** @class  FilterCsv
    @brief  FilterCsv
*/
// Created: LGY 2011-10-17
// =============================================================================
class FilterCsv : public Filter_ABC
                , private Progress_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterCsv( QWidget* parent, const tools::ExerciseConfig& config, Model& model,
                        const kernel::CoordinateConverter_ABC& converter );
    virtual ~FilterCsv();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual const std::string GetName() const;
    virtual const std::string GetDescription() const;
    virtual const tools::Path GetExerciseName() const;
    virtual QWidget* CreateParametersWidget( const QString& objectName, QWidget* parent );
    virtual bool IsValid() const;
    virtual bool NeedToReloadExercise() const;
    virtual void Update( unsigned int value );
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
    QProgressDialog* progressDialog_;
    std::unique_ptr< CsvExport > pExport_;
    gui::RichLineEdit* output_;
    const tools::Path exerciseFile_;
    const tools::Path exerciseName_;
    //@}
};

#endif // __FilterCsv_h_
