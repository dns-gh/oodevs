// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DockManager_h_
#define __DockManager_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class AutomatsLayer;
    class FormationLayer;
    class EntitySymbols;
    class ItemFactory_ABC;
    template< typename T >
    class LogisticList;
    class SymbolIcons;
    class ColorStrategy_ABC;
    class ParametersLayer;
    class WeatherLayer;
    class GlProxy;
}

namespace tools
{
    class ExerciseConfig;
}

class ModelBuilder;
class Model;
class StaticModel;
class LogisticListView;
class ObjectCreationPanel;
class InhabitantCreationPanel;
class CreationPanels;
class LivingAreaPanel;

// =============================================================================
/** @class  DockManager
    @brief  Dock manager
*/
// Created: LGY 2012-01-04
// =============================================================================
class DockManager : private boost::noncopyable
                  , public tools::Observer_ABC
                  , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DockManager( QMainWindow* parent, kernel::Controllers& controllers, gui::AutomatsLayer& automats,
                          gui::FormationLayer& formation, gui::EntitySymbols& icons, ModelBuilder& modelBuilder,
                          gui::ItemFactory_ABC& factory, Model& model, StaticModel& staticModel,
                          const tools::ExerciseConfig& config, gui::SymbolIcons& symbols,
                          gui::ColorStrategy_ABC& colorStrategy, gui::ParametersLayer& paramLayer, gui::WeatherLayer& weatherLayer,
                          gui::GlProxy& glProxy );
    virtual ~DockManager();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load();
    QByteArray SaveGeometry() const;
    QByteArray SaveState() const;
    ObjectCreationPanel& GetObjectCreationPanel() const;
    InhabitantCreationPanel& GetInhabitantCreationPanel() const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Helpers
    //@{
    void ToggleWidget( QList< QWidget* >& container, QList< QWidget* >& current );
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* parent_;
    kernel::Controllers& controllers_;
    gui::LogisticList< LogisticListView >* logisticListView_;
    QDockWidget* pExtensionsPanel_;
    CreationPanels* pCreationPanel_;
    LivingAreaPanel* pLivingAreaPanel_;
    QList< QWidget* > dockWidgets_;
    QList< QWidget* > widgets_;
    QList< QWidget* > toolbars_;
    QByteArray geometry_;
    QByteArray state_;
    bool editionModeEnabled_;
    //@}
};
#endif // __DockManager_h_
