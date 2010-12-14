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
#include <boost/shared_ptr.hpp>
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
}

namespace xml
{
    class xistream;
}

class Task;

namespace plugins
{
namespace score
{
    class IndicatorBuilder;
    class Score;

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
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ScoresModel( dispatcher::ClientPublisher_ABC& clients );
    virtual ~ScoresModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& file );
    void Update( const sword::Indicator& message );
    void Update( const sword::SimToClient& message );
    void RequestPlot( dispatcher::ClientPublisher_ABC& publisher, const sword::PlotRequest& request );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoresModel( const ScoresModel& );            //!< Copy constructor
    ScoresModel& operator=( const ScoresModel& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, Score* > T_Scores;
    //@}

    //! @name Helpers
    //@{
    void ReadIndicators( xml::xistream& xis );
    void ReadIndicator( xml::xistream& xis );
    virtual void RegisterIn( directia::brain::Brain& brain );
    boost::shared_ptr< Variable > CreateVariable( const std::string& name, const std::string& type, const std::string& value );
    void ComputeIndicator( const std::string& name, const std::string& formula, const std::vector< boost::shared_ptr< Variable > >& variables );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    std::vector< boost::shared_ptr< Task > > tasks_;
    std::auto_ptr< IndicatorBuilder > builder_;
    T_Scores scores_;
    //@}
};

    }
}

#endif // __ScoresModel_h_
