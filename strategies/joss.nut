::NAME <- "JOSS";
::AUTHOR <- "Johann Joss";

function make_move(history)
{
	if (history.other.len() == 0)
	{
		return EMove.COOPERATION;
	}

	if (history.other.top() == EMove.COOPERATION)
	{
		local randFloat = 1.0 * rand() / RAND_MAX;
		return randFloat < 0.9 ? EMove.COOPERATION : EMove.DEFECTION;
	}

	return EMove.DEFECTION;
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