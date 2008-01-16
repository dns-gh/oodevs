// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligencesPanel_h_
#define __IntelligencesPanel_h_

#include "InfoPanel_ABC.h"
#include "ValuedComboBox.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
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
    class Entity_ABC;
    class Formation_ABC;
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
                         , public kernel::SelectionObserver_ABC
                         , public kernel::SelectionObserver_Base< kernel::Formation_ABC >
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
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Formation_ABC& element );

    virtual void NotifyUpdated( const kernel::ModelLoaded& );
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
    QLabel* superiorLabel_;
    QString nature_;
    std::string symbol_;
    QCheckBox* mounted_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    std::auto_ptr< kernel::IntelligencePrototype > intelligence_;
    //@}
};

}

#endif // __IntelligencesPanel_h_
