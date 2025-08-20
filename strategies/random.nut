::NAME <- "RANDOM";
::AUTHOR <- "None";

function make_move(history)
{
	//TODO: fork squirrel and add proper rand and round for stdmathlib
	local randFloat = 1.0 * rand() / RAND_MAX;
	return randFloat < 0.5 ? EMove.COOPERATION : EMove.DEFECTION;
}

// enum EMove {
// 	NONE,
// 	COOPERATION,
// 	DEFECTION
// }

// local history = {
// 	self = [ EMove.COOPERATION /* ... */ ],
// 	other = [ EMove.DEFECTION /* ... */ ]
// };