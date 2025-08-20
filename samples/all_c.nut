::NAME <- "ALL C";
::AUTHOR <- "None";

function make_move(history)
{
	return EMove.COOPERATION;
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