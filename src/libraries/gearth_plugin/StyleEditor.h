// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_StyleEditor_h_
#define __gearth_StyleEditor_h_

#include <map>

namespace xml
{
    class xostream;
    class xistream;
}

namespace dispatcher
{
    class Config;
}

namespace gearth
{

// =============================================================================
/** @class  StyleEditor
    @brief  StyleEditor    
*/
// Created: JCR 2007-09-04
// =============================================================================
class StyleEditor
{
public:
    //! @name Constructors/Destructor
    //@{
             StyleEditor( const dispatcher::Config& config, const std::string& virtualPath );
    virtual ~StyleEditor();
    //@}

    //! @name 
    //@{
    void Load( xml::xostream& xos );
    //@}

    //! @name 
    //@{    
    std::string GetStyle( const std::string& type ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    StyleEditor( const StyleEditor& );            //!< Copy constructor
    StyleEditor& operator=( const StyleEditor& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    void ReadStyle( xml::xistream& xis );
    //@}

    //! @name Create New style
    //@{
    void WriteEntityStyle( xml::xostream& xos, const std::string& style, const std::string& ref );
    void WriteObjectStyle( xml::xostream& xos, const std::string& style, const std::string& color );
    void WriteLineStyle( xml::xostream& xos, const std::string& style, const std::string& color );
    //@}

private:
    //! @name 
    //@{
    typedef std::map<std::string, std::string>  T_StyleMap;
    typedef T_StyleMap::const_iterator          CIT_StyleMap;
    //@}
private:
    //! @name 
    //@{
    T_StyleMap  styles_;
    std::string path_small_;
    std::string path_large_;
    //@}
};

}

#endif // __gearth_StyleEditor_h_
