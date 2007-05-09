// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ESRI_Config_h_
#define __ESRI_Config_h_

namespace tools
{
    class GeneralConfig;
}

namespace dispatcher 
{   
namespace esri 
{

// =============================================================================
/** @class  ESRI_Config
    @brief  ESRI_Config
*/
// Created: JCR 2007-05-02
// =============================================================================
class Config
{
public:
    //! @name Constructors/Destructor
    //@{
            Config( const tools::GeneralConfig& config );
            Config( const Config& );            //!< Copy constructor
    virtual ~Config();
    //@}

    //! @name 
    //@{    
    void UpdateGeodatabase( const std::string& geodatabse );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetTerrainFile() const;
    std::string        BuildTerrainChildFile( const std::string& file ) const;        
    const std::string& GetPhysicalFile() const;
    std::string        BuildPhysicalChildFile( const std::string& file ) const;
    const std::string& GetGeodatabaseFile() const;
    //@}
        
private:
    //! @name Copy/Assignement
    //@{
    Config& operator=( const Config& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    void LoadExercise( const std::string& file );
    //@}

private:
    //! @name Member data
    //@{    
    std::string physical_;
    std::string terrain_;
    std::string geodatabase_;    
    //@}
};

} // end namespace esri
} // end namespace dispatcher

#endif // __ESRI_Config_h_
