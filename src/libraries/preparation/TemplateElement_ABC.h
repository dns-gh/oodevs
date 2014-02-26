// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TemplateElement_ABC_h_
#define __TemplateElement_ABC_h_

#include <boost/noncopyable.hpp>
#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>

namespace kernel
{
    class Entity_ABC;
}

class ColorController;

// =============================================================================
/** @class  TemplateElement_ABC
    @brief  TemplateElement_ABC
*/
// Created: AGE 2007-05-29
// =============================================================================
class TemplateElement_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TemplateElement_ABC();
    explicit TemplateElement_ABC( const kernel::Entity_ABC& entity );
    explicit TemplateElement_ABC( xml::xistream& xis );
    virtual ~TemplateElement_ABC();
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
    std::map< std::string, std::string > extensions_;
    //@}
};

#endif // __TemplateElement_ABC_h_
