// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmlNode_h_
#define __XmlNode_h_

#include <map>
#include <sstream>

namespace xml
{
    class xistream;
    class xostream;
}

namespace frontend
{

// =============================================================================
/** @class  XmlNode
    @brief  XmlNode
*/
// Created: SBO 2008-02-27
// =============================================================================
class XmlNode
{

public:
    //! @name Constructors/Destructor
    //@{
             XmlNode( const std::string& filename, const std::string& root );
    virtual ~XmlNode();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    template< typename T >
    void SetValue( const std::string& path, const T& value );
    //@}

private:
    //! @name Constructors
    //@{
             XmlNode();
    explicit XmlNode( xml::xistream& xis );
    //@}

    //! @name Copy/Assignment
    //@{
    XmlNode( const XmlNode& );            //!< Copy constructor
    XmlNode& operator=( const XmlNode& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, XmlNode* > T_Nodes;
    typedef std::map< std::string, std::string > T_Attributes;
    //@}

    //! @name Helpers
    //@{
    void ReadAttribute( const std::string& name, xml::xistream& xis );
    void ReadChild( const std::string& name, xml::xistream& xis );
    void SetStringValue( const std::string& path, const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    T_Attributes attributes_;
    T_Nodes children_;
    std::string text_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: XmlNode::SetValue
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
template< typename T >
void XmlNode::SetValue( const std::string& path, const T& value )
{
    std::stringstream ss;
    ss << std::boolalpha << value;
    SetStringValue( path, ss.str() );
}

}

#endif // __XmlNode_h_
