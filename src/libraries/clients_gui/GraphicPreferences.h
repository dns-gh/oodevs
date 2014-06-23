// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GraphicPreferences_h_
#define __GraphicPreferences_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include <spatialcontainer/TerrainData.h>
#include <graphics/GraphicSetup_ABC.h>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class TerrainPreference;

// =============================================================================
/** @class  GraphicPreferences
    @brief  Graphic preferences
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicPreferences : public QObject
                         , public GraphicSetup_ABC
                         , public tools::Observer_ABC
                         , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit GraphicPreferences( kernel::Controllers& controllers );
    virtual ~GraphicPreferences();
    //@}

    //! @name Operations
    //@{
    void SetAlpha( float a );

    void Display( QVBoxLayout* parent );

    virtual void SetupLineGraphics  ( const Data_ABC* pData );
    virtual void SetupLineGraphics  ( unsigned int offset );
    virtual void SetupBorderGraphics( const Data_ABC* pData );
    virtual void SetupAreaGraphics  ( const Data_ABC* pData );

    void Commit();
    void Revert();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GraphicPreferences( const GraphicPreferences& );            //!< Copy constructor
    GraphicPreferences& operator=( const GraphicPreferences& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadTerrainPreference( xml::xistream& xis );
    void Save() const;
    void Save( xml::xostream& xos, const TerrainPreference& preference ) const;
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    void Build(); 
    //@}

private slots:
    //! @name Slots
    //@{
    void Up( int );
    void Down( int );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< TerrainData, std::shared_ptr< TerrainPreference > > T_TerrainPreferences;
    typedef std::vector< std::shared_ptr< TerrainPreference > >           T_Displays;
    typedef std::map< std::string, T_Displays >                           T_Categories;
    typedef std::vector< std::string >                                    T_Orders;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_TerrainPreferences terrainPrefs_;
    T_Categories categories_;
    T_Orders currentOrders_;
    T_Orders previousOrders_;
    float alpha_;

    QVBoxLayout* layout_;
    QIcon upIcon_;
    QIcon downIcon_;
    QSignalMapper* upSignalMapper_;
    QSignalMapper* downSignalMapper_;
    //@}
};

}

#endif // __GraphicPreferences_h_
