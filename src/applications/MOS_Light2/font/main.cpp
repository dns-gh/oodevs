#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

#define CHECK( b ) if(!(b)) { std::cerr << #b << std::endl; return EXIT_FAILURE; }

double Round( double r, double rPrecision )
{
    double rSign = r < 0 ? -1. : 1;
    return std::floor( ( rSign * r + 0.5 ) / rPrecision ) * rSign * rPrecision;
}
double Transform( double r )
{
    double result = r; // * 1000. / 916.;
    const double rGrid = 10.;
    result = Round( result, rGrid );
    return result;
};

int main( int argc, char** argv )
{
    CHECK( argc == 2 );
    const std::string strFilename = argv[1];

    std::ifstream input( strFilename.c_str() );
    CHECK( input );

    std::ofstream output( ( "out/" + strFilename ).c_str() );

    std::string strLine;
    std::getline( input, strLine );
    output.clear();
    output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

    while( std::getline( input, strLine ) )
    {
        if( strLine == "<advanceWidth>916.0</advanceWidth>" )
        {
            output << "<advanceWidth>1000.0</advanceWidth>\n";
            continue;
        }

        std::stringstream streamLine( strLine );
        std::string strItem;
        bool bFirstItem = true;
        bool bWasRounded = false;
        while( std::getline( streamLine, strItem, '\"' ) )
        {
            if( ! bFirstItem )
                output << "\"";
            bFirstItem = false;

            // change "rounded"
            if( bWasRounded && strItem == "true" )
            {
                output << "false";
                bWasRounded = false;
                continue;
            }
            bWasRounded = strItem == " rounded=";

            // Scale
            std::stringstream streamItem( strItem );
            double r;
            if( streamItem >> r )
                output <<  std::setiosflags(std::ios::fixed) << std::setprecision(1) << Transform( r );
            else
                output << strItem;
            
        };
        output << "\n";
    };
}
