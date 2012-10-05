// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
             TemplateListView( QWidget* parent, kernel::Controllers& controllers, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types, ColorController& colorController );
    virtual ~TemplateListView();
    //@}

    //! @name Operations
    //@{
    void LoadTemplates( const std::string& filename );
    void SaveTemplates( const std::string& filename ) const;
    void CreateTemplate( const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRename( const QModelIndex& index, const QVariant& variant );
    void OnRename();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< HierarchyTemplate* > T_Templates;
    typedef T_Templates::iterator            IT_Templates;
    typedef T_Templates::const_iterator     CIT_Templates;
    //@}

    //! @name Helpers
    //@{
    void contextMenuEvent( QContextMenuEvent* event );
    virtual QStringList MimeTypes() const;
    void Clear();
    void ReadTemplate( xml::xistream& input );
    void CreateItem( HierarchyTemplate& t );
    void CreateItem( HierarchyTemplate& t, const QString& name );
    virtual void keyPressEvent( QKeyEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    AgentsModel& agents_;
    FormationModel& formations_;
    const kernel::AgentTypes& types_;
    ColorController& colorController_;
    T_Templates templates_;
    //@}
};

#endif // __TemplateListView_h_
