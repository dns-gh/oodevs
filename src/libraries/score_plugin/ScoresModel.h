// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoresModel_h_
#define __ScoresModel_h_

#include "dispatcher/Registrable_ABC.h"
#pragma warning( push, 0 )
#include <qdatetime.h>
#pragma warning( pop )
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>
#include <map>

namespace sword
{
    class SimToClient;
    class PlotRequest;
    class Indicator;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Profile_ABC;
}

namespace tools
{
    class SessionConfig;
}

namespace xml
{
    class xistream;
}

namespace aar
{
    class StaticModel;
}

class Task;

namespace plugins
{
namespace score
{
    class IndicatorBuilder;
    class Score;
    class ScoreAnnouncer;

    struct Variable
    {
        Variable( const std::string& name, const std::string& type, const std::string& value )
            : name_( name )
            , type_( type )
            , value_( value )
        {
            // NOTHING
        }
        std::string name_;
        std::string type_;
        std::string value_;
    };

// =============================================================================
/** @class  ScoresModel
    @brief  Scores model
*/
// Created: SBO 2009-08-20
// =============================================================================
class ScoresModel : public dispatcher::Registrable_ABC
                  , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ScoresModel( const tools::SessionConfig& config, dispatcher::ClientPublisher_ABC& clients );
    virtual ~ScoresModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& file );
    void Update( const sword::Indicator& message );
    void Update( const sword::SimToClient& message );
    void RequestPlot( dispatcher::ClientPublisher_ABC& publisher, const sword::PlotRequest& request );
    void Export() const;
    void SimplifiedExport( const std::string& path ) const;
    void SendInformation( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< Score > T_Score;
    typedef std::map< std::string, T_Score > T_Scores;
    typedef boost::shared_ptr< Task > T_Task;
    typedef std::vector< T_Task > T_Tasks;
    //@}

    //! @name Helpers
    //@{
    void ReadIndicators( xml::xistream& xis );
    void ReadIndicator( xml::xistream& xis );
    virtual void RegisterIn( directia::brain::Brain& brain );
    boost::shared_ptr< Variable > CreateVariable( const std::string& name, const std::string& type, const std::string& value );
    void ComputeIndicator( const std::string& name, const std::string& formula, const std::vector< boost::shared_ptr< Variable > >& variables );
    std::size_t AddHeader( std::ostream& file ) const;
    void AddLine( std::ostream& file, std::size_t index ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    T_Tasks tasks_;
    std::auto_ptr< IndicatorBuilder > builder_;
    std::auto_ptr< aar::StaticModel > model_;
    std::auto_ptr< ScoreAnnouncer > announcer_;
    T_Scores scores_;
    QDateTime initialDateTime_;
    bool dateTimeInitialized_;
    unsigned int tickDuration_;
    const std::string sessionDir_;
    const std::string separator_;
    //@}
};

}
}

#endif // __ScoresModel_h_
