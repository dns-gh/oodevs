// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreateExerciceWidget_h_
#define __CreateExerciceWidget_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/LanguageChangeObserver_ABC.h"

class ScenarioEditPage;

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

// =============================================================================
/** @class  CreateExerciceWidget
    @brief  CreateExerciceWidget
*/
// Created: JSR 2010-07-13
// =============================================================================
class CreateExerciceWidget : public gui::LanguageChangeObserver_ABC< Q3GroupBox >
                           , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             CreateExerciceWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader );
    virtual ~CreateExerciceWidget();
    //@}

    //! @name Operations
    //@{
    void Update();
    void CreateExercise();
    bool EnableEditButton();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnLanguageChanged();
    void UpdateExercises();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( Q3ListBoxItem* item );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC&    fileLoader_;
    ScenarioEditPage&           page_;
    QLineEdit*                  editName_;
    QComboBox*                  editTerrainList_;
    QComboBox*                  editModelList_;
    Q3GroupBox*                 saveAsGroupBox_;
    Q3ListBox*                  exerciseList_;
    Q3ListView*                 contentList_;

    QLabel*                     createLabel_;
    QLabel*                     copyLabel_;
    QLabel*                     copyContentLabel_;
    //@}
};

#endif // __CreateExerciceWidget_h_
