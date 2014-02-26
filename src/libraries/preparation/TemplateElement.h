// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TemplateElement_h_
#define __TemplateElement_h_

#include <boost/noncopyable.hpp>
#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>

namespace kernel
{
    class Entity_ABC;
}

class ColorController;

// =============================================================================
/** @class  TemplateElement
    @brief  TemplateElement
*/
// Created: AGE 2007-05-29
// =============================================================================
class TemplateElement : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TemplateElement();
    explicit TemplateElement( const kernel::Entity_ABC& entity );
    explicit TemplateElement( xml::xistream& xis );
    virtual ~TemplateElement();
    //@}

    //! @name Abstract operations
    //@{
    virtual bool IsCompatible( const kernel::Entity_ABC& superior ) const = 0;
    virtual kernel::Entity_ABC* Instanciate( kernel::Entity_ABC& superior,
                                             const geometry::Point2f& center,
                                             ColorController& colorController ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& output ) const;
    void SetColor( kernel::Entity_ABC& entity,
                   ColorController& colorController );
    void SetExtensions( kernel::Entity_ABC& entity );
    QString GetName() const;
    void Rename( const QString& name );
    //@}

protected:
    //! @name Member data
    //@{
    QString name_;
    const boost::optional< boost::tuple< unsigned int, unsigned int, unsigned int > > color_;
    const std::map< std::string, std::string > extensions_;
    //@}
};

#endif // __TemplateElement_h_
