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

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

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
    explicit ConfigurationManipulator( const std::string& filename );
             ConfigurationManipulator( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
    virtual ~ConfigurationManipulator();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void SetValue( const std::string& path, const T& value );
    bool HasNode( const std::string& path );
    bool RemoveNode( const std::string& path );
    template< typename T >
    T GetValue ( const std::string& path ) const;
    void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConfigurationManipulator( const ConfigurationManipulator& );            //!< Copy constructor
    ConfigurationManipulator& operator=( const ConfigurationManipulator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    static std::string GetSessionXml( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session );
    //@}

private:
    //! @name Member data
    //@{
    const std::string outputPath_;
    std::auto_ptr< XmlNode > document_;
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

// -----------------------------------------------------------------------------
// Name: ConfigurationManipulator::GetValue
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
template< typename T >
T ConfigurationManipulator::GetValue( const std::string& path ) const
{
    return document_->GetValue<T>(path);
}

}

#endif // __ConfigurationManipulator_h_
