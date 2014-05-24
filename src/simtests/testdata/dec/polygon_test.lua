-- ****************************************************************************
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2014 MASA Group
--
-- ****************************************************************************
require "geometry"

function TestFunction()
    local tests = {
        { nil, {
            {1, 2},
        }},
        { {}, {
            {1, 2},
        }},
        { { 5, 5 }, {
            {1, 2},
        }},
        { { 5, 5, 6, 6 }, {
            {1, 2},
        }},
        { { 0, 0, 5, 0, 5, 5, 0, 0 }, {
            {2, 1},
            -- Stupid precision
            {6, 6},
            -- This one is really outside
            {10, 10},
        }},
    }
    local output = ""
    for i = 1, #tests do
        local poly = CreatePolygonFromXY(tests[i][1])
        local points = tests[i][2]
        for j = 1, #points do
            local p = DEC_Geometrie_CreerPointXY(points[j][1], points[j][2])
            local result = DEC_Geometrie_EstPointDansLocalisation(p, poly)
            local relation = "is in"
            if not result then
                relation = "is not in"
            end
            output = output .. string.format("%s %s %s\n", PointToString(p),
                relation, PolygonToString(poly))
        end
    end
    expected = [[(1.00, 2.00) is not in nil
(1.00, 2.00) is in []
(1.00, 2.00) is in [(5.00, 5.00), ]
(1.00, 2.00) is in [(5.00, 5.00), (6.00, 6.00), ]
(2.00, 1.00) is in [(0.00, 0.00), (5.00, 0.00), (5.00, 5.00), (0.00, 0.00), ]
(6.00, 6.00) is in [(0.00, 0.00), (5.00, 0.00), (5.00, 5.00), (0.00, 0.00), ]
(10.00, 10.00) is not in [(0.00, 0.00), (5.00, 0.00), (5.00, 5.00), (0.00, 0.00), ]
]]
    return MakeCombinedResult(output, expected)
end
