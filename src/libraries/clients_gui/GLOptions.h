// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GLOptions_h
#define GLOptions_h

//#include "FireOption.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/SafePointer.h"
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

namespace kernel
{
    class AccommodationTypes;
    class Controllers;
    class DetectionMap;
    class Entity_ABC;
    class EntityResolver_ABC;
    class FourStateOption;
    class OptionVariant;
    class Options;
    class Profile_ABC;
    class Settings;
    class StaticModel;
    class UrbanObject_ABC;
}

class QColor;
class Lighting_ABC;

namespace gui 
{
    class ColorStrategy_ABC;
    class ContourLinesComputer;
    class Elevation2dTexture;
    class TerrainSettings;
    class UrbanDisplayOptions;
    class WatershedTexture;

// =============================================================================
/** @class  GLOptions
    @brief  GLOptions
*/
// Created: ABR 2014-06-20
// =============================================================================
class GLOptions : private boost::noncopyable
                , public tools::Observer_ABC
                , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    // create a GLOptions sharing kernel::Options pointer with OptionsController
             GLOptions( kernel::Controllers& controllers,
                        const kernel::Profile_ABC& profile,
                        const kernel::StaticModel& staticModel,
                        const kernel::EntityResolver_ABC& model,
                        const std::shared_ptr< Lighting_ABC >& lighting );
    virtual ~GLOptions();
    //@}

    //! @name Copy/Assignment
    //@{
    explicit GLOptions( const GLOptions& other );   //!< Copy constructor, new shared pointers
    GLOptions& operator=( const GLOptions& other ); //!< Assignment operator, same shared pointers with updated values
    //@}

    //! @name Serialisation
    //@{
    void Load();
    void Load( kernel::Settings& settings );
    void Save( kernel::Settings& settings );
    //@}

    //! @name Four states
    //@{
    void Select( bool selected, bool superiorSelected, bool controlled );
    bool IsDrawingSelection() const;
    bool ShouldDisplay( const std::string& fourStateOptionName ) const;
    //@}

    //! @name Operations
    //@{
    bool Has( const std::string& name ) const;
    const kernel::OptionVariant& Get( const std::string& name ) const;
    void Set( const std::string& name, const kernel::OptionVariant& value );
    void Remove( const std::string& name );

    float GetRatio( const std::string& level ) const;

    const kernel::Entity_ABC* GetFilterEntity() const;
    void SetFilterEntity( const kernel::Entity_ABC* filterEntity );

    const kernel::Entity_ABC* GetLockedEntity() const;
    void SetLockedEntity( const kernel::Entity_ABC* lockedEntity );

    geometry::Point2f GetCenterPosition();

    bool IsAggregated( const kernel::Entity_ABC& entity ) const;
    const std::vector< const kernel::Entity_ABC* >& GetAggregatedAutomats() const;
    const std::vector< const kernel::Entity_ABC* >& GetAggregatedFormations() const;
    void Aggregate( const kernel::Entity_ABC& entity );
    void Disaggregate( const kernel::Entity_ABC* entity = 0 );

    //const T_FireOptions& GetFireOptions( FireGroup group ) const;

    void SetContourLinesComputer( const std::shared_ptr< ContourLinesComputer >& computer );
    std::shared_ptr< ContourLinesComputer >& GetContourLinesComputer();
    const std::shared_ptr< ContourLinesComputer >& GetContourLinesComputer() const;

    WatershedTexture& GetWatershedTexture();
    const WatershedTexture& GetWatershedTexture() const;
    const std::shared_ptr< Elevation2dTexture >& GetElevation2dTexture() const;
    const std::shared_ptr< TerrainSettings >& GetTerrainSettings() const;
    const std::shared_ptr< Lighting_ABC >& GetLighting() const;

    void SetLighting();
    QColor ComputeUrbanColor( const kernel::UrbanObject_ABC& object ) const;
    //@}

private:
    //! @name Helpers
    //@{
    bool ShouldDisplay( const std::string& name, bool b1, bool b2, bool b3 ) const;
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    const kernel::DetectionMap& map_;
    const kernel::EntityResolver_ABC& model_;
    const kernel::AccommodationTypes& accommodationTypes_;

    // View options
    bool selected_;
    bool superiorSelected_;
    bool controlled_;
    std::shared_ptr< kernel::Options > options_;
    //std::vector< T_FireOptions > fires_;
    std::shared_ptr< ContourLinesComputer > contourLinesComputer_;
    std::shared_ptr< Elevation2dTexture > elevation2dTexture_;
    std::shared_ptr< TerrainSettings > graphicSetup_;
    std::shared_ptr< Lighting_ABC > lighting_;
    std::unique_ptr< UrbanDisplayOptions > urbanSetup_;
    std::unique_ptr< WatershedTexture > watershedTexture_;

    // Exercise options
    kernel::SafePointer< kernel::Entity_ABC > filterEntity_;
    kernel::SafePointer< kernel::Entity_ABC > lockedEntity_;

    std::vector< const kernel::Entity_ABC* > aggregatedAutomats_;
    std::vector< const kernel::Entity_ABC* > aggregatedFormations_;
    //@}
};

} //! namespace gui

#endif // GLOptions_h
