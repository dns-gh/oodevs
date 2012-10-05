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

namespace kernel { class Entity_ABC; class AgentTypes; }
namespace xml    { class xistream; }

class AgentsModel;
class ColorController;
class FormationModel;
class HierarchyTemplate;

namespace gui
{
    class ValuedListItem;
}

// =============================================================================
/** @class  TemplateListView
    @brief  TemplateListView
*/
// Created: AGE 2007-05-30
// =============================================================================
class TemplateListView : public Q3ListView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TemplateListView( QWidget* parent, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types, ColorController& colorController );
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
    void OnRename( Q3ListViewItem*, int, const QString& );
    void OnRename();
    void OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TemplateListView( const TemplateListView& );            //!< Copy constructor
    TemplateListView& operator=( const TemplateListView& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< HierarchyTemplate* > T_Templates;
    typedef T_Templates::iterator            IT_Templates;
    typedef T_Templates::const_iterator     CIT_Templates;
    //@}

    //! @name Helpers
    //@{
    virtual Q3DragObject* dragObject();
    void Clear();
    void ReadTemplate( xml::xistream& input );
    gui::ValuedListItem* CreateItem( HierarchyTemplate& t );
    virtual void keyPressEvent( QKeyEvent* event );
    void Rename( gui::ValuedListItem& item, const QString& newName );
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
