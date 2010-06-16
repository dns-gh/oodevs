// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanBlockDeserializer_h_
#define __UrbanBlockDeserializer_h_

#include <Urban/UrbanBlockDeserializer_ABC.h>

class ColorRGBA;

namespace MsgsSimToClient
{
    class MsgUrbanCreation;
}

// =============================================================================
/** @class  UrbanBlockDeserializer
@brief  UrbanBlockDeserializer
*/
// Created: SLG 2009-12-04
// =============================================================================
class UrbanBlockDeserializer : public urban::UrbanBlockDeserializer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanBlockDeserializer( const MsgsSimToClient::MsgUrbanCreation& message );
    virtual ~UrbanBlockDeserializer();
    //@}

    //! @name Operations
    //@{
    virtual float        GetArchitectureHeight              () const;
    virtual unsigned int GetArchitectureFloorNumber         () const;
    virtual std::string  GetArchitectureRoofShape           () const;
    virtual std::string  GetArchitectureMaterial            () const;
    virtual float        GetArchitectureOccupation                  () const;
    virtual float        GetArchitectureTrafficability              () const;
    

    std::auto_ptr< ColorRGBA >  GetColor                    () const;

    virtual bool HasColor()         const;
    virtual bool HasArchitecture()  const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanBlockDeserializer( const UrbanBlockDeserializer& );            //!< Copy constructor
    UrbanBlockDeserializer& operator=( const UrbanBlockDeserializer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const MsgsSimToClient::MsgUrbanCreation& message_;
    //@}
};

#endif // __UrbanBlockDeserializer_h_
