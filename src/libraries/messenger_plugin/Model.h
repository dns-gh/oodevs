// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

namespace xml
{
    class xistream;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Config;
    class ClientPublisher_ABC;
    class CompositeRegistrable;
}

namespace tools
{
    class IdManager;
}

namespace plugins
{
namespace messenger
{
    class TacticalLinesModel;
    class DrawingsModel;
    class NotesModel;
    class ClientObjectsModel;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: SBO 2008-06-06
// =============================================================================
class Model
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( const dispatcher::Config& config, dispatcher::ClientPublisher_ABC& clients, dispatcher::CompositeRegistrable& registrables );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    void Save( const std::string& name ) const;
    void SendStateToNewClient( dispatcher::ClientPublisher_ABC& client );
    void UpdateTime( std::string time );
    void DeleteUnit( int32_t id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Load();
    void ReadOrbat    ( xml::xistream& xis );
    void ReadSide     ( xml::xistream& xis );
    void ReadFormation( xml::xistream& xis );
    void ReadAutomat  ( xml::xistream& xis );
    void ReadUnit     ( xml::xistream& xis );
    void ReadMessenger( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< tools::IdManager > idManager_;
    std::unique_ptr< kernel::CoordinateConverter_ABC > converter_;
    //@}

public:
    //! @name Member data
    //@{
    const dispatcher::Config& config_;
    TacticalLinesModel& tacticalLines_;
    DrawingsModel& drawings_;
    NotesModel& notes_;
    ClientObjectsModel& clientObjects_;
    //@}
};

}
}

#endif // __Model_h_
