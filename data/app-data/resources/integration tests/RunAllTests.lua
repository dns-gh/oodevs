
    local function force(name)
      pcall( function() loadfile(name)() end )
    end
    force( "lunit.lua" )
    force( "lunit-console.lua" )
  require "lunit"
  local stats = lunit.main({"toolsFunctions.lua"})
  if stats.errors > 0 or stats.failed > 0 then
    os.exit(1)
  end
