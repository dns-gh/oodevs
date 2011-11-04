// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostSymbolEditor_h_
#define __GhostSymbolEditor_h_

#include "clients_kernel/SymbolVisitor_ABC.h"
#include "ENT/ENT_Enums_Gen.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Ghost_ABC;
    class Entity_ABC;
    class SymbolFactory;
    class SymbolRule;
}

namespace gui
{
    class ColorStrategy_ABC;
    class NatureEditionWidget;
    class SymbolIcons;
    class UnitPreviewIcon;
}

// =============================================================================
/** @class  GhostSymbolEditor
    @brief  GhostSymbolEditor
*/
// Created: ABR 2011-10-28
// =============================================================================
class GhostSymbolEditor : public QWidget
                        , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GhostSymbolEditor( QGridLayout* layout, int row, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory, gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy );
    virtual ~GhostSymbolEditor();
    //@}

    //! @name Operations
    //@{
    void Fill( const kernel::Ghost_ABC& ghost );
    void Reset();
    //@}

    //! @name Getters
    //@{
    const std::string& GetSymbol() const;
    const std::string& GetLevel() const;
    E_GhostType GetGhostType() const;
    bool IsLevelValid() const;
    //@}

private:
    //! @name Helpers
    //@{
    void FillLevelFromParent( const kernel::Entity_ABC* parent );
    //@}

signals:
    //! @name Signals
    //@{
    void StartDrag();
    void LevelChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateSymbol();
    void OnSelectionChanged( const kernel::Entity_ABC* parent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::SymbolFactory&          symbolsFactory_;
    // Level
    E_GhostType                     ghostType_;
    QString                         levelBase_;
    QComboBox*                      levelComboBox_;
    QStringList                     levelNames_;
    // Nature
    gui::NatureEditionWidget*       natureWidget_;
    // Icon
    gui::UnitPreviewIcon*           icon_;
    //@}
};

#endif // __GhostSymbolEditor_h_
