# VEX_json

_VEX functions to read json file._

Compilation doesn't require any dependency. It uses `UT_JSONValue` classes, distributed with Houdini, for JSON parsing, 

### Functions
 * `int jsonvalue(string inputFile, string& outputError, string& output, ... );`
 * `int jsonmapkeys(string inputFile, string& outputError, string& output[], ... );`
