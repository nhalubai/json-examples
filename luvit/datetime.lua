local http = require('http')
local testPort = process.env.PORT
if not testPort then
  print('missing env variable PORT.')
  print('Try `export PORT="3000"` and try again')
  error()
end
local fs = require('fs')
local JSON = require('json')
local os = require('os')
local math = require('math')
local pp = require('utils').prettyPrint
local location = {
      protocol = 'http'
    , hostname = '127.0.0.1'
    , port = testPort
    , pathname = '/datetime.json'
  }

local request = http.request(location, function (response)
  local body = ""

  response:on('error', function (err)
    print('response error')
    pp(err)
    error()
  end)

  response:on('data', function (chunk)
    -- print('response data')
    body = body .. chunk
  end)

  response:on('end', function ()
    -- print('response end')

    local result = JSON.parse(body).result
    local unixTime = result.unix / 1000
    local sysUnixTime = os.time()
    local timeDelta = sysUnixTime - unixTime

    if (false == (math.abs(timeDelta) < 2)) then
      print('times are ' .. timeDelta .. ' seconds apart')
      pp(result)
      error()
    end

    print('pass')
  end)
end)

request:on('error', function (err)
  print('request error')
  pp(err)
  error()
end)

request:on('end', function ()
  -- print('concluded successfully')
end)

request:done()
