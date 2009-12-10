// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainObjectProxy_h_
#define __TerrainObjectProxy_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class PropertiesDictionary;
}
namespace urban
{
    class TerrainObject_ABC;
}

namespace gui
{

// =============================================================================
/** @class  UrbanModel
@brief  UrbanModel
*/
// Created: SLG 2009-02-10
// =============================================================================
class TerrainObjectProxy : public kernel::Extension_ABC
                         //, public kernel::Entity_ABC
                         , public kernel::EntityImplementation< kernel::Entity_ABC > 
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainObjectProxy( const ASN1T_MsgUrbanCreation& asn, kernel::Controller& controller, urban::TerrainObject_ABC& object ); 
    virtual ~TerrainObjectProxy();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Select( kernel::ActionController& /*controller*/ ) const {};
    virtual void ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/) const {};
    virtual void Activate( kernel::ActionController& /*controller*/ ) const {};
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void AddDictionaryForArchitecture   ( kernel::PropertiesDictionary& dictionary );
    void AddDictionaryForVegetation     ( kernel::PropertiesDictionary& dictionary );
    void AddDictionaryForSoil           ( kernel::PropertiesDictionary& dictionary );
    //@}


public:
    //! @name Member data
    //@{
    urban::TerrainObject_ABC* object_;

    //@}

public:
    //! @name Copy/Assignement
    //@{
    TerrainObjectProxy( const TerrainObjectProxy& );            //!< Copy constructor
    TerrainObjectProxy& operator=( const TerrainObjectProxy& ); //!< Assignement operator
    bool operator==( const TerrainObjectProxy& ) const;
    //@}
};

}

#endif // __TerrainObjectProxy_h_
