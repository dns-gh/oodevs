// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CsvExport_h_
#define __CsvExport_h_

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>
#include <vector>

namespace kernel
{
    class Entity_ABC;
    class CoordinateConverter_ABC;
}

class Model;
class Progress_ABC;

// =============================================================================
/** @class  CsvExport
    @brief  Csv export
*/
// Created: LGY 2011-10-18
// =============================================================================
class CsvExport : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CsvExport( Model& model, const kernel::CoordinateConverter_ABC& converter );
    virtual ~CsvExport();
    //@}

    //! @name Operations
    //@{
    void Execute( const std::string& exerciseName, boost::filesystem::path& path, Progress_ABC& progress );
    //@}

private:
    //! @name Helpers
    //@{
    void WriteEntity( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteEntity( const kernel::Entity_ABC& entity, const std::string& side, std::ofstream& file, const std::string& separator );
    void Write( std::ofstream& file, const std::string& separator, const std::string& side, const std::string& category,
                const std::string& name, const std::string& type, const std::string& position );
    void WriteResources( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteStocks( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteWeather( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteDiplomaty( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteProfiles( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteProfiles( std::ofstream& file, const std::string& separator, const kernel::Entity_ABC& entity,
                        const std::set< std::string >& profiles );
    void WriteDiffusion( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteReceiver( std::ofstream& file, const std::string& separator, const kernel::Entity_ABC& entity );
    void WriteTransmitter( std::ofstream& file, const std::string& separator, const kernel::Entity_ABC& entity );
    void WriteTransmitter( std::ofstream& file, const std::string& separator, const kernel::Entity_ABC& entity,
                           const std::vector< std::string >& list );
    void WriteLogistic( const std::string& exerciseName, boost::filesystem::path& path, const std::string& separator );
    void WriteLogistic( std::ofstream& file, const std::string& separator, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member Data
    //@{
    Model& model_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __CsvExport_h_
