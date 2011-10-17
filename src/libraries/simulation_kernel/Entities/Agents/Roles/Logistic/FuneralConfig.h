// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FuneralConfig_h_
#define __FuneralConfig_h_

namespace tools
{
    template< typename T > class Iterator;
}

namespace logistic
{
class FuneralPackagingResource;

class FuneralConfig
{
public:
    //! @name Ugly static initialization
    //@{
    static void Initialize( xml::xistream& xis, unsigned tickDuration );
    static void Terminate ();

    static void   GetOrderedPackagingResources( std::vector< const FuneralPackagingResource* >& container, const FuneralPackagingResource* startingFrom = 0 );
    static double GetFakeTransporterSpeed  ();
    //@}

private:
    //! @name Ugly static initialization
    //@{
    static void ReadPackagingResource( xml::xistream& xis );
    //@}

public:
    static std::vector< const FuneralPackagingResource* > orderedPackagingResources_;
    static double fakeTransporterSpeed_;
};

} // end namespace logistic

#endif // __FuneralConfig_h_
