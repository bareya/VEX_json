#include "UT/UT_DSOVersion.h"

#include "VEX/VEX_VexOp.h"
#include "VEX/VEX_PodTypes.h"

#include "VEX_jsonvalue.h"
#include "VEX_jsonvaluetype.h"

void newVEXOp(void*)
{
//	int json(string json, string &errmsg, string &output, ...);
//	0			1			2				3			4
//	returns an array of keys based on provided path to the map

	std::vector<UT_StringHolder> value_defs =
	{
		"jsonvalue@&IS&S&I+", "jsonvalue@&IS&S&[I+",
		"jsonvalue@&IS&S&F+", "jsonvalue@&IS&S&[F+",
	};

	for(auto it=value_defs.begin(); it!=value_defs.end(); ++it)
	{
		new VEX_VexOp(*it,
					VEX_jsonvalue::evaluate,
					VEX_ALL_CONTEXT,
					VEX_InitJSONStorage,
					VEX_CleanupJSONStorage,
					VEX_OPTIMIZE_2,
					true
		);
	}

	// jsonvaluetype string
	new VEX_VexOp("jsonvaluetype@&IS&S&S+",
				VEX_jsonvaluetype::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);
}
