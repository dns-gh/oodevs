// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseProperties_h_
#define __ExerciseProperties_h_

#include <qvbox.h>

namespace frontend
{
    class Exercise_ABC;
}

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ExerciseProperties
    @brief  ExerciseProperties
*/
// Created: SBO 2010-11-12
// =============================================================================
class ExerciseProperties : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseProperties( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, bool briefing, bool models, bool editable );
    virtual ~ExerciseProperties();
    //@}

    //! @name Operations
    //@{
    void Update();
    void Select( const frontend::Exercise_ABC* exercise );
    void Commit( const frontend::Exercise_ABC& exercise );
    //@}

private slots:
    //! @name Slots
    //@{
    void ModelChanged();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseProperties( const ExerciseProperties& );            //!< Copy constructor
    ExerciseProperties& operator=( const ExerciseProperties& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadBriefingText( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    const QString language_;
    QLabel* briefingImage_;
    QTextEdit* briefingText_;
    QComboBox* terrainList_;
    QComboBox* modelList_;
    bool dataChanged_;
    //@}
};

#endif // __ExerciseProperties_h_
