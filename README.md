# VEX JSON Bindings

## Introduction
This repo is an attempt to provide a small set of VEX functions that gives access to JSON entries in VEX. 

## Features
 * shared, threadsafe storage
 * access to data through variadic arguments
 * detailed error reporting
 * no external dependencies

## Installation
Compiled `DSO` place into `~/houdiniX.X/dso/vex` directory. 
In addition you need to add an entry to `~/houdiniX.X/vex/VEXdso`: `vex/libVEX_json.so`.

## Functions:
 * `jsonvalue` access values 
 * `jsonkeys` access dictionary keys
 * `jsontype` returns string representation of the type
 
## Supported return data types:
 * simple, such as: `int`, `float` and `string`
 * arrays of: `int`, `float` and `string`
 * dictionary keys as array of `string`
 * `vector` and `matrix` types can be handled by VEX `unserialize` function.

## Function declarations
 * `int jsonvalue(string inputFile, string& outputError, type& output, ... );`
  
   `int jsonvalue(string inputFile, string& outputError, type& output[], ... );`
   
    returns value at specified path, if path is not found returns error.

 * `int jsontype(string inputFile, string& outputError, string& output, ... );`
 
    type at path: _bool_, _int_, _float_, _string_, _key_, _array_, _map_

 * `int jsonkeys(string inputFile, string& outputError, string& output[], ... );`
 
    dictionary keys at given path

#### Function arguments
 * `int` return value: 0 - failure to get value, 1 - success
 * `string inputPath` a full path to the JSON file
 * `string& outputError` error message
 * `type& output` returned value
 * `...` variadic arguments, a path to the value, accepts string or int type only.

## Numeric data precission
Numeric data types are casted automatically based on provided output argument. For instance JSON `int` data can be read as `float`. Same for `float` can be read as `int`, but beware of precission loss.

## Compilation and dependencies
JSON parsing is based on `HDK`'s `UT_JSONValue` utility class, therefore compilation doesn't require any external dependencies.
For smart pointers standard library is used, for caching `tbb::map` and `tbb::unordered_set`.

### Example JSON
JSON file:
```json
{
    "abc_geo_count": 30,
    "anim_end": 1123,
    "anim_start": 1001,
    "asset_name": "Peter",
    "asset_scale": 1.1,
    "asset_type": "Character",
    "cache_by": 1.0,
    "cache_end": 1126,
    "cache_start": 998,
}
```

VEX functions to retrieve values:
```c
string json = ".../example2.json";
string error;
string keys[];
if(!jsonkeys(json, error, keys ))
{
    error(error);
}

foreach(string key; keys)
{
    string type;
    if(!jsontype(json, error, type, key))
    {
        error(error);
    }
    
    if(type == "int")
    {
        int value;
        if(!jsonvalue(json, error, value, key))
        {
            error(error);
        }
        setpointattrib(geoself(), key, i@ptnum, value);
    }
    else if(type == "float")
    {
        float value;
        if(!jsonvalue(json, error, value, key))
        {
            error(error);
        }
        setpointattrib(geoself(), key, i@ptnum, value);
    }
    else if(type == "string")
    {
        string value;
        if(!jsonvalue(json, error, value, key))
        {
            error(error);
        }
        setpointattrib(geoself(), key, i@ptnum, value);
    }
    
}
```

# License
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
