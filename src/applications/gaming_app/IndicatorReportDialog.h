// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorReportDialog_h_
#define __IndicatorReportDialog_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    class LinkInterpreter_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

class ScoreModel;

// =============================================================================
/** @class  IndicatorReportDialog
    @brief  IndicatorReportDialog
*/
// Created: SBO 2009-05-05
// =============================================================================
class IndicatorReportDialog : public QDialog
                            , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorReportDialog( QWidget* parent, const ScoreModel& model, const tools::ExerciseConfig& config, gui::LinkInterpreter_ABC& interpreter );
    virtual ~IndicatorReportDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowseTemplate();
    void OnBrowseOutput();
    void OnFileChanged();
    void OnAccept();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateReport() const;
    std::string ReplaceTags( const std::string& line ) const;
    void DisplayReport() const;
    //@}

private:
    //! @name Member data
    //@{
    const ScoreModel& model_;
    const tools::ExerciseConfig& config_;
    gui::LinkInterpreter_ABC& interpreter_;
    QLineEdit* templateFile_;
    QLineEdit* outputFile_;
    QCheckBox* displayReport_;
    QButton* ok_;
    //@}
};

#endif // __IndicatorReportDialog_h_
