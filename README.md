# VEX JSON Bindings
### Introduction
This small set of VEX functions enables to bring data from JSON file to VEX. 

 ### Installation
 Compiled `DSO` place into `~/houdiniX.X/dso/vex` directory. In addition you need to add an entry to `~/houdiniX.X/vex/VEXdso`: `vex/lib.VEX_json.dso`, so VEX engine will be albe to load it.
 
 ### Compilation and dependencies
 I recommend to use CMake file distributed with the package to control symbols visibility.
 JSON parsing is based on `HDK`'s `UT_JSONValue` utility class and therefore compilation doesn't require any external dependencies.

### VEX Functions
```c
int jsonvalue(string inputFile, string& outputError, string& output, ... );
int jsonvaluetype(string inputFile, string& outputError, string& output, ... );
int jsonmapkeys(string inputFile, string& outputError, string& output[], ... );
int jsonarraysize(string inputFile, string& outputError, int& output, ... );
 ```
 VEX functions can query strings only, that's why returned value is a string or array of strings. In some cases, `jsonarraysize`, reutrned value is an `int`.

 Functions description:
   * `jsonvalue` - returns value at specified path
   * `jsonvaluetype` - type at path: _bool_, _int_, _float_, _string_, _key_, _array_, _map_
   * `jsonmapkeys` - keys of the map at path
   * `jsonarraysize` - size of the array at path
 
 Arguments description:
  * `int` - return value: 0 - failure to get value, 1 - success
  * `string inputPath` - a full path to the file
  * `string& outputError` - error message
  * `type& output` - returned value
  * `...` - variadic path to the value, accepts string or int type.

### Example
JSON file:
```json
{
    "glossary": {
        "title": "example glossary",
        "title": "S",
        "GlossList": {
            "ID": "SGML",
            "GlossDef": {
                "para": "A meta-markup language, used to create markup languages such as DocBook.",
                "GlossSeeAlso": ["GML", "XML"]
            },
            "GlossSee": "markup"
        }
    }
}
```

VEX functions to retrieve values:
```c
string file = "example.json";
string oerr, outp;
if(!jsonvalue(file, oerr, outp, "glossary", "GlossList", "ID"))
{
    error(oerr);
}
```
