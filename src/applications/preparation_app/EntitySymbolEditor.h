// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __EntitySymbolEditor_h_
#define __EntitySymbolEditor_h_

#include "clients_kernel/SymbolVisitor_ABC.h"
#include "ENT/ENT_Enums.h"

namespace kernel
{
    class Agent_ABC;
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
    template< typename T > class RichWidget;
}

// =============================================================================
/** @class  EntitySymbolEditor
    @brief  EntitySymbolEditor
*/
// Created: ABR 2012-08-08
// =============================================================================
class EntitySymbolEditor : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EntitySymbolEditor( QGridLayout* layout, int row, kernel::Controllers& controllers, kernel::SymbolFactory& symbolsFactory,
                                 gui::SymbolIcons& icons, gui::ColorStrategy_ABC& colorStrategy, const QString& iconPreviewToolTips,
                                 int stretch );
    virtual ~EntitySymbolEditor();
    //@}

    //! @name Operations
    //@{
    void Fill( const kernel::Entity_ABC& entity, const QString& nature, const QString& level );
    void Reset();
    //@}

    //! @name Getters
    //@{
    const std::string& GetSymbol() const;
    const std::string& GetLevel() const;
    const std::string& GetNature() const;
    E_GhostType GetGhostType() const;
    bool IsLevelValid() const;
    void SetDeep( int deep );
    //@}

private:
    //! @name Helpers
    //@{
    void FillLevel();
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
    kernel::SymbolFactory& symbolsFactory_;
    // Level
    E_GhostType ghostType_;
    std::string level_;
    QString levelBase_;
    gui::RichWidget< QComboBox >* levelComboBox_;
    QStringList levelNames_;
    // Nature
    gui::NatureEditionWidget* natureWidget_;
    std::string nature_;
    // Icon
    gui::UnitPreviewIcon* icon_;
    //@}
};

#endif // __EntitySymbolEditor_h_
