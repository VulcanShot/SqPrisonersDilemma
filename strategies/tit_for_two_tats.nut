::NAME <- "TIT FOR TWO TAT";
::AUTHOR <- "TODO";

function make_move(history)
{
	local turn = history.other.len();

	if (turn < 2)
	{
		return EMove.COOPERATION;
	}

	if (history.other[turn - 1] == EMove.DEFECTION && history.other[turn - 2] == EMove.DEFECTION)
	{
		return EMove.DEFECTION;
	}

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