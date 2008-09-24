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

#include "clients_kernel/Resolver.h"
#include "game_asn/Messenger.h"
#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Config;
}

namespace plugins
{
namespace messenger
{
    class Drawing;
    class DrawingProxy;
    class IdManager;

// =============================================================================
/** @class  DrawingsModel
    @brief  DrawingsModel
*/
// Created: SBO 2008-06-06
// =============================================================================
class DrawingsModel : public kernel::Resolver< Drawing >
                    , public dispatcher::Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawingsModel( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter );
    virtual ~DrawingsModel();
    //@}

    //! @name Operations
    //@{
    void Save( const std::string& directory ) const;
    //@}

    //! @name Requests
    //@{
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgShapeCreationRequest&    message );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgShapeDestructionRequest& message );
    void HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgShapeUpdateRequest&      message );
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& publisher ) const;

    void Publish( const Drawing& drawing );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingsModel( const DrawingsModel& );            //!< Copy constructor
    DrawingsModel& operator=( const DrawingsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Load( const dispatcher::Config& config );
    void ReadShape( xml::xistream& xis );
    virtual void RegisterIn( directia::Brain& brain );
    void ReadNamedShape( xml::xistream& xis, std::auto_ptr< Drawing >& result, const std::string& name );
    boost::shared_ptr< DrawingProxy > CreateDrawing( const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Config& config_;
    const kernel::CoordinateConverter_ABC& converter_;
    dispatcher::ClientPublisher_ABC& clients_;
    IdManager& idManager_;
    //@}
};

}
}

#endif // __DrawingsModel_h_
