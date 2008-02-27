// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConfigurationManipulator_h_
#define __ConfigurationManipulator_h_

#include "XmlNode.h"

namespace xml
{
    class xistream;
    class xostream;
    class cdata;
}

namespace frontend
{
    class XmlNode;

// =============================================================================
/** @class  ConfigurationManipulator
    @brief  ConfigurationManipulator
*/
// Created: SBO 2008-02-25
// =============================================================================
class ConfigurationManipulator
{

public:
    //! @name Constructors/Destructor
    //@{
             ConfigurationManipulator( const std::string& filename, const std::string& rootNode );
    virtual ~ConfigurationManipulator();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SetValue( const std::string& path, const T& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConfigurationManipulator( const ConfigurationManipulator& );            //!< Copy constructor
    ConfigurationManipulator& operator=( const ConfigurationManipulator& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< XmlNode > document_;
    std::auto_ptr< xml::xostream > output_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator::SetValue
// Created: SBO 2008-02-25
// -----------------------------------------------------------------------------
template< typename T >
void ConfigurationManipulator::SetValue( const std::string& path, const T& value )
{
    document_->SetValue( path, value );
}

}

#endif // __ConfigurationManipulator_h_
