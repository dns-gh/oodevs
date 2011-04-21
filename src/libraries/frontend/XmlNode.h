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
#include <boost/lexical_cast.hpp>

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
    explicit XmlNode( const std::string& filename );
    virtual ~XmlNode();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    template< typename T >
    void     SetValue( const std::string& path, const T& value );
    bool     HasNode( const std::string& path );
    XmlNode* GetChildNode( const std::string& path, bool parent = false );
    bool     RemoveNode( const std::string& path );
    template< typename T >
    T        GetValue( const std::string& path ) const ;
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
    bool GetStringValue( const std::string& path, std::string& value ) const;
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

// -----------------------------------------------------------------------------
// Name: XmlNode::GetValue
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
template< typename T >
T XmlNode::GetValue( const std::string& path ) const
{
    std::string str;
    if( GetStringValue( path, str ) )
        return boost::lexical_cast< T >(str);
    else
        throw std::exception( "XmlNode impossible to find path" );
}


template<>
bool XmlNode::GetValue<bool>( const std::string& path ) const;


}

#endif // __XmlNode_h_
