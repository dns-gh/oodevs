require "lunit"

integration = {}
masalife = {}
masalife.brain = {}
masalife.brain.communication = {}
masalife.brain.integration = {}
masalife.brain.integration.createBrainLocalStorage = function() end
masalife.brain.knowledge = {}
masalife.brain.communication.protocol = {}
masalife.brain.communication.protocol.leaveGroup = {}
masalife.brain.communication.protocol.setDynamicCommFunID = function() end


dofile "../integration/ToolsFunctions.lua"
dofile "../integration/TableIntegration.lua"


-- -------------------------------------------------------------------------------- 
-- Element activation or send a done feedback if no elements to activate
-- --------------------------------------------------------------------------------
module( "tables functions tests", package.seeall, lunit.testcase )

setup = function ()
end

-- The function existsIndex returns 'true' if the issued table is indexed with the issued value
existsIndex_TEST = function()
	local myTable = {}
	myTable = { myString = { "hello" }, "goodbye", true, false }
	local myIndex = 2
	local myResult = existsIndex( myTable, myIndex )
	assert_equal( true, myResult )

	myIndex = nil
	myResult = existsIndex( myTable, myIndex )
	assert_equal( false, myResult )

	myIndex = "hello"
	myResult = existsIndex( myTable, myIndex )
	assert_equal( false, myResult )

	myIndex = 1000
	myResult = existsIndex( myTable, myIndex )
	assert_equal( false, myResult )

	myTable[ "myStringIndex1" ] = true
	myResult = existsIndex( myTable, "myStringIndex1" )
	assert_equal( true, myResult )

	myResult = existsIndex( myTable, "hey" )
	assert_equal( false, myResult )

	myTable[ true ] = "hoho"
	myResult = existsIndex( myTable, true )
	assert_equal( true, myResult )

	local myOtherTable = { string = "hello" }
	myTable[ myOtherTable ] = "goodbye"
	myResult = existsIndex( myTable, myOtherTable )
	assert_equal( true, myResult )

	myResult = existsIndex( myTable, myOtherTable.string )
	assert_equal( false, myResult )

end

-- Compare the issued value with elements of issued table.
exists_TEST = function()
	local myTable = { 1, 2, 3, 4 }
	local myValue = 2
	local result = exists( myTable, myValue )
	assert_equal( true, result )

	myTable = { myString = { "hello" }, "goodbye", true, false }
	myValue = "hello"
	result = exists( myTable, myValue )
	assert_equal( false, result  )

	myValue = true
	result = exists( myTable, myValue)
	assert_equal( true, result  )

	myValue = "goodbye"
	result = exists( myTable, myValue )
	assert_equal( true, result  )

	myValue = { "hello" } -- same value but not the same table
	result = exists( myTable, myValue )
	assert_equal( false, result  )

	result = exists( myTable, myTable.myString ) -- same table
	assert_equal( true, result  )

	result = exists( myTable, myTable[ 12 ] )
	assert_equal( false, result  )
	result = exists( myTable, myTable.nilValue )
	assert_equal( false, result  )

end



-- Test fusionList method. Fusion list method returns the fusionned table from two tables.
-- Only différents values are fusionned
fusionList_TEST = function()
	local filledListOK1 = { 1, 2, 3 }       -- size is 3
	local filledListOK2 = { "1", "2", "3" } -- size is 3
	local filledListOK3 = { 2, "hello",3, 6, true } -- size is 5, 2 value in common with filledListOK1
	local emptyList = {}
	local badIndexList = {}
	badIndexList[ "astring" ] = true
	badIndexList[ 2 ] = false
	badIndexList[ "trois" ] = "hey buddy!!"

	-- test 1
	local finalList = fusionList( filledListOK1, filledListOK2 )
	assert_equal(  #filledListOK1 + #filledListOK2, #finalList )

    -- test 2
	local finalList = fusionList( filledListOK1, filledListOK3 )
	assert_equal( 6, #finalList )

    -- test 3
	local finalList = fusionList( filledListOK1, emptyList )
	assert_equal( #filledListOK1, #finalList )

    -- test 4
	local finalList = fusionList( filledListOK2, filledListOK3 )
	assert_equal( #filledListOK1 + #filledListOK3, #finalList )

	-- test 5
	local finalList = fusionList( emptyList, filledListOK1 )
	assert_equal( #filledListOK1, #finalList )

    -- test 6
	local finalList = fusionList( emptyList, badIndexList )
	assert_equal( 3 , #finalList )

	assert_table( finalList )
end

-- The minValue function should returns the min value among numeric values of issued the tables
minValue_TEST = function()
	local myNumericTable = { 1, 56, 2, 32, 0 }
	local minVal = minValue( myNumericTable )
	assert_equal( 0, minVal )

	local myNumericTable = { 48, 56, 789, 32, 12 }
	local minVal = minValue( myNumericTable )
	assert_equal( 12, minVal )
end

explode_TEST = function()
	-- Test 1
	local t = explode( ";", "a;b;c;d" )
	assert_table( t )
	assert_equal( 4, #t )
	assert_equal( "a", t[1] )
	assert_equal( "b", t[2] )
	assert_equal( "c", t[3] )
	assert_equal( "d", t[4] )

	-- Test 2 (the string starts with the separator)
	t = explode( ";", ";b;c" )
	assert_table( t )
	assert_equal( 3, #t )
	assert_equal( "", t[1] )
	assert_equal( "b", t[2] )
	assert_equal( "c", t[3] )

	-- Test 3 (the string only contains multiple occurrences of the separator)
	t = explode( ";", ";;" )
	assert_table( t )
	assert_equal( 3, #t )
	assert_equal( "", t[1] )
	assert_equal( "", t[2] )
	assert_equal( "", t[3] )

	-- Test 4 (the string does not contain the separator)
	t = explode( "!", "a;b;c;d" )
	assert_table( t )
	assert_equal( 1, #t )
	assert_equal( "a;b;c;d", t[1] )

	-- Test 5 (the separator is empty)
	assert_error( function() return explode( "", "abc" ) end )
end

existsInside_TEST = function()
	local t = { { 1, 2, 3 }, 
		    { "a", "b", "c" }, 
		    {}, 
		    { { "X" } } }
		   
	assert_true( existsInside( t, 1 ) )
	assert_true( existsInside( t, "b" ) )
	assert_false( existsInside( t, "e" ) )
	assert_false( existsInside( t, nil ) )
	assert_false( existsInside( t, { "X" } ) )
	assert_true( existsInside( t, t[4][1] ) )
end

removeFromList_TEST = function()
	local list = { "a", "b", "c" }
	local index

	-- Test 1
	list, index = removeFromList( "b", list )
	assert_table( list )
	assert_equal( 2, #list )
	assert_equal( "a", list[1] )
	assert_equal( "c", list[2] )
	assert_number( index )
	assert_equal( 2, index )

	-- Test 2 (the element to remove is not in the list)
	list, index = removeFromList( "d", list )
	assert_table( list )
	assert_equal( 2, #list )
	assert_equal( "a", list[1] )
	assert_equal( "c", list[2] )
	assert_number( index )
	assert_equal( 0, index )

	-- Test 3
	list, index = removeFromList( "a", list )
	assert_table( list )
	assert_equal( 1, #list )
	assert_equal( "c", list[1] )
	assert_number( index )
	assert_equal( 1, index )

	-- Test 4
	list, index = removeFromList( "c", list )
	assert_table( list )
	assert_equal( 0, #list )
	assert_number( index )
	assert_equal( 1, index )
end

removeListFromList_TEST = function()
	local list = { "a", "b", "c", "d", "e" }

	-- Test 1
	list = removeListFromList( { "b", "d" } , list )
	assert_table( list )
	assert_equal( 3, #list )
	assert_equal( "a", list[1] )
	assert_equal( "c", list[2] )
	assert_equal( "e", list[3] )

	-- Test 2 (some values to remove are not in the list)
	list = removeListFromList( { "x", "a", "y" }, list )
	assert_table( list )
	assert_equal( 2, #list )
	assert_equal( "c", list[1] )
	assert_equal( "e", list[2] )

	-- Test 3 (no value to remove)
	list = removeListFromList( {}, list )
	assert_table( list )
	assert_equal( 2, #list )
	assert_equal( "c", list[1] )
	assert_equal( "e", list[2] )

	-- Test 4 (some values to remove are not in the list)
	list = removeListFromList( { "a", "c", "e" }, list )
	assert_table( list )
	assert_equal( 0, #list )
end

copyTable_TEST = function()
	-- Test 1 (copying an empty table)
	local t1 = {}
	local copy1 = copyTable( t1 )
	assert_table( copy1 )
	assert_equal( 0, #copy1 )

	-- Test 2 (copying a list)
	local t2 = { "a", "b", "c" }
	local copy2 = copyTable( t2 )
	assert_table( copy2 )
	assert_equal( 3, #copy2 )
	assert_equal( "a", copy2[1] )
	assert_equal( "b", copy2[2] )
	assert_equal( "c", copy2[3] )

	-- Test 3 (copying a table)
	local t3 = { x = 10, y = 20 }
	local copy3 = copyTable( t3 )
	assert_table( copy3 )
	assert_equal( 10, copy3.x )
	assert_equal( 20, copy3.y )

	-- Test 4 (copying a table of table)
	local t4 = { { "X" } }
	local copy4 = copyTable( t4 )
	assert_table( copy4 )
	assert_equal( 1, #copy4 )
	assert_not_equal( { "X" }, copy4[1] )
	assert_equal( t4[1], copy4[1] )
end

tableSize_TEST = function()
	-- Test 1 (size of an empty table)
	local t1 = {}
	assert_equal( 0, tableSize( t1 ) )

	-- Test 2 (size of a list)
	local t2 = { "a", "b", "c" }
	assert_equal( 3, tableSize( t2 ) )

	-- Test 3 (size of a table)
	local t3 = { x = 10, y = 20, 30, 40, 50 }
	assert_equal( 5, tableSize( t3 ) )

	-- Test 4 (size of a table of table)
	local t4 = { { "X" } }
	assert_equal( 1, tableSize( t4 ) )
end

inverseTable_TEST = function()
	-- Test 1 (inversing an empty table)
	local t = integration.inverseTable( {} )
	assert_table( t )
	assert_equal( 0, #t )

	-- Test 2 (inversing a table with a single element)
	t = integration.inverseTable( { "a" } )
	assert_table( t )
	assert_table( t["a"] )
	assert_equal( 1, t["a"][1] )
	
	-- Test 3 (inversing a table with multiple times occurring elements)
	t = integration.inverseTable( { "a", "b", "a" } )
	assert_table( t )
	assert_table( t["a"] )
	assert_equal( 2, #t["a"] )
	assert_equal( 1, t["a"][1] )
	assert_equal( 3, t["a"][2] )
	assert_table( t["b"] )
	assert_equal( 1, #t["b"] )
	assert_equal( 2, t["b"][1] )

	-- Test 4
	t = integration.inverseTable( { x = 10, y = 20, z = 30 } )
	assert_table( t )
	assert_table( t[10] )
	assert_equal( "x", t[10][1] )
	assert_table( t[20] )
	assert_equal( "y", t[20][1] )
	assert_table( t[30] )
	assert_equal( "z", t[30][1] )
end

teardown = function()
end
