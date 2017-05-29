#include "UT/UT_DSOVersion.h"

#include "VEX/VEX_VexOp.h"
#include "VEX/VEX_PodTypes.h"

#include "VEX_jsonvalue.h"
#include "VEX_jsontype.h"
#include "VEX_jsonkeys.h"

void newVEXOp(void*)
{
	std::vector<UT_StringHolder> value_defs =
	{
		"jsonvalue@&IS&S&I+", "jsonvalue@&IS&S&[I+",
		"jsonvalue@&IS&S&F+", "jsonvalue@&IS&S&[F+",
		"jsonvalue@&IS&S&S+", "jsonvalue@&IS&S&[S+",
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

	// jsontype string
	new VEX_VexOp("jsontype@&IS&S&S+",
				VEX_jsontype::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);

	// jsonkeys
	new VEX_VexOp("jsonkeys@&IS&S&[S+",
				VEX_jsonkeys::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);
}
