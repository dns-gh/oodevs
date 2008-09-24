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

namespace plugins
{
namespace messenger
{
    class IdManager;
    class TacticalLinesModel;
    class IntelligencesModel;
    class DrawingsModel;

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
    void ReadSide     ( xml::xistream& xis );
    void ReadFormation( xml::xistream& xis );
    void ReadAutomat  ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< IdManager > idManager_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    //@}

public:
    //! @name Member data
    //@{
    const dispatcher::Config& config_;
    TacticalLinesModel& tacticalLines_;
    IntelligencesModel& intelligences_;
    DrawingsModel&      drawings_;
    //@}
};

}
}

#endif // __Model_h_
