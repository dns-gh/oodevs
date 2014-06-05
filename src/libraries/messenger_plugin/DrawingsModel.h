// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingsModel_h_
#define __DrawingsModel_h_

#include "dispatcher/Registrable_ABC.h"
#include <tools/Resolver.h>
#include <boost/optional/optional_fwd.hpp>

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Config;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace sword
{
    class Diffusion;
    class ShapeCreationRequest;
    class ShapeDestructionRequest;
    class ShapeUpdateRequest;
}

namespace tools
{
    class IdManager;
    class Path;
}

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace plugins
{
namespace messenger
{
    class Drawing;
    class DrawingProxy;

// =============================================================================
/** @class  DrawingsModel
    @brief  DrawingsModel
*/
// Created: SBO 2008-06-06
// =============================================================================
class DrawingsModel : public tools::Resolver< Drawing >
                    , public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingsModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, tools::IdManager& idManager, const kernel::CoordinateConverter_ABC& converter );
    virtual ~DrawingsModel();
    //@}

    //! @name Operations
    //@{
    void Save( const tools::Path& directory ) const;
    //@}

    //! @name Requests
    //@{
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ShapeCreationRequest&    message, unsigned int context );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ShapeDestructionRequest& message );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const sword::ShapeUpdateRequest&      message );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;

    void Publish( const Drawing& drawing );
    //@}

private:
    //! @name Helpers
    //@{
    void Load( const dispatcher::Config& config );
    void ReadShapes( xml::xisubstream xis );
    void ReadAutomat( xml::xistream& xis );
    void ReadFormation( xml::xistream& xis );
    void ReadShape( xml::xistream& xis, const boost::optional< sword::Diffusion >& diffusion );
    virtual void RegisterIn( directia::brain::Brain& brain );
    void ReadNamedShape( xml::xistream& xis, std::unique_ptr< Drawing >& result, const std::string& name );
    boost::shared_ptr< DrawingProxy > CreateDrawing( const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Config& config_;
    const kernel::CoordinateConverter_ABC& converter_;
    dispatcher::ClientPublisher_ABC& clients_;
    tools::IdManager& idManager_;
    //@}
};

}
}

#endif // __DrawingsModel_h_
