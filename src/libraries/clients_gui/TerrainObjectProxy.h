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
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class PropertiesDictionary;
    class ObjectType;
    class Displayer_ABC;
    class Controllers;
}

namespace sword
{
    class PopulationUpdate_BlockOccupation;
    class UrbanUpdate;
}

namespace urban
{
    class TerrainObject_ABC;
}

namespace gui
{
    class UrbanDisplayOptions;

// =============================================================================
/** @class  TerrainObjectProxy
    @brief  Terrain object proxy
*/
// Created: SLG 2009-02-10
// =============================================================================
class TerrainObjectProxy : public kernel::Extension_ABC
                         , public kernel::EntityImplementation< kernel::Object_ABC >
                         , public kernel::Updatable_ABC< sword::UrbanUpdate >
                         , public kernel::Creatable< TerrainObjectProxy >
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< UrbanDisplayOptions >
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TerrainObjectProxy( kernel::Controllers& controllers, urban::TerrainObject_ABC& object, unsigned int id, const QString& name, const kernel::ObjectType& type, UrbanDisplayOptions& options );
             TerrainObjectProxy( kernel::Controllers& controllers, urban::TerrainObject_ABC& object, const kernel::ObjectType& type, UrbanDisplayOptions& options );
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

    virtual bool IsInside( const geometry::Point2f& point ) const;
    geometry::Point2f Barycenter() const;
    const geometry::Polygon2f* GetFootprint() const;
    const urban::TerrainObject_ABC* GetObject() const;
    virtual void Display( kernel::Displayer_ABC& ) const {}
    virtual const kernel::ObjectType& GetType() const { return type_; }
    virtual QString GetTypeName() const;
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UrbanUpdate& msg );
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void Activate( kernel::ActionController& /*controller*/ ) const {}
    virtual void SetSelected( bool selected ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;

    void UpdateHumans( const std::string& inhabitant, const std::map< QString, unsigned int >& occupations, bool alerted, bool confined, bool evacuated, float angriness );
    void NotifyUpdated( const UrbanDisplayOptions& );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void AddDictionaryForArchitecture( kernel::PropertiesDictionary& dictionary );
    void Restore();
    float GetDensity() const;
    unsigned int GetHumans() const;
    void UpdateColor();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, float > T_Motivations;

    typedef std::map< QString, unsigned int >   T_BlockOccupation;
    typedef T_BlockOccupation::const_iterator CIT_BlockOccupation;

    struct T_Human
    {
        T_BlockOccupation persons_;
        bool alerted_;
        bool confined_;
        bool evacuated_;
        float angriness_;
    };
    typedef std::map< std::string, T_Human > T_Humans;
    typedef T_Humans::const_iterator       CIT_Humans;

    struct T_BaseColor
    {
        T_BaseColor()
            : red_  ( 0 )
            , green_( 0 )
            , blue_ ( 0 )
        {
            // NOTHING
        }
        unsigned short red_;
        unsigned short green_;
        unsigned short blue_;
    };
    //@}

private:
    //! @name Member data
    //@{
    urban::TerrainObject_ABC& object_;
    kernel::Controllers& controllers_;
    T_Humans humans_;
    T_BaseColor color_;
    const kernel::ObjectType& type_;
    T_Motivations motivations_;
    UrbanDisplayOptions& options_;
    //@}
};

}

#endif // __TerrainObjectProxy_h_
