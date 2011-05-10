// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_DiffusionListDialog_h_
#define gui_DiffusionListDialog_h_

#include <boost/noncopyable.hpp>
#include "clients_kernel/SafePointer.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class Entity_ABC;
    class ExtensionTypes;
    class Profile_ABC;
}

namespace gui
{
    class DiffusionListHierarchy;
    class EntitySymbols;
    class ItemFactory_ABC;

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
             DiffusionListDialog( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver< kernel::Agent_ABC >& agents, const kernel::ExtensionTypes& extensions,
                                  ItemFactory_ABC& factory, const EntitySymbols& icons, const kernel::Profile_ABC& profile, const char* name = 0 );
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
    kernel::Controllers&                        controllers_;
    const kernel::ExtensionTypes&               extensions_;
    const tools::Resolver< kernel::Agent_ABC >& agents_;
    kernel::SafePointer< kernel::Entity_ABC >   selected_;
    std::string                                 extensionName_;
    DiffusionListHierarchy*                     list_;
    QLineEdit*                                  text_;
    QLabel*                                     transmitterName_;
    QButtonGroup*                               filtersButtons_;
    QLineEdit*                                  filterLine_;
    //@}
};

}

#endif // gui_DiffusionListDialog_h_
