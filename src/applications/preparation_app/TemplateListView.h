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

namespace kernel { class Entity_ABC; class AgentTypes; }
namespace xml    { class xistream; }

class HierarchyTemplate;
class AgentsModel;
class FormationModel;

// =============================================================================
/** @class  TemplateListView
    @brief  TemplateListView
*/
// Created: AGE 2007-05-30
// =============================================================================
class TemplateListView : public QListView
{

public:
    //! @name Constructors/Destructor
    //@{
             TemplateListView( QWidget* parent, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types );
    virtual ~TemplateListView();
    //@}

    //! @name Operations
    //@{
    void LoadTemplates( const std::string& filename );
    void SaveTemplates( const std::string& filename ) const;
    void CreateTemplate( const kernel::Entity_ABC& entity );
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
    typedef T_Templates::const_iterator     CIT_Templates;
    //@}

    //! @name Helpers
    //@{
    virtual QDragObject* dragObject();
    void Clear();
    void ReadTemplate( xml::xistream& input );
    //@}

private:
    //! @name Member data
    //@{
    AgentsModel& agents_;
    FormationModel& formations_;
    const kernel::AgentTypes& types_;

    T_Templates templates_;
    //@}
};

#endif // __TemplateListView_h_
