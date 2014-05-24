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
    points = {
        DEC_Geometrie_CreerPoint(),
        DEC_Geometrie_CreerPointLatLong(-28.3456, -15.8193),
        DEC_Geometrie_CreerPointLatLong(0, 0),
        DEC_Geometrie_CreerPointXY(10000, 20000),
    }
    result = ""
    for i = 1, #points do
        p = points[i]
        result = result .. PointToString(p) .. "\n"
    end
    expected = [[(0.00, 0.00)
(11829.77, -5316878.77)
(1750510.29, -2899797.56)
(10000.00, 20000.00)
]]
    return MakeCombinedResult(result, expected)
end
