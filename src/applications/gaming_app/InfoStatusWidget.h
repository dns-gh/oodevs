// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoStatusWidget_h_
#define __InfoStatusWidget_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class HumanFactors_ABC;
    class Population_ABC;
    class Profile_ABC;
}

namespace gui
{
    class EntitySymbols;
}

class Q3ProgressBar;
class Attributes;
class HumanFactors;
class Reinforcements;

// =============================================================================
/** @class  InfoStatusWidget
    @brief  InfoStatusWidget
*/
// Created: SBO 2007-02-02
// =============================================================================
class InfoStatusWidget : public Q3VBox
                       , public tools::Observer_ABC
                       , public tools::SelectionObserver< kernel::Entity_ABC >
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                       , public tools::ElementObserver_ABC< Attributes >
                       , public tools::ElementObserver_ABC< kernel::HumanFactors_ABC >
                       , public tools::ElementObserver_ABC< kernel::Population_ABC >
                       , public tools::ElementObserver_ABC< Reinforcements >
                       , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InfoStatusWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const gui::EntitySymbols& icons );
    virtual ~InfoStatusWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void GotoParent();
    void SetIcon();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const Attributes& element );
    virtual void NotifyUpdated( const kernel::HumanFactors_ABC& element );
    virtual void NotifyUpdated( const kernel::Population_ABC& element );
    virtual void NotifyUpdated( const Reinforcements& element );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    void SetDefault();
    void SetLifeBar( const Attributes& attributes );
    void SetName   ( const Attributes* attributes );
    void SetHumanFactors( const HumanFactors& humans );
    void SetExperience( const HumanFactors& humans );
    void SetMorale( const HumanFactors& humans );
    void SetTiredness( const HumanFactors& humans );
    void SetReinforcements( const Reinforcements& reinforcements );

    void DrawCrowdChartPie( QPixmap& pixmap, const kernel::Population_ABC& population );
    QBrush GetCrowdBrush( const std::string& option, QColor defaultColor );

    virtual void resizeEvent( QResizeEvent* ev );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    const gui::EntitySymbols& icons_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;

    QPushButton* gotoParent_;
    QLabel* icon_;
    QLabel* name_;
    Q3ProgressBar* lifeBar_;

    QLabel* reinforced_;
    QLabel* reinforcing_;
    QLabel* experience_;
    QLabel* morale_;
    QLabel* tiredness_;

    QPixmap csword_;
    QImage background_;
    QImage boost_, warning_, error_;
    QImage experienced_, veteran_;
    //@}
};

#endif // __InfoStatusWidget_h_
