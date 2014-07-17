// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TemplateListView_h_
#define __TemplateListView_h_

#include "clients_gui/RichTreeView.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class AgentTypes;
}

class AgentsModel;
class ColorController;
class FormationModel;
class GhostModel;
class HierarchyTemplate;

// =============================================================================
/** @class  TemplateListView
    @brief  TemplateListView
*/
// Created: AGE 2007-05-30
// =============================================================================
class TemplateListView : public gui::RichTreeView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TemplateListView( const QString& objectName,
                               QWidget* parent,
                               kernel::Controllers& controllers,
                               AgentsModel& agents,
                               FormationModel& formations,
                               GhostModel& ghosts,
                               const kernel::AgentTypes& types,
                               ColorController& colorController );
    virtual ~TemplateListView();
    //@}

    //! @name Operations
    //@{
    void LoadTemplates( const tools::Path& filename );
    void CreateTemplate( const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRename( const QModelIndex& index, const QVariant& variant );
    void OnRename();
    void OnDelete();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< HierarchyTemplate* > T_Templates;
    //@}

    //! @name Helpers
    //@{
    void contextMenuEvent( QContextMenuEvent* event );
    virtual QStringList MimeTypes() const;
    void Clear();
    void CreateItem( HierarchyTemplate& t );
    void CreateItem( HierarchyTemplate& t, const QString& name );
    virtual void keyPressEvent( QKeyEvent* event );

    void SaveTemplates() const;
    //@}

private:
    //! @name Member data
    //@{
    AgentsModel& agents_;
    FormationModel& formations_;
    GhostModel& ghosts_;
    const kernel::AgentTypes& types_;
    ColorController& colorController_;
    T_Templates templates_;
    tools::Path file_;
    //@}
};

#endif // __TemplateListView_h_
