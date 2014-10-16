// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreList_h_
#define __ScoreList_h_

#include <tools/ElementObserver_ABC.h>

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace gui
{
    class ParametersLayer;
}

namespace indicators
{
    class GaugeTypes;
    class Primitives;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;
    template< typename T > class RichWidget;
}

namespace tools
{
    class ExerciseConfig;
}

class Score_ABC;
class ScoreEditor;
class ScoresModel;
class StaticModel;

// =============================================================================
/** @class  ScoreList
    @brief  ScoreList
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoreList : public QWidget
                , public tools::Observer_ABC
                , public tools::ElementObserver_ABC< Score_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreList( kernel::Controllers& controllers, ScoresModel& model, const tools::ExerciseConfig& config,
                        const StaticModel& staticModel, gui::GLView_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder, const QString& objectName );
    virtual ~ScoreList();
    //@}

    //! @name Operations
    //@{
    void Draw( gui::Viewport_ABC& viewport );
    void Load();
    //@}

signals:
    //! @name Signals
    //@{
    void ScoreDeleted( const Score_ABC& score );
    void Show();
    void Hide();
    void CloseDialogs();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEdit();
    void OnDelete();
    void OnGenerate();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Score_ABC& element );
    virtual void NotifyUpdated( const Score_ABC& element );
    virtual void NotifyDeleted( const Score_ABC& element );
    Score_ABC* FindSelected() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&             controllers_;
    ScoresModel&                     model_;
    gui::RichWidget< QTreeWidget >*                  scores_;
    ScoreEditor*                     editor_;
    const tools::ExerciseConfig&     config_;
    Q3HBox*                          generatorBox_;
    //@}
};

#endif // __ScoreList_h_
