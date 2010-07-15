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

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlineedit.h>

class ScenarioEditPage;

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  CreateExerciceWidget
    @brief  CreateExerciceWidget
*/
// Created: JSR 2010-07-13
// =============================================================================
class CreateExerciceWidget : public QGroupBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             CreateExerciceWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config );
    virtual ~CreateExerciceWidget();
    //@}

    //! @name Operations
    //@{
    void Update();
    void CreateExercise();
    bool EnableEditButton();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreateExerciceWidget( const CreateExerciceWidget& );            //!< Copy constructor
    CreateExerciceWidget& operator=( const CreateExerciceWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    ScenarioEditPage& page_;
    QLineEdit* editName_;
    QComboBox* editTerrainList_;
    QComboBox* editModelList_;
    //@}
};

#endif // __CreateExerciceWidget_h_
