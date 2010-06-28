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


#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Extension_ABC.h"
#include "InfrastructureParameters.h"

namespace kernel
{
    class PropertiesDictionary;
}
namespace urban
{
    class Drawer_ABC;
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
                         , public kernel::EntityImplementation< kernel::Entity_ABC > 
                         , public kernel::Updatable_ABC< InfrastructureParameters >
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainObjectProxy( kernel::Controller& controller, urban::TerrainObject_ABC& object, unsigned int id, const QString& name, const InfrastructureParameters& parameters ); 
             TerrainObjectProxy( kernel::Controller& controller, urban::TerrainObject_ABC& object ); 
    virtual ~TerrainObjectProxy();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const InfrastructureParameters& infrastructure );
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Select     ( kernel::ActionController& /*controller*/ ) const {};
    virtual void ContextMenu( kernel::ActionController& /*controller*/,  const QPoint& /*where*/) const {};
    virtual void Activate   ( kernel::ActionController& /*controller*/ ) const {};


    virtual void SetSelected( bool selected )                            const;
    virtual void Draw       ( urban::Drawer_ABC& drawer )                const;
    virtual bool IsInside   ( const geometry::Point2f& point )           const;
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary               ( kernel::Controller& controller );
    void AddDictionaryForArchitecture   ( kernel::PropertiesDictionary& dictionary );
    //@}

public:
    //! @name Member data
    //@{
    urban::TerrainObject_ABC* object_;  // $$$$ _RC_ FDS 2010-01-15: Must be encapsulated -> private
    InfrastructureParameters infrastructure_;
    //@}

public:
    //! @name Copy/Assignement
    //@{
    TerrainObjectProxy ( const TerrainObjectProxy& );            //!< Copy constructor
    TerrainObjectProxy& operator=( const TerrainObjectProxy& ); //!< Assignement operator
    bool operator==( const TerrainObjectProxy& ) const;
    //@}
};

}

#endif // __TerrainObjectProxy_h_
