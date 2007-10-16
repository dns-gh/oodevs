// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligencesPanel_h_
#define __IntelligencesPanel_h_

#include "InfoPanel_ABC.h"
#include "ValuedComboBox.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/TeamSelectionObserver.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class AgentTypes;
    class FormationLevels;
    class HierarchyLevel_ABC;
    class SymbolFactory;
    class IntelligencePrototype;
    class Team_ABC;
    class Entity_ABC;
}

namespace gui
{
    class PanelStack_ABC;
    class ItemFactory_ABC;
    class SymbolIcons;

// =============================================================================
/** @class  IntelligencesPanel
    @brief  IntelligencesPanel
*/
// Created: SBO 2007-10-12
// =============================================================================
class IntelligencesPanel : public InfoPanel_ABC
                         , public kernel::Observer_ABC
                         , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                         , public kernel::TeamSelectionObserver
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             IntelligencesPanel( QWidget* parent, PanelStack_ABC& panel, kernel::Controllers& controllers, const kernel::FormationLevels& levels, SymbolIcons& icons );
    virtual ~IntelligencesPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateSymbol();
    void OnNatureChanged( const QString& text );
    void OnNatureDragged();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligencesPanel( const IntelligencesPanel& );            //!< Copy constructor
    IntelligencesPanel& operator=( const IntelligencesPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void Select( const kernel::Team_ABC* element );
    virtual void mouseMoveEvent( QMouseEvent* event );
    void DoDrag();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    SymbolIcons& icons_;
    std::auto_ptr< kernel::SymbolFactory > symbolFactory_;
    ValuedComboBox< const kernel::HierarchyLevel_ABC* >* levelCombo_;
    QComboBox* karmaCombo_;
    QLabel* icon_;
    QLabel* teamLabel_;
    QString nature_;
    std::string symbol_;
    kernel::SafePointer< kernel::Team_ABC > selectedTeam_;
    std::auto_ptr< kernel::IntelligencePrototype > intelligence_;
    //@}
};

}

#endif // __IntelligencesPanel_h_
