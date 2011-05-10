// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DiffusionListDialog_h_
#define __DiffusionListDialog_h_

#include <boost/noncopyable.hpp>
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class ExtensionTypes;
}

namespace gui
{
    class EntitySymbols;
    class ItemFactory_ABC;
}

class AgentsModel;
class DiffusionListHierarchy;
class FormationModel;
class Model;

// =============================================================================
/** @class  DiffusionListDialog
    @brief  DiffusionListDialog
*/
// Created: ABR 2011-04-29
// =============================================================================
class DiffusionListDialog : public QDialog
                          , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DiffusionListDialog( QWidget* parent, kernel::Controllers& controllers, const AgentsModel& agents, const kernel::ExtensionTypes& extensions, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, const char* name = 0 );
    virtual ~DiffusionListDialog();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Fill( kernel::SafePointer< kernel::Entity_ABC > selected, const QString& diffusionList );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateDisplay();
    void AddLegend( const QString label, QWidget* parent, const QColor& color );
    //@}

signals:
    //! @name Signals
    //@{
    void Accepted( const QString& );
    void Rejected();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void reject();
    virtual void accept();
    void OnClear();
    void OnClearAll();
    void OnGenerate();
    void OnGenerateAll();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                      controllers_;
    const AgentsModel&                        agents_;
    const kernel::ExtensionTypes&             extensions_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::string                               extensionName_;
    DiffusionListHierarchy*                   list_;
    QLineEdit*                                text_;
    QLabel*                                   transmitterName_;
    QButtonGroup*                             filtersButtons_;
    QLineEdit*                                filterLine_;
    //@}
};

#endif // __DiffusionListDialog_h_
