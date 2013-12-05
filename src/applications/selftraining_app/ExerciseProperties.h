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

#include <boost/noncopyable.hpp>
#include "clients_gui/WidgetLanguageObserver_ABC.h"
#include <tools/Path.h>

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
class ExerciseProperties : public gui::WidgetLanguageObserver_ABC< QWidget >
                         , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseProperties( QWidget* parent, QWidget* granParent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, bool briefing, bool models, bool editable );
    virtual ~ExerciseProperties();
    //@}

    //! @name Operations
    //@{
    void Update();
    void Select( const frontend::Exercise_ABC* exercise );
    bool Commit( const frontend::Exercise_ABC& exercise );
    bool IsValid() const;
    //@}

signals:
    //! @name Signals
    //@{
    void ExercisePropertiesChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void ModelChanged();
    //@}

private:
    //! @name Helpers
    //@{
    class AutoResizedCombo;
    virtual void OnLanguageChanged();
    void ReadBriefingText( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    QWidget* parent_;
    QLabel* parametersLabel_;
    QLabel* briefingImage_;
    QTextEdit* briefingText_;
    AutoResizedCombo* terrainList_;
    AutoResizedCombo* modelList_;
    bool dataChanged_;
    tools::Path currentData_;
    tools::Path currentPhysical_;
    tools::Path currentTerrain_;
    //@}
};

#endif // __ExerciseProperties_h_
