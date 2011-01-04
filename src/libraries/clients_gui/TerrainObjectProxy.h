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

#include "clients_kernel/Creatable.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Displayable_ABC.h"

namespace kernel
{
    class GlTools_ABC;
    class PropertiesDictionary;
    class Viewport_ABC;
    class Displayer_ABC;
}

namespace sword
{
    class UrbanUpdate;
}

namespace urban
{
    class Drawer_ABC;
    class TerrainObject_ABC;
}

namespace gui
{
// =============================================================================
/** @class  TerrainObjectProxy
    @brief  Terrain object proxy
*/
// Created: SLG 2009-02-10
// =============================================================================
class TerrainObjectProxy : public kernel::Extension_ABC
                         , public kernel::EntityImplementation< kernel::Entity_ABC >
                         , public kernel::Updatable_ABC< sword::UrbanUpdate >
                         , public kernel::Creatable< TerrainObjectProxy >
                         , public kernel::Displayable_ABC
                         , public tools::Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainObjectProxy( kernel::Controller& controller, urban::TerrainObject_ABC& object, unsigned int id, const QString& name );
             TerrainObjectProxy( kernel::Controller& controller, urban::TerrainObject_ABC& object );
    virtual ~TerrainObjectProxy();
    //@}

    //! @name Copy/Assignment
    //@{
    TerrainObjectProxy( const TerrainObjectProxy& );           //!< Copy constructor
    TerrainObjectProxy& operator=( const TerrainObjectProxy& ); //!< Assignment operator
    //@}

    //! @name Operators
    //@{
    bool operator==( const TerrainObjectProxy& ) const;
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    geometry::Point2f Barycenter() const;
    const geometry::Polygon2f* GetFootprint() const;
    const urban::TerrainObject_ABC* GetObject() const;
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UrbanUpdate& msg );
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/) const {}
    virtual void Activate( kernel::ActionController& /*controller*/ ) const {}
    virtual void SetSelected( bool selected ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;

    void UpdateHumans( const std::string& inhabitant, unsigned int number );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void AddDictionaryForArchitecture( kernel::PropertiesDictionary& dictionary );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_Humans;
    typedef T_Humans::const_iterator            CIT_Humans;
    //@}

private:
    //! @name Member data
    //@{
    urban::TerrainObject_ABC& object_;
    kernel::Controller& controller_;
    T_Humans humans_;
    //@}
};

}

#endif // __TerrainObjectProxy_h_
