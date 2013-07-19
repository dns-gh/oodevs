#include "clients_kernel_test_pch.h"

#include "clients_kernel/XQuery.h"
#pragma warning( push, 0 )
#include <Qt/QApplication.h>
#include <QtCore/QString>
#include <QtCore/QStringList>
#pragma warning( pop )
#include <iostream>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

namespace
{
    void CheckQuery( const QString& xmlInput, const QString& query, const QStringList& expectedResult )
    {
        QStringList result = kernel::xquery::Evaluate( xmlInput, query );
        BOOST_CHECK_EQUAL_COLLECTIONS( result.begin(), result.end(), expectedResult.begin(), expectedResult.end() );
    }
}

BOOST_AUTO_TEST_CASE( TestXQuery )
{
    int argc = 0;
    char** argv = 0;
    QApplication qapp( argc, argv );
    const QString xmlInput(
"<?xml version='1.0' encoding='UTF-8'?>"
"<cookbook>"
    "<recipe xml:id='MushroomSoup'>"
        "<title>Quick and Easy Mushroom Soup</title>"
        "<ingredient name='Fresh mushrooms' quantity='100' unit='pieces'/>"
        "<ingredient name='Garlic' quantity='1' unit='cloves'/>"
        "<ingredient name='Milk' quantity='200' unit='milliliters'/>"
        "<ingredient name='Water' quantity='200' unit='milliliters'/>"
        "<ingredient name='Cream' quantity='100' unit='milliliters'/>"
    "</recipe>"
"</cookbook>" );

    CheckQuery( xmlInput, "/cookbook/recipe/string(foobar)", QStringList() );
    CheckQuery( xmlInput, "/cookbook/recipe/string(title)", QStringList() << "Quick and Easy Mushroom Soup" );
    CheckQuery( xmlInput, "/cookbook/recipe/ingredient/string(@name)", QStringList() << "Fresh mushrooms" << "Garlic" << "Milk" << "Water" << "Cream" );
    CheckQuery( xmlInput, "/cookbook/recipe/ingredient[@unit='milliliters']/string(@name)", QStringList() << "Milk" << "Water" << "Cream" );
    CheckQuery( xmlInput, "/cookbook/recipe/ingredient[@quantity='200']/string(@name)", QStringList() << "Milk" << "Water" );
    CheckQuery( xmlInput, "/cookbook/recipe/ingredient[@unit='milliliters'][@quantity='100']/string(@name)", QStringList() << "Cream" );
    CheckQuery( xmlInput, "/cookbook/recipe/ingredient/concat(string(@quantity), concat(' ', string(@name)))", QStringList() << "100 Fresh mushrooms" << "1 Garlic" << "200 Milk" << "200 Water" << "100 Cream" );
}
