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

#include "tools/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"

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
class ScoreList : public Q3VBox
                , public tools::Observer_ABC
                , public tools::ElementObserver_ABC< Score_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::ParametersLayer& layer,
                        ScoresModel& model, const StaticModel& staticModel, const tools::ExerciseConfig& config,
                        const kernel::GlTools_ABC& tools );
    virtual ~ScoreList();
    //@}

    //! @name Operations
    //@{
    void Draw( kernel::Viewport_ABC& viewport );
    void Load();
    //@}

signals:
    //! @name Signals
    //@{
    void ScoreDeleted( const Score_ABC& score );
    void Show();
    void Hide();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEdit();
    void OnDelete();
    void OnGenerate();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreList( const ScoreList& );            //!< Copy constructor
    ScoreList& operator=( const ScoreList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Score_ABC& element );
    virtual void NotifyUpdated( const Score_ABC& element );
    virtual void NotifyDeleted( const Score_ABC& element );
    void Display( const Score_ABC& score, gui::ValuedListItem* item );
    Score_ABC* FindSelected() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    ScoresModel& model_;
    gui::ListDisplayer< ScoreList >* scores_;
    ScoreEditor* editor_;
    const tools::ExerciseConfig& config_;
    Q3HBox* generatorBox_;
    //@}
};

#endif // __ScoreList_h_
