// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertyFactory_h_
#define __PropertyFactory_h_

#include "RenderingContext_ABC.h"

namespace xml
{
    class xistream; 
}

namespace svg
{
    class PropertyFactory_ABC;
    class PropertyGroup;

// =============================================================================
/** @class  PropertyFactory
    @brief  PropertyFactory
*/
// Created: AGE 2006-08-23
// =============================================================================
class PropertyFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             PropertyFactory();
    virtual ~PropertyFactory();
    //@}

    //! @name Operations
    //@{
    void ChangeFactory   ( RenderingContext_ABC::E_Properties type, std::unique_ptr< PropertyFactory_ABC > factory );
    void ChangeFactory   ( RenderingContext_ABC::E_Properties type, const PropertyFactory_ABC& factory );
    void CreateProperties( PropertyGroup& group, xml::xistream& input ) const;
    void CreateProperties( PropertyGroup& group, const std::string& css ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PropertyFactory( const PropertyFactory& );            //!< Copy constructor
    PropertyFactory& operator=( const PropertyFactory& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    struct PropertyType
    {
        std::string                        name_;
        RenderingContext_ABC::E_Properties type_;
        const PropertyFactory_ABC*         factory_;
    };
    typedef std::vector< PropertyType >       T_PropertyTypes;
    typedef T_PropertyTypes::iterator        IT_PropertyTypes;
    typedef T_PropertyTypes::const_iterator CIT_PropertyTypes;
    //@}

    //! @name Helpesr
    //@{
    void AddType( const std::string& name, RenderingContext_ABC::E_Properties type, const PropertyFactory_ABC* factory );
    void CreateFromCss( PropertyGroup& group, xml::xistream& input ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_PropertyTypes propertyTypes_;
    //@}
};

}

#endif // __PropertyFactory_h_
