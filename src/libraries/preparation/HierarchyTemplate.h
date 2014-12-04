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

#include <boost/noncopyable.hpp>

namespace gui
{
    class ColorController_ABC;
}

namespace kernel
{
    class Entity_ABC;
    class AgentTypes;
}

class AgentFactory_ABC;
class AgentsModel;
class FormationFactory_ABC;
class FormationModel;
class GhostModel;
class TemplateElement;

// =============================================================================
/** @class  HierarchyTemplate
    @brief  Hierarchy template
*/
// Created: AGE 2007-05-29
// =============================================================================
class HierarchyTemplate : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HierarchyTemplate( AgentsModel& agents,
                                FormationModel& formations,
                                GhostModel& ghosts,
                                const kernel::Entity_ABC& base,
                                bool root,
                                gui::ColorController_ABC& colorController );
             HierarchyTemplate( AgentsModel& agents,
                                FormationModel& formations,
                                GhostModel& ghosts,
                                const kernel::AgentTypes& types,
                                xml::xistream& xis,
                                gui::ColorController_ABC& colorController );
    virtual ~HierarchyTemplate();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void Instanciate( kernel::Entity_ABC& superior,
                      const geometry::Point2f& center ) const;
    bool IsCompatible( const kernel::Entity_ABC& superior ) const;
    QString GetName() const;
    void Rename( const QString& name );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< HierarchyTemplate* > T_Templates;
    //@}

    //! @name Helpers
    //@{
    void SetBasePosition( geometry::Point2f center );
    //@}

private:
    //! @name Member data
    //@{
    T_Templates subTemplates_;
    std::unique_ptr< TemplateElement > element_;
    geometry::Point2f referencePosition_;
    gui::ColorController_ABC& colorController_;
    //@}
};

#endif // __HierarchyTemplate_h_
