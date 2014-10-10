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

namespace kernel
{
    class Options;
    class OptionsController;
}

namespace gui
{
    class TerrainPreference;
    class TerrainSettings;
    class TerrainPreferenceWidget;

// =============================================================================
/** @class  GraphicPreferences
    @brief  Graphic preferences
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicPreferences : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit GraphicPreferences( kernel::OptionsController& options,
                                 const std::shared_ptr< TerrainSettings >& settings,
                                 QWidget* parent = 0 );
    virtual ~GraphicPreferences();
    //@}

    //! @name Operations
    //@{
    void Display( QVBoxLayout* parent );
    void Load( const kernel::Options& options );
    //@}

private:
    //! @name Helpers
    //@{
    void Save() const;
    void Save( xml::xostream& xos, const TerrainPreference& preference ) const;
    void Build(); 
    void Swap( int direction, QWidget* widget );
    //@}

private slots:
    //! @name Slots
    //@{
    void Up( QWidget* );
    void Down( QWidget* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    std::shared_ptr< TerrainSettings > settings_;
    std::map< QString, TerrainPreferenceWidget* > widgets_;
    QStringList orders_;

    QVBoxLayout* layout_;
    QIcon upIcon_;
    QIcon downIcon_;
    QSignalMapper* upSignalMapper_;
    QSignalMapper* downSignalMapper_;
    //@}
};

} //! namespace gui

#endif // __GraphicPreferences_h_
