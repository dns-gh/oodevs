utilities = utilities or {}

--- Return true if the provided element is a value of the given table.
-- @param t Table
-- @param k Any type, the sought element
-- @return Boolean
utilities.exists = function( t, k )
    for _, content in pairs( t ) do
        if content == k then
            return true
        end
    end
    return false
end

--- Returns true if the given table of table contains a table with
-- the provided element as a value.
-- @param t Table containing only tables as values
-- @param k Any type, the sought element
-- @return Boolean
utilities.existsInside = function( t, k )
    for _, content in pairs( t ) do
        for _, element in pairs( content ) do
            if element == k then
                return true
            end
        end
    end
    return false
end

--- Returns true if the provided index is a key of the given table.
-- @param t Table
-- @param i Any type, the sought index
-- @return Boolean
utilities.existsIndex = function( t, i )
    return t[i] ~= nil
end

--- Returns a list containing all the values of the two given tables.
--- Values that appear in both tables only appear once in the returned list.
-- @param table1 First table
-- @param table2 Second table
-- @return List
utilities.fusionList = function( table1, table2 )
    local res = {}
    for _, elem in pairs( table1 or emptyTable ) do
        res[ #res+1 ] = elem
    end
    for _, elem in pairs( table2 or emptyTable ) do
        if not exists( res, elem ) then
            res[ #res+1 ] = elem
        end
    end
    return res
end

--- Removes the given value from the provided list, and returns it.
-- Shifts down any remaining integer keys.
-- Removes only the first occurrence of the given value, does nothing
-- if the value is not in the provided list.
-- @param value Any type, the value to remove
-- @param list List upon which the removal will take place
-- @return Two return values : 
-- <ul> <li> List, the provided list without the given value; </li>
-- <li> Integer, the first index where the value was found, 0 if the value was not found. </li> </ul>
utilities.removeFromList = function( value, list )
    local index = 0
    for i = 1, #list do
        if value == list[i] then
            index = i
            table.remove( list, i )
            break
        end
    end
    return list, index
end

--- Removes all the given values from the provided list, and returns it.
--- Shifts down any remaining integer keys.
-- @param values Table of values to remove
-- @param list List upon which the removal will take place
-- @return List, the provided list without the given value
utilities.removeListFromList = function( values, list )
    for _, value in pairs( values ) do
        for i = 1, #list do
            if value == list[i] then
                table.remove( list, i )
                break
            end
        end
    end
    return list
end

--- Returns a deep copy of the given table
-- @param tableFrom Table of elements to copy
-- @return Table
utilities.copyTable =  function ( tableFrom ) 
    local tableTo = {}
    for k, v in pairs( tableFrom or emptyTable ) do
        tableTo[k] = v
    end
    return tableTo
end

--- Returns the size of the given table
-- @param table Table
-- @return Integer, the number of distinct keys in the given table
utilities.tableSize = function( table )
    local cpt = 0
    for k in pairs( table or emptyTable ) do
        cpt = cpt + 1
    end
    return cpt
end


--- Clears the given table
-- @param table Table
utilities.clearTable = function( table )
    for k in pairs ( table or emptyTable ) do
        table[ k ] = nil
    end
end

--- Returns the inverse of the given table
-- For instance : if tableFrom = { 1 = "a", 2 = "b", 3 = "a" },
-- then the function returns { "a" = { 1, 3 }, "b" = { 2 } }
-- @param tableFrom Table
-- @return Table
utilities.inverseTable = function ( tableFrom )
    local tableTo = {}
    for k, v in pairs( tableFrom or emptyTable ) do
        tableTo[ v ] = tableTo[ v ] or {}
        tableTo[ v ][ #tableTo[ v ] + 1 ] = k
    end
    return tableTo
end

--- Shuffles the provided list using a Fisher-Yates shuffle
-- @param list List
-- @return List, a random permutation of the given list
utilities.shuffleList = function( list )
    for i = #list, 2, -1 do
        local j = getRandomNumber( 1, i )
        local tmp = list[i]
        list[i] = list[j]
        list[j] = tmp
    end
    return list
end

--- Creates a new table, that can be safely used with simulation sources as indexes.
-- When indexing usual tables with simulation sources,
-- the propriety "(a == b) => (t[a] == t[b])" is not necessarily true,
-- whereas it is guaranteed to be true for tables created with this method.
-- @return Table
utilities.createTableWithSimIndexes = function()
    return setmetatable( {}, { __index = function( t, k )
                                             for key, value in pairs( t ) do
                                                 if key == k then
                                                     return value
                                                 end
                                             end
                                         end,
                                         
                               __newindex = function( t, k, v )
                                                for key in pairs( t ) do
                                                    if key == k then
                                                        rawset( t, key, v )
                                                        return
                                                    end
                                                end
                                                rawset( t, k, v )
                                            end, } )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

exists = utilities.exists

existsInside = utilities.existsInside

existsIndex = utilities.existsIndex

fusionList = utilities.fusionList

removeFromList = utilities.removeFromList

removeListFromList = utilities.removeListFromList

copyTable = utilities.copyTable

tableSize = utilities.tableSize

clearTable = utilities.clearTable

integration.inverseTable = utilities.inverseTable

integration.shuffleList = utilities.shuffleList
