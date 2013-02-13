// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef OrbatConfigPanel_h
#define OrbatConfigPanel_h

#include "frontend/PluginConfig_ABC.h"

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    class Exercise_ABC;
}

// =============================================================================
/** @class  OrbatConfigPanel
    @brief  Orbat config panel
*/
// Created: LGY 2013-02-06
// =============================================================================
class OrbatConfigPanel : public frontend::PluginConfig_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             OrbatConfigPanel( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~OrbatConfigPanel();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual void Commit( const std::string& exercise, const std::string& session );
    virtual void OnLanguageChanged();
    //@}

public slots:
    //! @name Operations
    //@{
    void Select( const frontend::Exercise_ABC& exercise );
    void ClearSelection();
    //@}

private:
    //! @name TYpes
    //@{
    typedef std::map< unsigned int, QCheckBox* > T_Checkbox;
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    QGroupBox* sideBox_;
    QVBoxLayout* mainLayout_;
    QLabel* noSideLabel_;
    QCheckBox* noSideObjectsLabel_;
    T_Checkbox sideCheckBox_;
    std::string currentExercise_;
    //@}
};

#endif // OrbatConfigPanel_h
