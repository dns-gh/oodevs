// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanType_h_
#define __UrbanType_h_

namespace xml
{
    class xistream;
}

namespace urban
{
    class StaticModel;
}

// =============================================================================
/** @class  UrbanType
    @brief  Urban type
*/
// Created: SLG 2010-03-05
// =============================================================================
class UrbanType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    static UrbanType& GetUrbanType();
    urban::StaticModel& GetStaticModel();
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             UrbanType( xml::xistream& xis );
    virtual ~UrbanType();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< urban::StaticModel > staticModel_;
    static UrbanType* urbanType_;
    //@}
};

#endif // __UrbanType_h_
