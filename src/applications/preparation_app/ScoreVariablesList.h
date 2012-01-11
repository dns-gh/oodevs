// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreVariablesList_h_
#define __ScoreVariablesList_h_

#include "clients_gui/ListDisplayer.h"
#include <boost/noncopyable.hpp>

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
    class UtmParser;
}

namespace indicators
{
    class Element_ABC;
    class Variables;
}

namespace kernel
{
    class Controllers;
    class Location_ABC;
}

class ScoreVariableCreationWizard;
class StaticModel;

// =============================================================================
/** @class  ScoreVariablesList
    @brief  ScoreVariablesList
*/
// Created: SBO 2009-04-20
// =============================================================================
class ScoreVariablesList : public Q3VBox
                         , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScoreVariablesList( QWidget* parent, gui::ItemFactory_ABC& factory, kernel::Controllers& controllers,
                                 const StaticModel& staticModel, const kernel::GlTools_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder );
    virtual ~ScoreVariablesList();
    //@}

    //! @name Operations
    //@{
    void Draw( kernel::Viewport_ABC& viewport );
    void StartEdit( const indicators::Variables& variables );
    indicators::Variables GetValue() const;
    //@}

signals:
    //! @name Signals
    //@{
    void Insert( const QString& text );
    void Updated();
    void StartEdit();
    void EndEdit();
    //@}

public slots:
    //! @name Slots
    //@{
    void AddVariable( const indicators::Element_ABC& variable );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAdd();
    void OnDelete();
    void OnPaste();
    void OnItemClick();
    //@}

private:
    //! @name Helpers
    //@{
    void Display( const indicators::Element_ABC& variable, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    const kernel::GlTools_ABC& tools_;
    ScoreVariableCreationWizard* wizard_;
    gui::ListDisplayer< ScoreVariablesList >* list_;
    std::auto_ptr< kernel::Location_ABC > location_;
    std::auto_ptr< gui::UtmParser > parser_;
    //@}
};

#endif // __ScoreVariablesList_h_
