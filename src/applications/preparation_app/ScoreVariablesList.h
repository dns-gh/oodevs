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

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace gui
{
    class GlTools_ABC;
    class ParametersLayer;
    class MgrsParser;
    class Viewport_ABC;
    template< typename T > class RichWidget;
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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ScoreVariablesList( kernel::Controllers& controllers, const StaticModel& staticModel,
                                 gui::GlTools_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder );
    virtual ~ScoreVariablesList();
    //@}

    //! @name Operations
    //@{
    void Draw( gui::Viewport_ABC& viewport );
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
    void OnCloseDialogs();
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
    //! @name Member data
    //@{
    const gui::GlTools_ABC& tools_;
    ScoreVariableCreationWizard* wizard_;
    gui::RichWidget< QTreeWidget >* list_;
    std::unique_ptr< kernel::Location_ABC > location_;
    std::unique_ptr< gui::MgrsParser > parser_;
    //@}
};

#endif // __ScoreVariablesList_h_
