// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostsPanel_h_
#define __GhostsPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/SymbolVisitor_ABC.h"
#include "clients_kernel/GhostPrototype.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class ModelLoaded;
    class SymbolFactory;
    class SymbolRule;
}

namespace gui
{
    class ColorStrategy_ABC;
    class NatureEditionWidget;
    class PanelStack_ABC;
    class SymbolIcons;

    class UnitPreviewIcon;
}

class GhostModel;

// =============================================================================
/** @class  GhostsPanel
    @brief  GhostsPanel
*/
// Created: ABR 2011-10-12
// =============================================================================
class GhostsPanel : public gui::InfoPanel_ABC
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                  , public kernel::SymbolVisitor_ABC
                  , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GhostsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers,
                          kernel::SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy );
    virtual ~GhostsPanel();
    //@}

    //! @name Operations
    //@{
    void Load();
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnCategoryChange();
    void UpdateSymbol();
    void IconDragged();
    void OnSelectionChanged( const kernel::Entity_ABC* );
    void UpdateWarning();
    //@}

private:
    //! @name Types
    //@{
    enum E_Status { eLoading = 0, eSwapping = 1, eVisitingWeapon = 2 };

    typedef std::map< QString, std::string >    T_TraductionMap;
    typedef T_TraductionMap::const_iterator   CIT_TraductionMap;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    void AddChoice( const std::string& name, T_TraductionMap& traductionMap );
    void ResetCombo( QComboBox& combo, T_TraductionMap& traductionMap );
    void FillCombo( QComboBox& combo, T_TraductionMap& traductionMap );
    void Select( QComboBox& combo, const QString& value );
    Q3DragObject* dragObject();
    //@}

    //! @name SymbolVisitor implementation
    //@{
    virtual void StartCategory( const std::string& title );
    virtual void EndCategory();
    virtual void AddChoice( kernel::SymbolRule* rule, const std::string& name, const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&            controllers_;
    kernel::SymbolFactory&          symbolsFactory_;
    E_Status                        status_;
    kernel::GhostPrototype          protoType_;
    // Name and type
    QString                         nameBase_;
    QLineEdit*                      nameLineEdit_;
    QString                         typeBase_;
    QLineEdit*                      typeLineEdit_;
    // Level
    QString                         levelBase_;
    QComboBox*                      levelComboBox_;
    QStringList                     levelNames_;
    // Category and weapon
    QString                         categoryCurrentName_;
    const kernel::SymbolRule*       categoryRule_;
    QComboBox*                      categoryComboBox_;
    T_TraductionMap                 categoryNames_;
    QComboBox*                      weaponComboBox_;
    T_TraductionMap                 weaponNames_;
    // Icon
    gui::UnitPreviewIcon*           icon_;
    // Warning
    QLabel*                         warningLabel_;
    //@}
};

#endif // __GhostsPanel_h_
