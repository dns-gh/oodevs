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

#include <boost/noncopyable.hpp>
#include "XmlNode.h"

namespace tools
{
    class GeneralConfig;
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
class ConfigurationManipulator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ConfigurationManipulator( const tools::Path& filename );
             ConfigurationManipulator( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session );
    virtual ~ConfigurationManipulator();
    //@}

    //! @name Operations
    //@{
    const tools::Path& GetPath() const;
    template< typename T >
    void SetValue( const std::string& path, const T& value );
    bool HasNode( const std::string& path );
    bool RemoveNode( const std::string& path );
    template< typename T >
    T GetValue ( const std::string& path ) const;
    void Commit();
    //@}

    //! @name Helpers
    //@{
    static tools::Path GetSessionXml( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Path outputPath_;
    std::unique_ptr< XmlNode > document_;
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
