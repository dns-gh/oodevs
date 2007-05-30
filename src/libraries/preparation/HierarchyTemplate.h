// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __HierarchyTemplate_h_
#define __HierarchyTemplate_h_

namespace kernel { class Entity_ABC; class AgentTypes; }
namespace xml    { class xostream; class xistream; }

class TemplateElement_ABC;
class AgentFactory_ABC;
class FormationFactory_ABC;
class AgentsModel;
class FormationModel;

// =============================================================================
/** @class  HierarchyTemplate
    @brief  Hierarchy template
*/
// Created: AGE 2007-05-29
// =============================================================================
class HierarchyTemplate
{

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyTemplate( AgentsModel& agents, FormationModel& formations, const kernel::Entity_ABC& base, bool root = true );
             HierarchyTemplate( AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types, xml::xistream& input );
    virtual ~HierarchyTemplate();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& output ) const;
    void Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& center ) const;
    bool IsCompatible( const kernel::Entity_ABC& superior ) const;
    QString GetName() const;
    void Rename( const QString& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HierarchyTemplate( const HierarchyTemplate& );            //!< Copy constructor
    HierarchyTemplate& operator=( const HierarchyTemplate& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< HierarchyTemplate* > T_Templates;
    typedef T_Templates::const_iterator     CIT_Templates;
    //@}

    //! @name Helpers
    //@{
    TemplateElement_ABC* CreateElement( AgentsModel& agents, FormationModel& formations, const kernel::Entity_ABC& entity );
    TemplateElement_ABC* CreateElement( AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types, xml::xistream& input );
    void ReadSubTemplate( xml::xistream& input, AgentsModel& agents, FormationModel& formations, const kernel::AgentTypes& types );
    void SetBasePosition( geometry::Point2f center );
    //@}

private:
    //! @name Member data
    //@{
    T_Templates subTemplates_;
    std::auto_ptr< TemplateElement_ABC > element_;
    geometry::Point2f referencePosition_;
    QString name_;
    //@}
};

#endif // __HierarchyTemplate_h_
